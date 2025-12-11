#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "MyTerm.h"
#include "Enum.h"

int bc_strlen(char *str)
{
    if (str == NULL)
    {
        return 0;
    }

    int len = 0;

    while (*str)
    {
        if ((*str & 0b11000000) != 0b10000000)
        {
            len++;
        }    
        str++;
    }

    return len;
}

int bc_printA(char *str)
{
    if (str == NULL)
    {
        return -1;
    }
    printf("\033(0%s\033(B", str);

    return 0;
}

int bc_box(int x1, int y1, int x2, int y2, enum COLORS box_fg, enum COLORS box_bg, char *header, enum COLORS header_fg, enum COLORS header_bg)
{
    if (x1 < 1 || y1 < 1 || x2 < 1 || y2 < 1 || header == NULL)
    {
        return -1;
    }

    // printf("\033(0");

    mt_setfgcolor(box_fg);
    mt_setbgcolor(box_bg);

    int i;

    for (i = 0; i < x2; i++)
    {
        printf("\033[%d;%dH", x1 + i, y1);

        if (i == 0)
        {
            bc_printA("l");
            for (int j = 1; j < y2 - 1; j++)
            {
                bc_printA("q");
            }
            bc_printA("k");
        }
        else if(i == (x2 - 1))
        {
            bc_printA("m");
            for (int j = 1; j < y2 - 1; j++)
            {
                bc_printA("q");
            }
            bc_printA("j");
        }
        else
        {
            bc_printA("x");
            for (int j = 1; j < y2 - 1; j++)
            {
                bc_printA(" ");
            }
            bc_printA("x");
        }
    }
    // printf("\033(B");
    if (header)
    {
        int header_len = bc_strlen(header);
        if (header_len == 0)
        {
            return -1;
        }
        if (header_len > y2 - 2)
        {
            header_len = y2 - 2;
        }
        int header_x = x1;
        int header_y = y1 + (y2 - header_len) / 2;

        printf("\033[%d;%dH", header_x, header_y);
        mt_setfgcolor(header_fg);
        mt_setbgcolor(header_bg);

        printf("%s", header);
    }

    mt_setdefaultcolor();

    return 0;
}

int bc_setbigcharpos(int *big, int x, int y, int value)
{
    if (x < 0 || x > 7 || y < 0 || y > 7)
    {
        return -1;
    }

    int index = x / 4;
    int bit = (x % 4) * 8 + y;

    if (value)
    {
        big[index] |= (1 << bit);
    }
    else
    {
        big[index] &= ~(1 << bit);
    }
    return 0;
}

int bc_getbigcharpos(int *big, int x, int y, int *value)
{
    if (x < 0 || x > 7 || y < 0 || y > 7)
    {
        return -1;
    }
    int index = x / 4;
    int bit = (x % 4) * 8 + y;

    *value = (big[index] >> bit) & 1;
    return 0;
}

int bc_printbigchar(int big[2], int x_pos, int y_pos, enum COLORS fg, enum COLORS bg)
{
    if (x_pos < 1 || y_pos < 1)
    {
        return -1;
    }

    mt_setfgcolor(fg);
    mt_setbgcolor(bg);

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            int value;
            bc_getbigcharpos(big, x, y, &value);
            printf("\033[%d;%dH", x_pos + x, y_pos + y);
            bc_printA(value ? "a" : " ");
        }
    }
    mt_setdefaultcolor();
    return 0;
}

int bc_bigcharwrite(int fd, int *big, int count)
{
    if (write(fd, big, count * sizeof(int) * 2) != count * sizeof(int) * 2)
    {
        return -1;
    }
    return 0;
}

int bc_bigcharread(int fd, int *big, int need_count, int *count)
{
    *count = read(fd, big, need_count * sizeof(int) * 2) / (sizeof(int) * 2);
    if (*count > 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

