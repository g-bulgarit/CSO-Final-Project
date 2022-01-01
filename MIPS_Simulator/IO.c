#include "IO.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "MIPS.h"
#include "monitor.h"
#include "Constants.h"

// Deal with hard drive
int hardDrive[SECTOR_COUNT][SECTOR_SIZE] = { 0 };
unsigned int hw_reg[HW_REGISTER_AMOUNT] = { 0 };

// Define led state variables
unsigned int oldLedState = 0;
char** ledStateArray = NULL;
int ledStateArrayLength = 0;

// Define 7-segment display state variables
unsigned int old7SegmentValue = 0;
char** sevenSegmentValueArray = NULL;
int sevenSegmentValueArrayLength = 0;

void incrementTimer() {
	// If timer enable
	if (hw_reg[TIMERENABLE] == 1) {
		// If timer
		if (hw_reg[TIMERCURRRENT] < hw_reg[TIMERMAX]) {
			hw_reg[TIMERCURRRENT]++;
		}
		else {
			hw_reg[TIMERCURRRENT] = 0;
			hw_reg[IRQ0STATUS] = 1;
		}
	}
}
void incrementHWClock() {
	// If we're overflowing on this cycle - reset
	if (hw_reg[CLKS] == INT_MAX) hw_reg[CLKS] = 0;
	// then, increment clock.
	hw_reg[CLKS]++;
}

void DumpHardDrive(char* fileName) {
	// Dump content of hard drive to a file.
	FILE* wfp;
	wfp = fopen(fileName, "w+");
	int totalHardDriveSize = SECTOR_COUNT * SECTOR_SIZE;

	for (int i = 0; i < totalHardDriveSize; i++)
	{
		fprintf(wfp, "%08X\n", hardDrive[i / SECTOR_SIZE][i % SECTOR_SIZE]);
	}
	fclose(wfp);
}

void WriteLedStateToArray(int cycle) {
	char* line = (char*) malloc(sizeof(char)*500);
	if (line == NULL) {
		exit(-1);
	}
	if (ledStateArray == NULL) {
		ledStateArray = (char**) malloc(sizeof(char*));
	}
	else {
		ledStateArray = (char**) realloc(ledStateArray, sizeof(char*) * (ledStateArrayLength + 1));
	}
	sprintf(line, "%d %08x", cycle, hw_reg[9]);
	ledStateArray[ledStateArrayLength] = line;
	ledStateArrayLength ++;
}

void Write7SegmentValueToArray(int cycle) {
	char* line = (char*)malloc(sizeof(char) * 500);
	if (line == NULL) {
		exit(-1);
	}
	if (sevenSegmentValueArray == NULL) {
		sevenSegmentValueArray = (char**)malloc(sizeof(char*));
	}
	else {
		sevenSegmentValueArray = (char**)realloc(sevenSegmentValueArray, sizeof(char*) * (sevenSegmentValueArrayLength + 1));
	}
	sprintf(line, "%d %08x", cycle, hw_reg[10]);
	sevenSegmentValueArray[sevenSegmentValueArrayLength] = line;
	sevenSegmentValueArrayLength++;
}

// Define variables for handling IRQ2
int* IRQ2EnableCycles;
int IRQ2EnableCyclesLength;
int isCurrentlyHandlingInterupt = 0;

void LogLedState(unsigned long long cycle) {
	unsigned int currentLedState = hw_reg[9];
	if (currentLedState != oldLedState) {
		WriteLedStateToArray(cycle);
		oldLedState = currentLedState;
	}
}

void Log7SegmentValue(unsigned long long cycle) {
	unsigned int current7SegmentValue = hw_reg[10];
	if (current7SegmentValue != old7SegmentValue) {
		Write7SegmentValueToArray(cycle);
		old7SegmentValue = current7SegmentValue;
	}
}

void DumpLedArrayToFile(char** LedArray, int LedArrayLength, char* fileName) {
	FILE* wfp;
	wfp = fopen(fileName, "w+");

	for (int i = 0; i < LedArrayLength; i++)
	{
		fprintf(wfp, "%s\n", LedArray[i]);  // Print to file with a newline
	}
	fclose(wfp);
}

void Dump7SegmentArrayToFile(char** sevenSegmentArray, int arrayLength, char* fileName) {
	FILE* wfp;
	wfp = fopen(fileName, "w+");

	for (int i = 0; i < arrayLength; i++)
	{
		fprintf(wfp, "%s\n", sevenSegmentArray[i]);  // Print to file with a newline
	}
	fclose(wfp);
}

void WriteLedArrayToFile(char* fileName) {
	DumpLedArrayToFile(ledStateArray, ledStateArrayLength, fileName);
}

void Write7SegmentArrayToFile(char* fileName) {
	Dump7SegmentArrayToFile(sevenSegmentValueArray, sevenSegmentValueArrayLength, fileName);
}

void WriteToMonitor() {
	unsigned char color = hw_reg[MONITORDATA];
	unsigned int offset = hw_reg[MONITOROFFSET];

	int row = offset / SCREEN_SIZE;
	int col = offset % SCREEN_SIZE;

	updatePixel(row, col, color);
}

void InitializeIRQ2Cycles(char* filePath) {
	// Read irq2in.txt file and save cycles to interrupt at in an array.

	IRQ2EnableCycles = (int*)malloc(sizeof(int));
	// Read IRQ2in file
	FILE* rfp = fopen(filePath, "r");
	char buffer[LINE_LENGTH];

	IRQ2EnableCyclesLength = 0;

	while (fgets(buffer, LINE_LENGTH - 1, rfp))
	{
		buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

#ifdef DEBUG
		printf("IRQ2 interrupt line : %s\n", buffer);
#endif

		// Reallocate enough space
		IRQ2EnableCycles = (int*)realloc(IRQ2EnableCycles, (IRQ2EnableCyclesLength + 1) * sizeof(int));

		int interruptLine = strtoul(buffer, NULL, 0);
		IRQ2EnableCycles[IRQ2EnableCyclesLength] = interruptLine;
		IRQ2EnableCyclesLength++;
	}
}

void UpdateIRQ2(unsigned long long cycle) {
	// Check if current cycle is in IRQ2EnableCycles array
	// If so - set IRQ2STATUS = 1
	for (int i = 0; i < IRQ2EnableCyclesLength; i++)
	{
		if (cycle == IRQ2EnableCycles[i]) {
			hw_reg[IRQ2STATUS] = 1;
			break;
		}
	}
}


void Interrupt(int* pc, unsigned long long cycle) {
	// Function that checks if MIPS needs to stop what it's doing and
	// handle an interrupt instead. To be called **every** clock cycle.

	incrementTimer();
	if (isCurrentlyHandlingInterupt) return;

	UpdateIRQ2(cycle); // Check to see if IRQ2 needs to be triggered.

	int irq = (hw_reg[IRQ0ENABLE] & hw_reg[IRQ0STATUS]) |
			  (hw_reg[IRQ1ENABLE] & hw_reg[IRQ1STATUS]) |
			  (hw_reg[IRQ2ENABLE] & hw_reg[IRQ2STATUS]);

	// handle interrupt
	if (irq) {
		isCurrentlyHandlingInterupt = 1;
		hw_reg[IRQRETURN] = (*pc);
		(*pc) = hw_reg[IRQHANDLER] & 0xFFF; // 12 bits only - make sure we don't jump to junk.
	}
}

void HandleMonitor() {
	// If we need to write to monitor, do it now.
	unsigned int shouldWriteToMonitor = hw_reg[MONITORCMD];

	if (shouldWriteToMonitor) {
		WriteToMonitor();
	}
	hw_reg[MONITORCMD] = 0;
}

void ReadSector(int* mem, unsigned long long* cycle) {
	// Function to read a disk sector to memory,
	// Also increment current cycle to simulate a that it takes a long while to execute disk commands.

	int TargetSector = hw_reg[DISKSECTOR];
	int TargetBuffer = hw_reg[DISKBUFFER]; // offset

	for (int i = 0; i < SECTOR_SIZE; i++) {
		mem[TargetBuffer + i] = hardDrive[TargetSector][i];
	}

	hw_reg[DISKSTATUS] = 1;
	// Waste clock time
	*(cycle) += DISK_CYCLE_USAGE;
	hw_reg[CLKS] += DISK_CYCLE_USAGE;
	// Raise interrupt after done reading and reset HW registers.
	hw_reg[DISKCMD] = 0;
	hw_reg[DISKSTATUS] = 0;
	hw_reg[IRQ1STATUS] = 1;
}

void WriteSector(int* mem, unsigned long long* cycle) {
	// Function to write a disk sector from data in memory,
	// Also increment current cycle to simulate a that it takes a long while to execute disk commands.

	int TargetSector = hw_reg[DISKSECTOR];
	int TargetBuffer = hw_reg[DISKBUFFER]; // offset

	for (int i = 0; i < SECTOR_SIZE; i++) {
		hardDrive[TargetSector][i] = mem[TargetBuffer + i];
	}

	hw_reg[DISKSTATUS] = 1;
	// Waste clock time
	*(cycle) += DISK_CYCLE_USAGE;
	hw_reg[CLKS] += DISK_CYCLE_USAGE;
	// Raise interrupt after done reading and reset HW registers.
	hw_reg[DISKCMD] = 0;
	hw_reg[DISKSTATUS] = 0;
	hw_reg[IRQ1STATUS] = 1;
}

void ReadDiskFromFile(char* diskinFile) {
	// Read memory from dmemin.txt file into a preallocated array of 4096 zeros.
	FILE* rfp = fopen(diskinFile, "r");
	char buffer[LINE_LENGTH];

	int row = 0;
	int col = 0;

	while (fgets(buffer, LINE_LENGTH - 1, rfp))
	{
		buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

#ifdef DEBUG
		printf("Disk line : %s\n", buffer);
#endif

		int memoryValue = strtoul(buffer, NULL, 0);

		hardDrive[row][col] = memoryValue;

		col = (col + 1) % SECTOR_COUNT;

		if (col == 0) row = (row + 1) % SECTOR_SIZE;
	}

	fclose(rfp);
}

void WriteDiskToFile(char* diskoutFile) {
	// Output disk content to file
	FILE* wfp = fopen(diskoutFile, "w+");

	int row = 0;
	int col = 0;

	for (int row = 0; row < SECTOR_SIZE; row++)
	{
		for (int col = 0; col < SECTOR_COUNT; col++)
		{
			fprintf(wfp, "%02x\n", hardDrive[row][col]);
		}
	}
	fclose(wfp);
}

void retiIO(int* pc) {
	// When returning from interrupt, set isCurrentlyHandlingInterupt to false.
	reti(hw_reg, pc);
	isCurrentlyHandlingInterupt = 0;
}

void inIO(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc, unsigned long long cycle) {
	in(mips, hw_reg, rd, rs, rt, rm, imm1, imm2, pc, cycle);
}
void outIO(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc, unsigned long long* cycle, int* memory) {
	int targetRegister = mips[rs] + mips[rt];
	int targetValue = mips[rm];

	out(mips, hw_reg, rd, rs, rt, rm, imm1, imm2, pc, *cycle);
	// Check if there is a disk write command
	if (targetRegister == DISKCMD) {
		// Check if the operation is a read, write, or nothing
		switch (hw_reg[mips[rs] + mips[rt]])
		{
		case 1:
			// read
			ReadSector(memory, cycle);
			break;

		case 2:
			WriteSector(memory, cycle);
			break;

		default:
			break;
		}
	}
}

void WriteMonitorOutputFiles(char* txtFileName, char* yuvFileName) {
	// Call monitor.c's dump monitor function.
	DumpMonitorFiles(txtFileName, yuvFileName);
}
