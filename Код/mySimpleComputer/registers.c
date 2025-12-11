#include "mySimpleComputer.h"
#include <stdio.h>
#include <stdlib.h>

int accumulator;
int icounter;
int flags;

int sc_regInit(void)
{
	flags = 0;
	return 0;
}

int sc_regSet(int registerflag, int value)
{
	if (registerflag != REG_OPERATION_OVERFLOW && registerflag != REG_DIVISION_ZERO && registerflag != REG_MEMORY_OVERFLOW && registerflag != REG_INVALID_COMMAND && registerflag != REG_IMPULSE_IGNORE)
	{
		return -1;
	}

	if (value == 1)
	{
		flags |= registerflag;
	}
	else
	{
		flags &= ~registerflag;
	}

	return 0;
}

int sc_regGet(int registerflag, int *value)
{
	if (registerflag < REG_OPERATION_OVERFLOW || registerflag > REG_IMPULSE_IGNORE)
	{
		return -1;
	}

	if (value == NULL)
	{
		return -1;
	}

	*value = (flags >> (registerflag - 1)) & 1;

	return 0;
}

int sc_accumulatorInit(void)
{
	accumulator = 0;
	return 0;
}

int sc_accumulatorSet(int value)
{
	if (value < MIN_VALUE || value > MAX_VALUE)
	{
		return -1;
	}
	accumulator = value;
	return 0;
}

int sc_accumulatorGet(int *value)
{
	if (value == NULL)
	{
		return -1;
	}

	*value = accumulator;
	return 0;
}

int sc_icounterInit(void)
{
	icounter = 0;
	return 0;
}

int sc_icounterSet(int value)
{
	//	if(value < 0 || value > MEMORY_SIZE)
	//	{
	//		return -1;
	//	}
	icounter = value;
	return 0;
}

int sc_icounterGet(int *value)
{
	if (value == NULL)
	{
		return -1;
	}

	*value = icounter;
	return 0;
}
