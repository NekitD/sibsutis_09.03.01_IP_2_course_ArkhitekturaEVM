#include "mySimpleComputer.h"
#include "MyTerm.h"
#include "Enum.h"
#include "myBigChars.h"
#include "myReadkey.h"
#include "CU.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define FONT_FILE "./font.bin"
#define FONT_WIDTH 8
#define FONT_HEIGHT 8
#define TERM_LINES 5
#define NUM_SYMBOLS 18

unsigned char font[NUM_SYMBOLS][FONT_HEIGHT];
int cursor_pos = 0;
int program_pos = 0;

void printCell(int address, int currentAddress, enum COLORS fg, enum COLORS bg)
{
    int value;
    if (address < 0 || address >= 128)
    {
        return;
    }

    if (sc_memoryGet(address, &value) != 0)
    {
        return;
    }

    int sign = (value >> 14) & 1;
    int magnitude = value & 0x3FFF;

    int command = (magnitude >> 7) & 0x7F;
    int operand = magnitude & 0x7F;

    mt_setfgcolor(fg);
    mt_setbgcolor(bg);

    int row = 2 + (address / 10);
    int col = 2 + (address % 10) * 7;

    if (row >= 0 && col >= 0)
    {
        mt_gotoXY(row, col);

        if ((row == 2 + (currentAddress / 10)) && (col == 2 + (currentAddress % 10) * 7))
        {
            mt_setfgcolor(bg);
            mt_setbgcolor(fg);
        }

        printf("%c%02X%02X", sign ? '-' : '+', command, operand);

        mt_setfgcolor(DEFAULT);
        mt_setbgcolor(DEFAULT);
    }
}

void printFlags(int row, int col)
{
    if (row >= 0 && col >= 0)
    {
        mt_gotoXY(row, col);
        printf("%c %c %c %c %c\n",
               (flags & REG_OPERATION_OVERFLOW) ? 'P' : '_', // Operation Overflow
               (flags & REG_DIVISION_ZERO) ? 'O' : '_',      // Division by Zero
               (flags & REG_MEMORY_OVERFLOW) ? 'M' : '_',    // Memory Overflow
               (flags & REG_IMPULSE_IGNORE) ? 'T' : '_',     // Impulse Ignore
               (flags & REG_INVALID_COMMAND) ? 'E' : '_');   // Invalid Command
    }
}

void printDecodedCommand(int value, int row, int col)
{
    if (row < 0 || col < 0)
        return;

    mt_gotoXY(row, col);

    int limited_value = value;
    if (value > 16383)
        limited_value = 16383;
    if (value < -16383)
        limited_value = -16383;

    int encoded_value = limited_value & 0x7FFF;

    int sign = (limited_value < 0);
    printf("Dec: %c%05d |", sign ? '-' : '+', sign ? -limited_value : limited_value);

    printf(" Oct: %05o |", encoded_value);

    printf(" Hex: %04X |", encoded_value);

    printf(" Bin: %d ", sign);
    int i;
    for (i = 13; i >= 7; i--)
    {
        printf("%d", (encoded_value >> i) & 1);
    }
    printf(" ");
    for (i = 6; i >= 0; i--)
    {
        printf("%d", (encoded_value >> i) & 1);
    }

    mt_gotoXY(row + 1, col);
}

void printAccumulator(int row, int col)
{
    if (row >= 0 && col >= 0)
    {
        mt_gotoXY(row, col);

        int sign = (accumulator >> 14) & 1;
        int magnitude = accumulator & 0x3FFF;

        int command = (magnitude >> 7) & 0x7F;
        int operand = magnitude & 0x7F;

        printf("sc: %c%02X%02X ", sign ? '-' : '+', command, operand);

        int limited_value = accumulator;
        int encoded_value = limited_value & 0x7FFF;
        printf("hex: %04X", encoded_value);
    }
}

void printCounters(int row, int col)
{
    if (row >= 0 && col >= 0)
    {
        mt_gotoXY(row, col - 10);
        printf("\033[5X");
        printf("T: %d", impulse_counter);
        mt_gotoXY(row, col);
        printf("IC: %c%04X", (icounter >= 0) ? '+' : '-', icounter);
    }
}

void printTerm(int address, int input, int row, int col)
{
    static int addresses[TERM_LINES];
    static int inputs[TERM_LINES];
    static int index = 0;
    static int count = 0;

    addresses[index] = address;
    inputs[index] = input;
    index = (index + 1) % TERM_LINES;
    if (count < TERM_LINES)
    {
        count++;
    }

    for (int i = 0; i < TERM_LINES; i++)
    {
        mt_gotoXY(row + i, col);
        printf("         ");
    }

    for (int i = 0; i < count; i++)
    {
        int currentIndex = (index - count + i + TERM_LINES) % TERM_LINES;
        int currentAddress = addresses[currentIndex];
        int currentInput = inputs[currentIndex];
        int value;
        mt_gotoXY(row + i, col);

        if (currentInput)
        {
            if (sc_memoryGet(currentAddress, &value) == 0)
            {
                int sign = (value >> 14) & 1;
                int magnitude = value & 0x3FFF;

                int command = (magnitude >> 7) & 0x7F;
                int operand = magnitude & 0x7F;

                printf("%02d< %c%02X%02X", 
                    currentAddress, 
                    sign ? '-' : '+',
                    command,
                    operand);
            }
            else
            {
                printf("%02d  !Error", currentAddress);
            }
        }
        else
        {
            if (sc_memoryGet(currentAddress, &value) == 0)
            {
                int sign = (value >> 14) & 1;
                int magnitude = value & 0x3FFF;

                int command = (magnitude >> 7) & 0x7F;
                int operand = magnitude & 0x7F;

                printf("%02d> %c%02X%02X",
                       currentAddress,
                       sign ? '-' : '+',
                       command,
                       operand);
            }
            else
            {
                printf("%02d  !Error", currentAddress);
            }
        }
    }
}

void printCommand(int row, int col)
{
    int value;
    int sign, command, operand;

    if (sc_memoryGet(icounter, &value) == -1)
    {
        sc_regSet(REG_INVALID_COMMAND, 1);
        printFlags(2, 101);
        return;
    }

    sign = (value >> 14) & 1;
    command = (value >> 7) & 0x7F;
    operand = value & 0x7F;

    if (icounter < 0 || icounter >= 128)
    {
        sc_regSet(REG_INVALID_COMMAND, 1);
        printFlags(2, 101);
        mt_gotoXY(row, col - 2);
        printf("! %c %02X : %02X", sign ? '-' : '+', command, operand);
        return;
    }

    if (sc_commandValidate(command) != 0)
    {
        sc_regSet(REG_INVALID_COMMAND, 1);
        printFlags(2, 101);
        mt_gotoXY(row, col - 2);
        printf("! %c %02X : %02X", sign ? '-' : '+', command, operand);
    }
    else
    {
        sc_regSet(REG_INVALID_COMMAND, 0);
        printFlags(2, 101);
        mt_gotoXY(row, col - 2);
        printf("  %c %02X : %02X", sign ? '-' : '+', command, operand);
    }
}

int load_font(const char *filename, unsigned char *font_data)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        return -1;
    }

    for (int i = 0; i < NUM_SYMBOLS; i++)
    {
        if (read(fd, &font_data[i * FONT_HEIGHT], FONT_HEIGHT) != FONT_HEIGHT)
        {
            close(fd);
            return -1;
        }
    }

    close(fd);
    return 0;
}

void printBigCell(int address, int row, int col, enum COLORS addr_color, enum COLORS fg_color, enum COLORS bg_color)
{
    int value;
    unsigned char font_data[NUM_SYMBOLS * FONT_HEIGHT];

    if (load_font(FONT_FILE, font_data))
    {
        return;
    }

    if (sc_memoryGet(address, &value))
    {
        return;
    }

    int sign = (value >> 14) & 1;
    int magnitude = value & 0x3FFF;

    int command = (magnitude >> 7) & 0x7F;
    int operand = magnitude & 0x7F;

    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%c%02X%02X", sign ? '-' : '+', command, operand);

    mt_setfgcolor(fg_color);
    mt_setbgcolor(bg_color);

    int bigchars[NUM_SYMBOLS][2] = {0};
    for (int i = 0; i < NUM_SYMBOLS; i++)
    {
        for (int y = 0; y < FONT_HEIGHT; y++)
        {
            unsigned char row_data = font_data[i * FONT_HEIGHT + y];
            for (int x = 0; x < FONT_WIDTH; x++)
            {
                int pixel = (row_data >> (FONT_WIDTH - x - 1)) & 1;
                bc_setbigcharpos(bigchars[i], y, x, pixel);
            }
        }
    }

    for (int i = 0; i < 5; i++)
    {
        char ch = buffer[i];
        int index;

        if (ch >= '0' && ch <= '9')
        {
            index = ch - '0';
        }
        else if (ch >= 'A' && ch <= 'F')
        {
            index = ch - 'A' + 10;
        }
        else if (ch == '+')
        {
            index = 16;
        }
        else if (ch == '-')
        {
            index = 17;
        }
        else
        {
            index = 0;
        }

        bc_printbigchar(bigchars[index], row, col + i * (FONT_WIDTH + 1), fg_color, bg_color);
    }

    mt_gotoXY(row + 8, col);
    mt_setfgcolor(addr_color);
    printf("Номер редактируемой ячейки: %03d", address);

    mt_setfgcolor(DEFAULT);
    mt_setbgcolor(DEFAULT);
}

void move_cursor(enum keys key)
{
    int old_pos = cursor_pos;
    int row = cursor_pos / 10;
    int col = cursor_pos % 10;
    int max_row = 12;
    int max_col_last_row = 7;

    switch (key)
    {
    case UP:
        if (row == 0)
        {
            row = max_row;
            if (col > max_col_last_row)
            {
                col = max_col_last_row;
            }
        }
        else
        {
            row--;
        }
        break;

    case DOWN:
        if (row == max_row)
        {
            row = 0;
        }
        else
        {
            row++;
            if (row == max_row && col > max_col_last_row)
            {
                col = max_col_last_row;
            }
        }
        break;

    case LEFT:
        if (col == 0)
        {
            if (row == 0)
            {
                row = max_row;
                col = max_col_last_row;
            }
            else
            {
                row--;
                col = (row == max_row) ? max_col_last_row : 9;
            }
        }
        else
        {
            col--;
        }
        break;

    case RIGHT:
        if ((row == max_row && col == max_col_last_row) || (row != max_row && col == 9))
        {
            if (row == max_row)
            {
                row = 0;
                col = 0;
            }
            else
            {
                row++;
                col = 0;
            }
        }
        else
        {
            col++;
        }
        break;

    default:
        return;
    }

    if (row == max_row && col > max_col_last_row)
    {
        col = max_col_last_row;
    }

    cursor_pos = row * 10 + col;

    if (cursor_pos >= 0 && cursor_pos < MEMORY_SIZE)
    {
        update_cell_display(old_pos, cursor_pos);
        update_cell_display(program_pos, cursor_pos);
    }
    else
    {
        cursor_pos = old_pos;
    }
}

void update_cell_display(int old_pos, int new_pos)
{
    // printCell(old_pos, new_pos, WHITE, BLACK);
    int b;
    for(b = 0; b < 128; b++)
    {
        printCell(b, new_pos, WHITE, BLACK);
    }
    

    int value;
    if (sc_memoryGet(new_pos, &value) != 0)
    {
        return;
    }

    printBigCell(new_pos, 9, 72, BLUE, GREEN, BLACK);

    printDecodedCommand(value, 17, 5);

    mt_setfgcolor(DEFAULT);
    mt_setbgcolor(DEFAULT);
    fflush(stdout);
}

void init_editor()
{
    cursor_pos = 0;
    program_pos = 0;
    rk_mytermregime(1, 0, 1, 0, 1);
    update_cell_display(0, 0);
}
