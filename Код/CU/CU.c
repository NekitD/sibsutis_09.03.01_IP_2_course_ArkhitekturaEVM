#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include "Enum.h"
#include "mySimpleComputer.h"
#include "myReadkey.h"
#include "MyTerm.h"
#include "CU.h"
#include "Cash.h"

#define VALUE_MASK 0x3FFF

int interactive = 1;
int impulse_counter = 0;
volatile sig_atomic_t alarm_received = false;

void CU()
{
    // impulse_counter++;
    int value;
    int command;
    int operand;

    if (icounter > 127 || icounter < 0)
    {
        sc_regSet(REG_MEMORY_OVERFLOW, 1);
        sc_regSet(REG_IMPULSE_IGNORE, 1);
        icounter = 0;
        return;
    }

    if (cache_read(icounter, &value) != 0) 
    {
        sc_regSet(REG_IMPULSE_IGNORE, 1);
        return;
    }

    int magnitude = value & 0x3FFF;
    command = (magnitude >> 7) & 0x7F;
    operand = magnitude & 0x7F;

    if (sc_commandValidate(command) != 0)
    {
        sc_regSet(REG_INVALID_COMMAND, 1);
        sc_regSet(REG_IMPULSE_IGNORE, 1);
        interactive = 1;
        return;
    }

    if (operand >= MEMORY_SIZE)
    {
        sc_regSet(REG_INVALID_COMMAND, 1);
        sc_regSet(REG_IMPULSE_IGNORE, 1);
        return;
    }

    if (command >= 0x1E && command <= 0x21)
    {
        if (ALU(command, operand) != 0)
        {
            sc_regSet(REG_IMPULSE_IGNORE, 1);
            return;
        }
    }

    switch (command)
    {
    case 0x00: // NOP
        break;

    case 0x01:
        mt_gotoXY(28, 0);
        printf("ИП-312 Грязин Алексей Владимирович, Дорогин Никита Сергеевич");
        fflush(stdout);
        break;

    case 0x0A: // READ
        mt_gotoXY(28, 0);
        printf("                                                                                             ");
        mt_gotoXY(28, 0);
        printf("Input: \n");
        mt_gotoXY(28, 8);
        rk_readvalue(&value, 10);
        cache_write(operand, value); 
        printTerm(operand, 1, 20, 77);
        break;

    case 0x0B: // WRITE
        cache_read(operand, &value); 
        mt_gotoXY(28, 0);
        printf("                                                                                              ");
        int sign = (value >> 14) & 1;
        int magnitude = value & 0x3FFF;
        int command = (magnitude >> 7) & 0x7F;
        int operand_out = magnitude & 0x7F;
        mt_gotoXY(28, 0);
        printf("OUT: %c%02X%02X", sign ? '-' : '+', command, operand_out);
        printTerm(operand, 0, 20, 77);
        break;

    case 0x14: // LOAD
        cache_read(operand, &value); 
        accumulator = value;
        break;

    case 0x15: // STORE
        cache_write(operand, accumulator); 
        break;

        case 0x28: // JUMP
            if (operand == icounter) 
            {
                sc_regSet(REG_INVALID_COMMAND, 1);
                sc_regSet(REG_IMPULSE_IGNORE, 1);
                return;
            }
            icounter = operand;
            break;

        case 0x29: // JNEG
            if (accumulator & 0x4000) 
            {
                icounter = operand;
            }
            else
            {
                icounter++;
            }
            break;

        case 0x2A: // JZ
            if ((accumulator & 0x3FFF) == 0) 
            {
                icounter = operand;
            }
            break;

        case 0x2B: // HALT
            sc_regSet(REG_IMPULSE_IGNORE, 1);
            interactive = 1;
            icounter--;
            // impulse_counter--;
            cache_flush();
            break;

        case 0x38: // JC (56)
            int flag;
            sc_regGet(REG_OPERATION_OVERFLOW, &flag);
            if (flag) icounter = operand;
            else icounter++;
            break;

        case 0x39: // JNC (57)
            int flagt;
            sc_regGet(REG_OPERATION_OVERFLOW, &flagt);
            if (!flagt) icounter = operand;
            else icounter++;
            break;

        case 0x3A: // JP (58)
            if ((accumulator & 0x0001) == 0) icounter = operand;
            else icounter++;
            break;

        case 0x3B: // JNP (59)
            if (accumulator & 0x0001) icounter = operand;
            else icounter++;
            break;

        default:
            break;
    }

    cache_print(2, 20, WHITE, BLACK);
    fflush(stdout);

    if (!(command == 0x28 || command == 0x29 || command == 0x2A || 
          command == 0x38 || command == 0x39 || command == 0x3A || command == 0x3B)) {
        icounter++;
    }
}

int ALU(int command, int operand)
{
    int mem_value;
    if (cache_read(operand, &mem_value) != 0) 
    {
        return -1;
    }

    int result;
    switch (command) 
    {
        case 0x1E: // ADD
            result = accumulator + mem_value;
            break;
        case 0x1F: // SUB
            result = accumulator - mem_value;
            break;
        case 0x20: // DIVIDE
            if (mem_value == 0) 
            {
                sc_regSet(REG_DIVISION_ZERO, 1);
                return -1;
            }
            result = accumulator / mem_value;
            break;
        case 0x21: // MUL
            result = accumulator * mem_value;
            break;
        default:
            return -1;
    }

    if(result > MAX_VALUE || result < MIN_VALUE) 
    {
        sc_regSet(REG_OPERATION_OVERFLOW, 1);
        int sign = (result < 0) ? -1 : 1;
        unsigned int abs_value = (unsigned int)(sign * result);
        abs_value &= VALUE_MASK;
        result = sign * (int)abs_value;
    }
    
    accumulator = result;
    return 0;
}

void IRC(int signum)
{
    mt_gotoXY(29, 0);
    mt_delline();
    switch (signum) {
        case SIGALRM:
            alarm_received = true;
            printf("Получен сигнал SIGALRM от генератора импульсов\n");
            fflush(stdout);
            break;

        case SIGUSR1:
            sc_regSet(REG_OPERATION_OVERFLOW, 0);
            sc_regSet(REG_DIVISION_ZERO, 0);
            sc_regSet(REG_MEMORY_OVERFLOW, 0);
            sc_regSet(REG_IMPULSE_IGNORE, 1);
            sc_regSet(REG_INVALID_COMMAND, 0);
            sc_accumulatorSet(0);
            sc_icounterSet(0);
            interactive = 1;
            for (int b = 0; b < MEMORY_SIZE; b++) 
            {
                sc_memorySet(b, 0);
            }
            cache_flush();
            cache_init();
            printf("Получен сигнал SIGUSR1 от Reset\n");
            fflush(stdout);
            break;

        default:
            printf("Получен неизвестный сигнал: %d\n", signum);
            fflush(stdout);
            break;
    }
}
