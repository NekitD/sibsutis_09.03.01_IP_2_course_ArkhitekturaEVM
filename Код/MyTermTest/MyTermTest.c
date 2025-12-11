#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include "myBigChars.h"
#include "MyTerm.h"
#include "Enum.h"
#include "mySimpleComputer.h"
#include "myReadkey.h"
#include "CU.h"
#include "Cash.h"

int main()
{
    int rows, cols;
    mt_getscreensize(&rows, &cols);
    if (rows < 30 || cols < 120)
    {
        printf("Invalid terminale size!");
        return -1;
    }

    mt_clrscr();
    char *headerRAM = " Оперативная память ";
    bc_box(1, 1, 15, 70, WHITE, BLACK, headerRAM, RED, BLACK);

    int b;
    for (b = 0; b < 128; b++)
    {
        printCell(b, 0, WHITE, BLACK);
    }

    // cache_init();
    cache_init();

    int value;
    sc_memoryGet(0, &value);

    char *headerDecode = " Редактируемая ячейка (формат) ";
    bc_box(16, 1, 3, 70, WHITE, BLACK, headerDecode, RED, WHITE);
    printDecodedCommand(value, 17, 5);

    sc_regSet(REG_OPERATION_OVERFLOW, 0);
    sc_regSet(REG_DIVISION_ZERO, 0);
    sc_regSet(REG_MEMORY_OVERFLOW, 0);
    sc_regSet(REG_IMPULSE_IGNORE, 1);
    sc_regSet(REG_INVALID_COMMAND, 0);
    char *headerAccum = " Аккумулятор ";
    bc_box(1, 71, 3, 23, WHITE, BLACK, headerAccum, RED, BLACK);
    printAccumulator(2, 73);
    char *headerFlags = " Регистр флагов ";
    bc_box(1, 95, 3, 22, WHITE, BLACK, headerFlags, RED, BLACK);
    printFlags(2, 101);
    char *headerCount = " Счетчик команд ";
    bc_box(4, 71, 3, 23, WHITE, BLACK, headerCount, RED, BLACK);
    printCounters(5, 82);
    char *headerCommand = " Команда ";
    bc_box(4, 95, 3, 22, WHITE, BLACK, headerCommand, RED, BLACK);
    printCommand(5, 102);

    char *headerBig = " Редактируемая ячейка (увеличино) ";
    bc_box(7, 71, 12, 46, WHITE, BLACK, headerBig, RED, WHITE);
    printBigCell(0, 9, 72, BLUE, GREEN, BLACK);

    char *headerKeys = " Клавиши ";
    bc_box(19, 87, 7, 30, WHITE, BLACK, headerKeys, GREEN, WHITE);
    mt_gotoXY(20, 88);
    printf("l - load");
    mt_gotoXY(21, 88);
    printf("r - run");
    mt_gotoXY(20, 98);
    printf("s - save");
    mt_gotoXY(21, 98);
    printf("t - step");
    mt_gotoXY(20, 107);
    printf("i - reset");
    mt_gotoXY(22, 88);
    printf("ESC - выход");
    mt_gotoXY(23, 88);
    printf("F5 - accumulator");
    mt_gotoXY(24, 88);
    printf("F6 - instruction counter");

    char *headerCash = " Кэш процессора ";
    bc_box(19, 1, 7, 75, WHITE, BLACK, headerCash, GREEN, WHITE);
    // printCache(19, 2);
    cache_print(2, 20, WHITE, BLACK);
    char *headerInOut = " IN--OUT ";
    bc_box(19, 76, 7, 11, WHITE, BLACK, headerInOut, GREEN, WHITE);

    mt_gotoXY(80, 1);
    init_editor();

    enum keys key;

    signal(SIGALRM, IRC);
    signal(SIGUSR1, IRC);

    while (1)
    {

        if (interactive)
        {
            if (rk_readkey(&key) == 0)
            {
                if (key == ESC)
                {
                    mt_setcursorvisible(1);
                    break;
                }
                else if (key == ENTER && interactive)
                {
                    int new_value;
                    int x = (cursor_pos % 10) * 7 + 2;
                    int y = (cursor_pos / 10) + 2;
                    mt_gotoXY(y, x);
                    printf("\033[5X");
                    fflush(stdout);
                    mt_setcursorvisible(1);

                    if (rk_readvalue(&new_value, 10) == 0)
                    {
                        sc_memorySet(cursor_pos, new_value);
                        // cache_write(cursor_pos, new_value);
                        cache_write(cursor_pos, new_value);
                        printCell(cursor_pos, cursor_pos, WHITE, BLACK);
                        printDecodedCommand(new_value, 17, 5);
                        printBigCell(cursor_pos, 9, 72, BLUE, GREEN, BLACK);
                        printCounters(5, 82);
                        printCommand(5, 102);
                        cache_print(2, 20, WHITE, BLACK);
                        fflush(stdout);
                    }
                    else
                    {
                        printCell(cursor_pos, cursor_pos, WHITE, BLACK);
                        fflush(stdout);
                    }
                    mt_setcursorvisible(0);
                    fflush(stdout);
                }
                else if (key == i)
                {
                    impulse_counter = 0;
                    IRC(SIGUSR1);
                    cursor_pos = 0;
                    program_pos = 0;
                    // cache_init();
                    printFlags(2, 101);
                    fflush(stdout);
                    for (b = 0; b < MEMORY_SIZE; b++)
                    {
                        printCell(b, 0, WHITE, BLACK);
                        fflush(stdout);
                    }
                    printCounters(5, 82);
                    printAccumulator(2, 73);
                    printCommand(5, 102);
                    printDecodedCommand(0, 17, 5);
                    printBigCell(0, 9, 72, BLUE, GREEN, BLACK);
                    cache_print(2, 20, WHITE, BLACK);
                    fflush(stdout);
                }
                else if (key == r)
                {
                    impulse_counter = 0;
                    for(impulse_counter = 1; impulse_counter > 0; impulse_counter--) 
                    {
                        printCounters(5, 82); 
                        fflush(stdout);
                        usleep(2000000);
                    }
                    int b;
                    for(b=0; b < 128; b+=1)
                    {
                        if (MEMORY[b] != 0) 
                        {
                            cache_write(b, MEMORY[b]);
                        }
                    }
                    cache_print(2, 20, WHITE, BLACK);
                    update_cell_display(cursor_pos, program_pos);
                    sc_regSet(REG_IMPULSE_IGNORE, 0);
                    interactive = 0;
                    alarm_received = false;
                    alarm(2);
                    printCounters(5, 82);
                    printFlags(2, 101);
                    fflush(stdout);
                }
                else if (key == s && interactive)
                {
                    rk_mytermsave();
                }
                else if (key == l && interactive)
                {
                    rk_mytermrestore();
                    cursor_pos = 0;
                    program_pos = 0;
                    sc_regSet(REG_IMPULSE_IGNORE, 1);
                    for(b = 0; b < 128; b++)
                    {
                        sc_memorySet(b, MEMORY[b]);
                        
                        printCell(b, 0, WHITE, BLACK);
                        fflush(stdout);
                    }
                    // for(b=0; b < 128; b+=1)
                    // {
                    //     if (MEMORY[b] != 0) 
                    //     {
                    //         cache_write(b, MEMORY[b]);
                    //     }
                    // }
                    printCommand(5, 102);
                    printDecodedCommand(MEMORY[0], 17, 5);
                    printBigCell(0, 9, 72, BLUE, GREEN, BLACK);
                    cache_print(2, 20, WHITE, BLACK);
                }
                else if (key == t)
                {

                    if (!sc_regGet(REG_IMPULSE_IGNORE, &value))
                    {
                        CU();
                        fflush(stdout);

                        for (b = 0; b < 128; b++)
                        {
                            printCell(b, icounter, WHITE, BLACK);
                        }
                        printAccumulator(2, 73);
                        printFlags(2, 101);
                        printCounters(5, 82);
                        printCommand(5, 102);
                        program_pos = icounter;
                        int v;
                        cache_read(icounter, &v);
                        printDecodedCommand(v, 17, 5);
                        printBigCell(icounter, 9, 72, BLUE, GREEN, BLACK);
                        cache_print(2, 20, WHITE, BLACK);
                        fflush(stdout);
                    }
                }

                else if (key == F5 && interactive)
                {
                    int new_ac;
                    mt_gotoXY(2, 77);
                    printf("\033[5X");
                    fflush(stdout);
                    mt_setcursorvisible(1);

                    if (rk_readvalue(&new_ac, 10) == 0)
                    {
                        sc_accumulatorSet(new_ac);
                        fflush(stdout);
                    }
                    mt_setcursorvisible(0);
                    printAccumulator(2, 73);
                    fflush(stdout);
                }
                else if (key == F6 && interactive)
                {
                    int new_count;
                    mt_gotoXY(5, 87);
                    printf("\033[4X");
                    fflush(stdout);
                    char input[5] = {0};
                    int pos = 0;

                    while (1)
                    {
                        char c;
                        if (read(STDIN_FILENO, &c, 1) != 1)
                        {
                            break;
                        }

                        c = toupper(c);
                        mt_gotoXY(5, 87 + pos);
                        printf("%c", c);
                        fflush(stdout);

                        if (c == '\n' || c == '\r')
                        {
                            if (pos > 0)
                            {
                                input[pos] = '\0';

                                char *endptr;
                                unsigned long hex_value = strtoul(input, &endptr, 16);

                                if (*endptr != '\0')
                                {
                                    break;
                                }
                                new_count = (int)hex_value;
                                if (new_count > 128)
                                {
                                    sc_regSet(REG_MEMORY_OVERFLOW, 1);
                                }
                                else
                                {
                                    sc_regSet(REG_MEMORY_OVERFLOW, 0);
                                }
                                sc_icounterSet(new_count);
                                impulse_counter = 0;
                                printFlags(2, 101);
                                printCounters(5, 82);
                                printCommand(5, 102);
                                fflush(stdout);
                                break;
                            }
                            break;
                        }
                        else if (c == 127 || c == 8)
                        {
                            if (pos > 0)
                            {
                                pos--;
                                mt_gotoXY(5, 87 + pos);
                                printf(" ");
                                fflush(stdout);
                            }
                        }
                        else if (pos < 4)
                        {
                            if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))
                            {
                                input[pos++] = c;
                            }
                        }
                    }
                }
                else if (interactive)
                {
                    move_cursor(key);
                }
            }
        }

        if (!interactive)
        {
            if (alarm_received)
            {
                alarm_received = false;

                if (!sc_regGet(REG_IMPULSE_IGNORE, &value))
                {
                    CU();
                    fflush(stdout);
                    for (b = 0; b < 128; b++)
                    {
                        printCell(b, icounter, WHITE, BLACK);
                    }
                    printAccumulator(2, 73);
                    printFlags(2, 101);
                    printCounters(5, 82);
                    printCommand(5, 102);
                    printDecodedCommand(MEMORY[icounter], 17, 5);
                    program_pos = icounter;
                    printBigCell(icounter, 9, 72, BLUE, GREEN, BLACK);
                    cache_print(2, 20, WHITE, BLACK);
                    fflush(stdout);
                }

                if (!sc_regGet(REG_IMPULSE_IGNORE, &value))
                {
                    alarm(2);
                }
                else
                {
                    interactive = 1;
                    rk_mytermregime(1, 0, 1, 0, 1);
                    update_cell_display(program_pos, cursor_pos);
                }
            }

            // if (reset_received)
            // {
            //     reset_received = false;
            //     interactive = 1;
            //     rk_mytermregime(1, 0, 1, 0, 1);
            //     update_cell_display(program_pos, cursor_pos);
            // }
        }
    }

    cache_flush();

    if (rk_mytermregime(0, 0, 1, 1, 0) != 0)
        return -1;
    mt_gotoXY(30, 1);

    return 0;
}
