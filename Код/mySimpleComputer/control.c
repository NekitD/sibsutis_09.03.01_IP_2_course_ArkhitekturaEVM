#include "mySimpleComputer.h"
#include <stdio.h>
#include <stdlib.h>



int sc_commandEncode(int sign, int command, int operand, int *value)
{
	if (sign < 0 || sign > 1) 
	{
        return -1;
    }
    if (command < 0 || command > 127) 
	{
        return -1;
    }
    if (operand < 0 || operand > 255) 
	{
        return -1; 
    }

    *value = (sign << 15) | (command << 8) | operand;
    return 0;
}

int sc_commandDecode(int value, int *sign, int *command, int *operand) 
{
    if (sign == NULL || command == NULL || operand == NULL) 
        return -1;

    *sign = (value & 0x8000) ? 1 : 0;
    
    *command = (value >> 8) & 0x7F;

    *operand = value & 0xFF;

    return 0;
}

int sc_commandValidate(int command) 
{
    if (command == 0x00 || command == 0x01) return 0;
    if (command >= 0x0A && command <= 0x0B) return 0;
    if (command >= 0x14 && command <= 0x15) return 0;
    if (command >= 0x1E && command <= 0x21) return 0;
    if (command >= 0x28 && command <= 0x2B) return 0;
    if (command >= 0x38 && command <= 0x3B) return 0;
    return -1;
}
