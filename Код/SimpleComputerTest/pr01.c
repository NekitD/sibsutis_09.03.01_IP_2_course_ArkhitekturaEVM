#include "mySimpleComputer.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    int status;

    status = sc_memoryInit();
    printf("Memory initialization status: %d\n", status);
    status = sc_regInit();
    printf("Flags initialization status: %d\n", status);
    status = sc_accumulatorInit();
    printf("Accumulator initialization status: %d\n", status);
    status = sc_icounterInit();
    printf("Instruction counter initialization status: %d\n\n", status);

    sc_memorySet(0, -1234);
    sc_memorySet(1, 5678);
    sc_memorySet(2, 9012);
    sc_memorySet(3, 3456);
    sc_memorySet(4, 7890);
    printf("Memory content (decoded):\n");
    int j;
    for (j = 0; j < MEMORY_SIZE; j++)
    {
        // printCell(j);
    }
    printf("\n");

    status = sc_memorySet(130, 9999);
    printf("Attempt to set an invalid memory address status: %d\n", status);
    status = sc_memorySet(9, 2147483645);
    printf("Attempt to set an invalid value status: %d\n", status);

    sc_regSet(REG_OPERATION_OVERFLOW, 1);
    sc_regSet(REG_DIVISION_ZERO, 1);
    sc_regSet(REG_MEMORY_OVERFLOW, 0);
    sc_regSet(REG_IMPULSE_IGNORE, 0);
    sc_regSet(REG_INVALID_COMMAND, 1);
    printf("Flags content:\n");
    // printFlags();
    printf("\n");

    status = sc_regSet(999, 1);
    printf("Attempt to set an invalid flag status: %d\n", status);

    sc_accumulatorSet(12);
    printf("Accumulator value: ");
    // printAccumulator();
    printf("\n");

    status = sc_accumulatorSet(2147483645);
    printf("Attempt to set an invalid value of accumulator status: %d\n", status);

    sc_icounterSet(1);
    printf("Instruction Counter value: ");
    // printCounters();
    printf("\n");

    status = sc_icounterSet(200);
    printf("Attempt to set invalid value of instruction counter status: %d\n", status);

    int value;
    int sign, command, operand;

    sc_memoryGet(0, &value);
    sc_commandDecode(value, &sign, &command, &operand);
    printf("Decoded memory cell 0: Sign: %d, Command: %d, Operand: %d\n", sign, command, operand);

    sc_accumulatorGet(&value);
    sc_commandDecode(value, &sign, &command, &operand);
    printf("Decoded accumulator: Sign: %d, Command: %d, Operand: %d\n", sign, command, operand);

    int encodedValue;
    status = sc_commandEncode(1, 2, 5, &encodedValue);
    if (status == 0)
    {
        printf("Encoded value in different bases:\n");
        // printDecodedCommand(encodedValue);
    }
    else
    {
        printf("Failed to encode command!\n");
    }


    return 0;
}
