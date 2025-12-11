#pragma once

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

extern int interactive;
// extern int impulse_counter;
extern volatile sig_atomic_t alarm_received;
// extern volatile sig_atomic_t reset_received;

void CU(void);
int ALU(int command, int operand);
void IRC(int signum);
