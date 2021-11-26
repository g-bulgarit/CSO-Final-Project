#include "MIPS.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//
// Created by Itay Zilka on 17/11/2021.
//

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
}