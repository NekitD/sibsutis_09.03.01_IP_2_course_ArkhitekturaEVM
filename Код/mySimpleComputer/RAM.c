#include "mySimpleComputer.h"
#include <stdio.h>
#include <stdlib.h>


int MEMORY[MEMORY_SIZE]; 

int sc_memoryInit(void)
{
    int i = 0;
    for (i = 0; i < MEMORY_SIZE; i++)
    {
        MEMORY[i] = 0;
    }
    return 0;
}

int sc_memorySet(int address, int value)
{
    if ((address < 0) || (address >= MEMORY_SIZE) || (value < MIN_VALUE) || (value > MAX_VALUE))
    {
        return -1;
    }
    else
    {
        MEMORY[address] = value;
        return 0;
    }
}

int sc_memoryGet(int address, int *value)
{
    if ((address < 0) || (address >= MEMORY_SIZE) || (value == NULL))
    {
        return -1;
    }
    else
    {
        *value = MEMORY[address];
        return 0;
    }
}

int sc_memorySave(char *filename)
{
    if (filename == NULL)
    {
        return -1;
    }
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        return -1;
    }
    size_t res = fwrite(MEMORY, sizeof(int), MEMORY_SIZE, file);

    fclose(file);
    if (res != MEMORY_SIZE)
    {
        return -1;
    }
    return 0;
}

int sc_memoryLoad(char *filename)
{
    if (filename == NULL)
    {
        return -1;
    }
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        return -1;
    }
    size_t res = fread(MEMORY, sizeof(int), MEMORY_SIZE, file);

    fclose(file);
    if (res != MEMORY_SIZE)
    {
        return -1;
    }
    return 0;
}
