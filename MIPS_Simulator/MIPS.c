#include "MIPS.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "IO.h"

//
// Created by Itay Zilka on 17/11/2021.
//

#define LOW_BITS_MASK 0x00000FFF

char* registerTrace(int* mips, int pc, char* instruction) {
	// Function to generate a line with the status of all registers
	// PC INST R0 R1 R2 R3 R4 R5 R6 R7 R8 R9 R10 R11 R12 R13 R14 R15
	// These can be used for debugging, but should also be printed to an output file trace.txt
	char fullLine[500];
	sprintf(fullLine, "%03d %s %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x",
		pc, instruction,
		mips[0], mips[1], mips[2], mips[3], mips[4], mips[5], mips[6], mips[7],
		mips[8], mips[9], mips[10], mips[11], mips[12], mips[13], mips[14], mips[15]);

	#ifdef DEBUG
	printf("%s\n", fullLine);
	#endif

	return fullLine;
}

// 0
void add(int* mips, int rd, int rs, int rt, int rm, int* pc){
	mips[rd] = mips[rs] + mips[rt] + mips[rm];
	(*pc)++;
}

// 1
void sub(int* mips, int rd, int rs, int rt, int rm, int* pc){
	mips[rd] = mips[rs] - mips[rt] - mips[rm];
	(*pc)++;
}

// 2
void mac(int* mips, int rd, int rs, int rt, int rm, int* pc){
	mips[rd] = mips[rs] * mips[rt] + mips[rm];
	(*pc)++;
}

// 3
void and(int* mips, int rd, int rs, int rt, int rm, int* pc){
	mips[rd] = mips[rs] & mips[rt] & mips[rm];
	(*pc)++;
}

// 4
void or(int* mips, int rd, int rs, int rt, int rm, int* pc) {
	mips[rd] = mips[rs] | mips[rt] | mips[rm];
	(*pc)++;
}

// 5
void xor(int* mips, int rd, int rs, int rt, int rm, int* pc) {
	mips[rd] = mips[rs] ^ mips[rt] ^ mips[rm];
	(*pc)++;
}

// 6 NOT TESTED
void sll(int* mips, int rd, int rs, int rt, int rm, int* pc) {
	mips[rd] = mips[rs] << mips[rt];
	(*pc)++;
}

// 7 NOT TESTED | Arithmatic shift with sign extension
void sra(int* mips, int rd, int rs, int rt, int rm, int* pc) {
	// Signed shift is arithmetic
	mips[rd] = mips[rs] >> mips[rt];
	(*pc)++;
}

//8 Not Test | Logical shift
void srl(int* mips, int rd, int rs, int rt, int rm, int* pc) {
	// Get unsigned Shift right for logical shift to create
	// a mask for an arithmetic shift
	int mask = 0xFFFFFFFF >> mips[rt];

	// Shift right rs by rt, mask for logical shift
	mips[rd] = (mips[rs] >> mips[rt]) & mask;

	(*pc)++;
}

// 9 NOT TESTED
void beq(int* mips, int rd, int rs, int rt, int rm, int* pc) {
	if (mips[rs] == mips[rt]) {
		(*pc) = mips[rm] & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}


//10 NOT TESTED
void bne(int* mips, int rd, int rs, int rt, int rm, int* pc) {
	if (mips[rs] != mips[rt]) {
		(*pc) = mips[rm] & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

//11 NOT TESTED
void blt(int* mips, int rd, int rs, int rt, int rm, int* pc) {
	if (mips[rs] < mips[rt]) {
		(*pc) = mips[rm] & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

//12 NOT TESTED
void bgt(int* mips, int rd, int rs, int rt, int rm, int* pc) {
	if (mips[rs] > mips[rt]) {
		(*pc) = mips[rm] & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

//13 NOT TESTED
void ble(int* mips, int rd, int rs, int rt, int rm, int* pc) {
	if (mips[rs] <= mips[rt]) {
		(*pc) = mips[rm] & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

//14 NOT TESTED
void bge(int* mips, int rd, int rs, int rt, int rm, int* pc) {
	if (mips[rs] >= mips[rt]) {
		(*pc) = mips[rm] & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

void jal(int* mips, int rd, int rs, int rt, int rm, int* pc) {
	mips[rd] = (*pc) + 1;
	(*pc) = mips[rm] & LOW_BITS_MASK;
}

// 16 NOT TESTED
void lw(int* mips,int* memory, int rd, int rs, int rt, int rm, int* pc) 
{
	mips[rd] = memory[mips[rs] + mips[rt] + mips[rm]];
	(*pc)++;
}

// 17 NOT TESTED
void sw(int* mips, int* memory, int rd, int rs, int rt, int rm, int* pc)
{
	memory[mips[rs] + mips[rt]] = mips[rm] + mips[rd];
	(*pc)++;
}

// 18 NOT TESTED
void reti(int* IO, int* pc)
{
	(*pc) = IO[IRQRETURN];
	(*pc)++;
}

// 19 NOT TESTED
void in(int* mips,int* IORegs, int rd, int rs, int rt, int* pc) {
	mips[rd] = IORegs[mips[rs] + mips[rt]];
	(*pc)++;
}

// 20 NOT TESTED
void out(int* mips, int* IORegs, int rs, int rt, int rm, int* pc) {
	IORegs[mips[rs] + mips[rt]] = mips[rm];
	(*pc)++;
}

// 21 NOT TESTED
// ShutdownMIPS(mips, commands, memory, pc)
void ShutdownMIPS(int* mips, Command** commands, int* memoryDump, char** TraceArray, int TraceArrayLength, int pc) {
	// Handle writing all output files and free allocated memory.

	// Write trace array to file
	DumpRegisterTraceToFile(TraceArray, TraceArrayLength);
	// TODO:
	// Write other things...
	exit(0);
}