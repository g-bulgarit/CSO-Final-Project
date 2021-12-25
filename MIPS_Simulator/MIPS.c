#include "MIPS.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "IO.h"

//
// Created by Itay Zilka on 17/11/2021.
//

#define LOW_BITS_MASK 0x00000FFF

char IORegisterNames[][50] = {
	"irq0enable",
	"irq1enable",
	"irq2enable",
	"irq0status",
	"irq1status",
	"irq2status",
	"irqhandler",
	"irqreturn",
	"clks",
	"leds",
	"display7seg",
	"timerenable",
	"timercurrent",
	"timermax",
	"diskcmd",
	"disksector",
	"diskbuffer",
	"diskstatus",
	"reserved",
	"reserved",
	"monitoraddr",
	"monitordata",
	"monitorcmd"
};


char** hardwareRegOutArray = NULL;
int hardwareRegOutArrayLength = 0;

void addHardwareRegTraceLine(int read, int cycle, int regIdx, int data) {
	// Function to add a new HW REGOUT line to the array

	// Allocate an array if one does not exist yet
	if (hardwareRegOutArray == NULL) {
		hardwareRegOutArray = (char**)malloc(sizeof(char*));
	}
	// Allocate space for a line
	char* line = (char*)malloc(sizeof(char*) * LINE_LENGTH);
	char* regName = IORegisterNames[regIdx];

	// if read == 1, it's a read operation, else it's a write operation.
	if (read == 1) {
		sprintf(line, "%d READ %s %08X", cycle, regName, data);
	}
	else {
		sprintf(line, "%d WRITE %s %08X", cycle, regName, data);
	}

	hardwareRegOutArray[hardwareRegOutArrayLength] = line;
	hardwareRegOutArrayLength++;
	hardwareRegOutArray = (char**)realloc(hardwareRegOutArray, sizeof(char*) * (hardwareRegOutArrayLength + 1));
}

void DumpHardwareRegisterTraceToFile(char** hardwareRegOutArray, int hardwareRegOutArrayLength, char* fileName) {
	// Dump hardware register traces to file.
	FILE* wfp;
	wfp = fopen(fileName, "w+");

	for (int i = 0; i < hardwareRegOutArrayLength; i++)
	{
		fprintf(wfp, "%s\n", hardwareRegOutArray[i]);  // Print to file with a newline
	}
	fclose(wfp);
}

char** commitRegisterTrace(int* mips, int pc, char* hexInstruction, char** TraceArray, int* TraceArrayLength) {
	// Function to generate a line with the status of all registers
	// PC INST R0 R1 R2 R3 R4 R5 R6 R7 R8 R9 R10 R11 R12 R13 R14 R15
	char* fullLine = (char*)malloc(sizeof(char) * 200);
	sprintf(fullLine, "%03X %s %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\0",
		pc, hexInstruction,
		mips[0], mips[1], mips[2], mips[3], mips[4], mips[5], mips[6], mips[7],
		mips[8], mips[9], mips[10], mips[11], mips[12], mips[13], mips[14], mips[15]);

	int stringLength = strlen(fullLine);
	fullLine = (char*)realloc(fullLine, sizeof(char)*stringLength + 1);

	#ifdef DEBUG
	printf("%s\n", fullLine);
	#endif

 	TraceArray = (char**)realloc(TraceArray, sizeof(char*) * (*TraceArrayLength + 1));
	TraceArray[(*TraceArrayLength)] = fullLine;
	*TraceArrayLength += 1;

	return TraceArray;
}

void DumpRegisterTraceToFile(char** TraceArray, int TraceArrayLength, char* fileName) {
	// Dump traces to file.
	FILE* wfp;
	wfp = fopen(fileName, "w+");

	for (int i = 0; i < TraceArrayLength; i++)
	{
		fprintf(wfp, "%s\n", TraceArray[i]);  // Print to file with a newline
	}
	fclose(wfp);
}

void DumpRegisterState(int mipsRegisters[], int registerAmount, char* fileName) {
	// Output current state of registers to a file, separated by newlines.
	// Skip the first three registers (R0, R1, R2) as they are constant.

	FILE* wfp;
	wfp = fopen(fileName, "w+");

	for (int i = 3; i < registerAmount; i++)
	{
		fprintf(wfp, "%08X\n", mipsRegisters[i]);
	}
	fclose(wfp);
}

void DumpMemory(int mem[], int memoryBlockLength, char* fileName) {
	FILE* wfp;
	wfp = fopen(fileName, "w+");

	for (int i = 0; i < memoryBlockLength; i++)
	{
		fprintf(wfp, "%08X\n", mem[i]);
	}
	fclose(wfp);
}

void DumpCycles(int cycles, char* fileName) {
	FILE* wfp;
	wfp = fopen(fileName, "w+");

	fprintf(wfp, "%d", cycles);

	fclose(wfp);
}

// 0 - TESTED
void add(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc){
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	mips[rd] = final_rs + final_rt + final_rm;

	(*pc)++;
}

// 1
void sub(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc){
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	mips[rd] = final_rs - final_rt - final_rm;

	(*pc)++;
}

// 2
void mac(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc){
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	mips[rd] = final_rs * final_rt * final_rm;

	(*pc)++;
}

// 3
void and(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc){
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	mips[rd] = final_rs & final_rt & final_rm;

	(*pc)++;
}

// 4
void or(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	mips[rd] = final_rs | final_rt | final_rm;

	(*pc)++;
}

// 5
void xor(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	mips[rd] = final_rs ^ final_rt ^ final_rm;

	(*pc)++;
}

// 6 NOT TESTED
void sll(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	mips[rd] = final_rs << final_rt;

	(*pc)++;
}

// 7 NOT TESTED | Arithmatic shift with sign extension
void sra(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	// Signed shift is arithmetic
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	mips[rd] = final_rs >> final_rt;

	(*pc)++;
}

//8 Not Test | Logical shift
void srl(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	// Get unsigned Shift right for logical shift to create
	// a mask for an arithmetic shift
	int mask = 0xFFFFFFFF >> final_rt;

	// Shift right rs by rt, mask for logical shift
	mips[rd] = (final_rs >> final_rt) & mask;

	(*pc)++;
}

// 9 NOT TESTED
void beq(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	if (final_rs == final_rt) {
		(*pc) = final_rm & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}


//10 NOT TESTED
void bne(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	if (final_rs != final_rt) {
		(*pc) = final_rm & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

//11 NOT TESTED
void blt(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	if (final_rs < final_rt) {
		(*pc) = final_rm & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

//12 NOT TESTED
void bgt(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	if (final_rs > final_rt) {
		(*pc) = final_rm & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

//13 NOT TESTED
void ble(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	if (final_rs <= final_rt) {
		(*pc) = final_rm & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

//14 NOT TESTED
void bge(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	if (final_rs >= final_rt) {
		(*pc) = final_rm & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

void jal(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	mips[rd] = (*pc) + 1;
	(*pc) = final_rm & LOW_BITS_MASK;
}

// 16 NOT TESTED
void lw(int* mips,int* memory, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc)
{
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	mips[rd] = memory[final_rs + final_rt] + final_rm;
	(*pc)++;
}

// 17 NOT TESTED
void sw(int* mips, int* memory, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc)
{
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];

	memory[final_rs + final_rt] = final_rm + mips[rd];
	(*pc)++;
}

// 18 NOT TESTED
void reti(unsigned int * IO, int* pc)
{
	(*pc) = IO[IRQRETURN];
}

// 19 NOT TESTED
void in(int* mips, unsigned int* IORegs, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc, unsigned long long cycle) {
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];
	int targetRegister = final_rs + final_rt;
	mips[rd] = IORegs[targetRegister];

	addHardwareRegTraceLine(1, cycle, targetRegister, mips[rd]);
	(*pc)++;
}

// 20 NOT TESTED
void out(int* mips, unsigned int* IORegs, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc, unsigned long long cycle) {
	int final_rs = mips[rs];
	int final_rt = mips[rt];
	int final_rm = mips[rm];
	int targetRegister = final_rs + final_rt;

	IORegs[targetRegister] = final_rm;
	addHardwareRegTraceLine(0, cycle, targetRegister, final_rm);

	(*pc)++;
}

// 21 NOT TESTED
// ShutdownMIPS(mips, commands, memory, pc)
void ShutdownMIPS(int* mips, unsigned long long cycles, Command** commands, int* memoryDump, char** TraceArray, int TraceArrayLength, int pc, char* argv[]) {
	// Handle writing all output files and free allocated memory.

	// Write trace array to file
	WriteLedArrayToFile(argv[LEDS]);
	Write7SegmentArrayToFile(argv[DISPLAY7SEG]);
	DumpRegisterTraceToFile(TraceArray, TraceArrayLength, argv[TRACE]);
	WriteMonitorOutputFiles(argv[MONITOR_TXT], argv[MONITOR_YUV]);
	DumpRegisterState(mips, REGISTER_AMOUNT, argv[REGOUT]);
	DumpMemory(memoryDump, MEM_SIZE, argv[DMEMOUT]);
	DumpHardDrive(argv[DISKOUT]);
	DumpCycles(cycles, argv[CYCLES]);
	DumpHardwareRegisterTraceToFile(hardwareRegOutArray, hardwareRegOutArrayLength, argv[HWREGTRACE]);
	// TODO:
	// Write other things...
	exit(0);
}
