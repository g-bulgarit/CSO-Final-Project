#include "IO.h"
int hardDrive[SECTOR_COUNT][SECTOR_SIZE] = { 0 };
unsigned int hw_reg[HW_REGISTER_AMOUNT] = { 0 };

void Interrupt(int* pc) {
	int irq = (hw_reg[IRQ0ENABLE] & hw_reg[IRQ0STATUS]) |			  (hw_reg[IRQ1ENABLE] & hw_reg[IRQ1STATUS]) |			  (hw_reg[IRQ2ENABLE] & hw_reg[IRQ2STATUS]);
	int is_currently_handling = hw_reg[IRQ0STATUS] | hw_reg[IRQ1STATUS] | hw_reg[IRQ2STATUS];
	if (is_currently_handling) return;

	// handle interrupt
	hw_reg[IRQRETURN] = (*pc);
	(*pc) = hw_reg[IRQHANDLER] & 0xFFF; // 12 bits only - make sure we don't jump to junk.
}

void ReadDiskFromFile() {

}

void WriteDiskToFile() {

}

void ReadSector(int* mem, int* cycle) {
	// DISKCMD is set, need to read sector to memory.
	int TargetSector = hw_reg[DISKSECTOR];
	int TargetBuffer = hw_reg[DISKBUFFER]; // offset

	for (int i = 0; i < SECTOR_SIZE; i++) {
		mem[TargetBuffer + i] = hardDrive[TargetSector][i];
	}
	// Waste clock time
	*(cycle) += DISK_CYCLE_USAGE;
}

void WriteSector(int* mem, int* cycle) {
	// DISKCMD is set, need to write memory to sector.
	int TargetSector = hw_reg[DISKSECTOR];
	int TargetBuffer = hw_reg[DISKBUFFER]; // offset

	for (int i = 0; i < SECTOR_SIZE; i++) {
		hardDrive[TargetSector][i] = mem[TargetBuffer + i];
	}
	// Waste clock time
	*(cycle) += DISK_CYCLE_USAGE;
}
