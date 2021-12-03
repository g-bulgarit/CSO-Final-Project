#pragma once
#include "Constants.h"

#define DISK_CYCLE_USAGE 1024
#define SECTOR_COUNT 128
#define SECTOR_SIZE 16

#define IRQ0ENABLE 0
#define IRQ1ENABLE 1
#define IRQ2ENABLE 2
#define IRQ0STATUS 3
#define IRQ1STATUS 4
#define IRQ2STATUS 5
#define IRQHANDLER 6
#define IRQRETURN 7


#define DISKCMD 14
#define DISKSECTOR 15
#define DISKBUFFER 16
#define DISKSTATUS 17

void Interrupt(int* pc, int cycle);
void InitializeIRQ2Cycles(char* filePath);

void retiIO(int* pc);
void inIO(int* mips, int rd, int rs, int rt, int* pc);
void outIO(int* mips, int rs, int rt, int rm, int* pc);
