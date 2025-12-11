#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "Enum.h"

#define SIZE 128
#define MIN_VALUE -16384
#define MAX_VALUE 16383

#define FGCOLOR "\033[3%dm"
#define LIGHT_FGCOLOR "\033[9%dm"
#define BGCOLOR "\033[4%dm"
#define LIGHT_BGCOLOR "\033[10%dm"



int mt_clrscr(void);
int mt_gotoXY(int x, int y);
int mt_getscreensize(int *rows, int *cols);
int mt_setfgcolor(enum COLORS color);
int mt_setbgcolor(enum COLORS color);
int mt_setdefaultcolor(void);
int mt_setcursorvisible(int value);
int mt_delline(void);
