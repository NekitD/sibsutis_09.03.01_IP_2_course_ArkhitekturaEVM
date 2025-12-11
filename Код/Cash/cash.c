#include "Cash.h"
#include "MyTerm.h"
#include "Enum.h"
#include "mySimpleComputer.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static Cache cache;

void cache_init() 
{
    memset(&cache, 0, sizeof(Cache));
    for (int i = 0; i < CACHE_LINES; i++) 
    {
        cache.lines[i].valid = false;
        cache.lines[i].dirty = false;
        cache.lines[i].lru = i;
        cache.lines[i].tag = -1;
    }
    cache.idle_ticks = 0;
}


static int cache_find_line(int tag) 
{
    for (int i = 0; i < CACHE_LINES; i++) 
    {
        if (cache.lines[i].valid && cache.lines[i].tag == tag)
        {
            return i;
        }   
    }
    return -1;
}


static int cache_find_lru() 
{
    int max_lru = -1;
    int idx = 0;
    
    for (int i = 0; i < CACHE_LINES; i++) 
    {
        if (cache.lines[i].valid && cache.lines[i].lru > max_lru) 
        {
            max_lru = cache.lines[i].lru;
            idx = i;
        }
    }
    return idx;
}

static void cache_update_lru(int used_idx) 
{
    
    cache.lines[used_idx].lru = 0;
    
    for (int i = 0; i < CACHE_LINES; i++) 
    {
        if (i != used_idx && cache.lines[i].valid) 
        {
            cache.lines[i].lru++;
        }
    }
}


static void cache_writeback(int idx) 
{
    if(!cache.lines[idx].valid || !cache.lines[idx].dirty)
    {
        return;
    } 
    int base_addr = cache.lines[idx].tag * CACHE_LINE_SIZE;
    for(int i = 0; i < CACHE_LINE_SIZE; i++) 
    {
        if(base_addr + i < MEMORY_SIZE)
        {
            sc_memorySet(base_addr + i, cache.lines[idx].data[i]);
        }
    }
    cache.lines[idx].dirty = false;
}

static void cache_load_line(int idx, int tag) 
{
    int base_addr = tag * CACHE_LINE_SIZE;
    for(int i = 0; i < CACHE_LINE_SIZE; i++) 
    {
        int val = 0;
        if(base_addr + i < MEMORY_SIZE)
        {
            sc_memoryGet(base_addr + i, &val);
        } 
        cache.lines[idx].data[i] = val;
    }
    cache.lines[idx].tag = tag;
    cache.lines[idx].valid = true;
    cache.lines[idx].dirty = false;
}

int cache_read(int address, int* value) 
{
    impulse_counter = 1;
    printCounters(5, 82); 
    for(impulse_counter = 2; impulse_counter > 0; impulse_counter--) 
    {
        printCounters(5, 82); 
        fflush(stdout);
        usleep(1000000);
    }
    impulse_counter = 0;
    printCounters(5, 82); 
    if (address < 0 || address >= MEMORY_SIZE) return -1;
    
    int tag = address / CACHE_LINE_SIZE;
    int offset = address % CACHE_LINE_SIZE;
    int idx = cache_find_line(tag);

    if (idx >= 0) 
    { 

        *value = cache.lines[idx].data[offset];
        cache_update_lru(idx); 
        cache.idle_ticks = 1;
        return 0;
    } 
    else 
    {
        
        int lru_idx = cache_find_lru();
        
        for (int i = 0; i < CACHE_LINES; i++) 
        {
            if (!cache.lines[i].valid) 
            {
                lru_idx = i;
                break;
            }
        }

        cache_writeback(lru_idx);
        for(impulse_counter = 8; impulse_counter > 0; impulse_counter--) 
        {
            printCounters(5, 82);
            fflush(stdout);
            usleep(1000000);
        }
        cache_load_line(lru_idx, tag);
        cache_update_lru(lru_idx);
        *value = cache.lines[lru_idx].data[offset];
        cache.idle_ticks = 10;
        return 0;
    }
}

int cache_write(int address, int value) 
{
    if (address < 0 || address >= MEMORY_SIZE) return -1;
    
    int tag = address / CACHE_LINE_SIZE;
    int offset = address % CACHE_LINE_SIZE;
    int idx = cache_find_line(tag);

    if (idx < 0) 
    {
        
        int lru_idx = cache_find_lru();
        
        for (int i = 0; i < CACHE_LINES; i++) 
        {
            if (!cache.lines[i].valid) 
            {
                lru_idx = i;
                break;
            }
        }

        cache_writeback(lru_idx);
        for(impulse_counter = 8; impulse_counter > 0; impulse_counter--) 
        {
            printCounters(5, 82);
            fflush(stdout);
            usleep(1000000); // 1 секунда = 1 000 000 микросекунд
        }
        cache_load_line(lru_idx, tag);
        cache_update_lru(lru_idx);
        idx = lru_idx;
        cache.idle_ticks = 10;
    } 
    else 
    {
        cache_update_lru(idx);
        cache.idle_ticks = 1;
    }
    
    cache.lines[idx].data[offset] = value;
    cache.lines[idx].dirty = true;
    return 0;
}


void cache_flush() 
{
    for(int i = 0; i < CACHE_LINES; i++)
    {
        cache_writeback(i);
    }
        
}

int cache_get_idle_ticks() 
{
    return cache.idle_ticks;
}

void cache_print(int x, int y, enum COLORS fg, enum COLORS bg) 
{
    for (int line = 0; line < CACHE_LINES; line++) 
    {
        mt_gotoXY(y + line, x);

        if(!cache.lines[line].valid || cache.lines[line].tag == -1) 
        {
            printf(" -1:"); 
            for(int i = 0; i < CACHE_LINE_SIZE - 1; i++) 
            {
                printf("       ");
            }
            printf("      ");
            printf("\n");
            continue;
        }

        int base_addr = cache.lines[line].tag * CACHE_LINE_SIZE;

        printf("%3d:  ", base_addr);

        for(int i = 0; i < CACHE_LINE_SIZE; i++) 
        {
            int addr = base_addr + i;
            if(addr >= MEMORY_SIZE) 
            {
                printf("       ");
                continue;
            }

            int value = cache.lines[line].data[i];

            int sign = (value >> 14) & 1;
            int magnitude = value & 0x3FFF;
            int command = (magnitude >> 7) & 0x7F;
            int operand = magnitude & 0x7F;

            mt_setfgcolor(fg);
            mt_setbgcolor(bg);

            printf("%c%02X%02X", sign ? '-' : '+', command, operand);

            mt_setfgcolor(DEFAULT);
            mt_setbgcolor(DEFAULT);


            if (i != CACHE_LINE_SIZE - 1) 
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}
