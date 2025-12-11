#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "Enum.h"
#include "myBigChars.h"

#define MEMORY_SIZE 128
#define MIN_VALUE -16384
#define MAX_VALUE 16383

#define REG_OPERATION_OVERFLOW (1 << 0) // 00001
#define REG_DIVISION_ZERO (1 << 1)      // 00010
#define REG_MEMORY_OVERFLOW (1 << 2)    // 00100
#define REG_INVALID_COMMAND (1 << 3)    // 01000
#define REG_IMPULSE_IGNORE (1 << 4)     // 10000

#define TERM_LINES 5

extern int accumulator;
extern int icounter;
extern int flags;

extern int MEMORY[MEMORY_SIZE];
extern int cursor_pos;
extern int program_pos;

int sc_memoryInit(void);
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int *value);
int sc_memorySave(char *filename);
int sc_memoryLoad(char *filename);

int sc_regInit(void);
int sc_regSet(int registerflag, int value);
int sc_regGet(int registerflag, int *value);
int sc_accumulatorInit(void);
int sc_accumulatorSet(int value);
int sc_accumulatorGet(int *value);
int sc_icounterInit(void);
int sc_icounterSet(int value);
int sc_icounterGet(int *value);

int sc_commandEncode(int sign, int command, int operand, int *value);
int sc_commandDecode(int value, int *sign, int *command, int *operand);
int sc_commandValidate(int command);

void printCell(int address, int currentAddress, enum COLORS fg, enum COLORS bg);
void printFlags(int row, int col);
void printDecodedCommand(int value, int row, int col);
void printAccumulator(int row, int col);
void printCounters(int row, int col);
void printTerm(int address, int input, int row, int col);
void printCommand(int row, int col);
int load_font(const char *filename, unsigned char *font_data);
void printBigCell(int address, int row, int col, enum COLORS addr_color, enum COLORS fg_color, enum COLORS bg_color);
void move_cursor(enum keys key);
void update_cell_display(int old_pos, int new_pos);
void init_editor();

void cache_init();
void printCache(int start_row, int start_col);
