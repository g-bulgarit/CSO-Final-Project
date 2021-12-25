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

#define MONITOROFFSET 20
#define MONITORDATA 21
#define MONITORCMD 22

void Interrupt(int* pc, unsigned long long cycle);
void HandleMonitor();
void incrementHWClock();
void LogLedState(unsigned long long cycle);
void Log7SegmentValue(unsigned long long cycle);
void DumpLedArrayToFile(char** LedArray, int LedArrayLength, char* fileName);
void InitializeIRQ2Cycles(char* filePath);
void WriteLedArrayToFile(char* fileName);
void DumpHardDrive(char* fileName);
void retiIO(int* pc);
void inIO(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc, unsigned long long cycle);
void outIO(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc, unsigned long long cycle);
void WriteMonitorOutputFiles(char* txtFileName, char* yuvFileName);
void Write7SegmentArrayToFile(char* fileName);
