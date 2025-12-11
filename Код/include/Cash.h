#pragma once

#include <stdbool.h>
#include "Enum.h"
#include "mySimpleComputer.h"

#define CACHE_LINES 5
#define CACHE_LINE_SIZE 10
#define MEMORY_SIZE 128

typedef struct 
{
    int data[CACHE_LINE_SIZE];
    int tag;                   
    bool valid;                
    bool dirty;                
    int lru;                   
} CacheLine;

typedef struct 
{
    CacheLine lines[CACHE_LINES];
    int idle_ticks; 
} Cache;

void cache_init();
int cache_read(int address, int* value);  
int cache_write(int address, int value);  
void cache_flush();                       
void cache_print(int x, int y, enum COLORS fg, enum COLORS bg);
int cache_get_idle_ticks();               
