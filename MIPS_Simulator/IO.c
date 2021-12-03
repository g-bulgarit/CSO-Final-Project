#include "IO.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Set all hardware IO registers to zero
unsigned int hw_reg[HW_REGISTER_AMOUNT] = { 0 };

// Deal with hard drive
int hardDrive[SECTOR_COUNT][SECTOR_SIZE] = { 0 };

// Define variables for handling IRQ2
int* IRQ2EnableCycles;
int IRQ2EnableCyclesLength;


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

	// Store in array.
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

	UpdateIRQ2(cycle); // Check to see if IRQ2 needs to be triggered.
	// Also check timer interrupt here.

	int irq = (hw_reg[IRQ0ENABLE] & hw_reg[IRQ0STATUS]) |
			  (hw_reg[IRQ1ENABLE] & hw_reg[IRQ1STATUS]) |
			  (hw_reg[IRQ2ENABLE] & hw_reg[IRQ2STATUS]);

	//int is_currently_handling = hw_reg[IRQ0STATUS] | hw_reg[IRQ1STATUS] | hw_reg[IRQ2STATUS];
	//if (is_currently_handling) return;
	// TODO: This is a bug, fix, because it will always be true and we will never jump to IRQHANDLER.
	// Maybe we need another variable.

	// handle interrupt
	hw_reg[IRQRETURN] = (*pc);
	(*pc) = hw_reg[IRQHANDLER] & 0xFFF; // 12 bits only - make sure we don't jump to junk.
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
	ReadMemoryFile(diskinFile, hardDrive);
}

void WriteDiskToFile() {
	// TODO
}