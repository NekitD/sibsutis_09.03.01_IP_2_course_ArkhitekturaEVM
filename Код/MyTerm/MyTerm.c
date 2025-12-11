#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "MyTerm.h"

int mt_clrscr(void)
{
    printf("\033[H\033[2J");
    return 0;
}

int mt_gotoXY(int x, int y)
{
    if ((x >= 0) && (y >= 0))
    {
        printf("\033[%d;%dH", x, y);
        return 0;
    }
    else
    {
        return -1;
    }
}

int mt_getscreensize(int *rows, int *cols)
{
    if (rows == NULL || cols == NULL)
    {
        return -1;
    }
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
    {
        return -1;
    }
    else
    {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
        return 0;
    }
}

int mt_setfgcolor(enum COLORS color)
{
    printf(FGCOLOR, color);
    return 0;
}

int mt_setbgcolor(enum COLORS color)
{
    printf(BGCOLOR, color);
    return 0;
}

int mt_setdefaultcolor(void)
{
    printf("\033[39;49m");
    return 0;
}

int mt_setcursorvisible(int value)
{
    if (value == 1)
    {
        printf("\033[?25h");
    }
    else if (value == 0)
    {
        printf("\033[?25l");
    }
    else
    {
        return -1;
    }
    return 0;
}

int mt_delline(void)
{
    printf("\033[2K");
    return 0;
}
