#include "IO.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MIPS.h"
#include "monitor.h"

// Deal with hard drive
int hardDrive[SECTOR_COUNT][SECTOR_SIZE] = { 0 };

unsigned int hw_reg[HW_REGISTER_AMOUNT] = { 0 };

// Define variables for handling IRQ2
int* IRQ2EnableCycles;
int IRQ2EnableCyclesLength;
int isCurrentlyHandlingInterupt = 0;

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

void UpdateIRQ2(int cycle) {
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


void Interrupt(int* pc, int cycle) {
	// Function that checks if MIPS needs to stop what it's doing and
	// handle an interrupt instead.
	// To be called **every** clock cycle.

	if (isCurrentlyHandlingInterupt) return;

	UpdateIRQ2(cycle); // Check to see if IRQ2 needs to be triggered.
	// Also check timer interrupt here.

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
	unsigned int shouldWriteToMonitor = hw_reg[MONITORCMD];

	if (shouldWriteToMonitor) {
		WriteToMonitor();
	}

	hw_reg[MONITORCMD] = 0;
}

void ReadSector(int* mem, int* cycle) {
	// Function to read a disk sector to memory,
	// Also increment current cycle to simulate a that it takes a long while to execute disk commands.

	int TargetSector = hw_reg[DISKSECTOR];
	int TargetBuffer = hw_reg[DISKBUFFER]; // offset

	for (int i = 0; i < SECTOR_SIZE; i++) {
		mem[TargetBuffer + i] = hardDrive[TargetSector][i];
	}
	// Waste clock time
	*(cycle) += DISK_CYCLE_USAGE;
}

void WriteSector(int* mem, int* cycle) {
	// Function to write a disk sector from data in memory,
	// Also increment current cycle to simulate a that it takes a long while to execute disk commands.

	int TargetSector = hw_reg[DISKSECTOR];
	int TargetBuffer = hw_reg[DISKBUFFER]; // offset

	for (int i = 0; i < SECTOR_SIZE; i++) {
		hardDrive[TargetSector][i] = mem[TargetBuffer + i];
	}
	// Waste clock time
	*(cycle) += DISK_CYCLE_USAGE;
}

void ReadDiskFromFile(char* diskinFile) {
	// Read memory frmo dmemin.txt file into a preallocated array of 4096 zeros.
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
}

void WriteDiskToFile() {
	// TODO
}

void retiIO(int* pc) {
	reti(hw_reg, pc);
	isCurrentlyHandlingInterupt = 0;
}

void inIO(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	in(mips, hw_reg, rd, rs, rt, rm, imm1, imm2, pc);
}
void outIO(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	out(mips, hw_reg, rd, rs, rt, rm, imm1, imm2, pc);
}