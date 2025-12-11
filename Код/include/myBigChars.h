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

int bc_strlen(char *str);
int bc_printA(char *str);
int bc_box(int x1, int y1, int x2, int y2, enum COLORS box_fg, enum COLORS box_bg, char *header, enum COLORS header_fg, enum COLORS header_bg);
int bc_setbigcharpos(int *big, int x, int y, int value);
int bc_getbigcharpos(int *big, int x, int y, int *value);
int bc_printbigchar(int big[2], int x, int y, enum COLORS fg, enum COLORS bg);
int bc_bigcharwrite(int fd, int *big, int count);
int bc_bigcharread(int fd, int *big, int need_count, int *count);
