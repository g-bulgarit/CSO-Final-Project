#include "MIPS.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "IO.h"

//
// Created by Itay Zilka on 17/11/2021.
//

#define LOW_BITS_MASK 0x00000FFF

char** commitRegisterTrace(int* mips, int pc, char* hexInstruction, char** TraceArray, int* TraceArrayLength) {
	// Function to generate a line with the status of all registers
	// PC INST R0 R1 R2 R3 R4 R5 R6 R7 R8 R9 R10 R11 R12 R13 R14 R15
	char* fullLine = (char*)malloc(sizeof(char) * 200);
	sprintf(fullLine, "%03d %s %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x \0",
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

void DumpRegisterTraceToFile(char** TraceArray, int TraceArrayLength) {
	// Dump traces to file.
	FILE* wfp;
	wfp = fopen("trace.txt", "w+");

	for (int i = 0; i < TraceArrayLength; i++)
	{
		fprintf(wfp, "%s\n", TraceArray[i]);  // Print to file with a newline
	}
	fclose(wfp);
}

void HandleIMM(int* mips, int* rs, int* rt, int* rm, int imm1, int imm2) {
	// If RD, RS, RT, or RM contain a reference to either $IMM1 or $IMM2,
	// we need to place the value in $IMM_X into the 'register' part of the command.
	// We can check that $IMM_X is not zero to make sure.

	if (*rs == IMM1 && imm1 != 0) { *rs = imm1; }
	else if (*rs == IMM2 && imm2 != 0) { *rs = imm2; }
	else { *rs = mips[*rs]; }

	if (*rt == IMM1 && imm1 != 0) { *rt = imm1; }
	else if (*rt == IMM2 && imm2 != 0) { *rt = imm2; }
	else { *rt = mips[*rt]; }

	if (*rm == IMM1 && imm1 != 0) { *rm = imm1; }
	else if (*rm == IMM2 && imm2 != 0) { *rm = imm2; }
	else { *rm = mips[*rm]; }
}

// 0 - TESTED
void add(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc){
	int final_rs = rs;
	int final_rt = rt;
	int final_rm = rm;

	HandleIMM(mips, &final_rs, &final_rt, &final_rm, imm1, imm2);
	mips[rd] = final_rs + final_rt + final_rm;

	(*pc)++;
}

// 1
void sub(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc){
	int final_rs = rs;
	int final_rt = rt;
	int final_rm = rm;

	HandleIMM(mips, &final_rs, &final_rt, &final_rm, imm1, imm2);
	mips[rd] = final_rs - final_rt - final_rm;

	(*pc)++;
}

// 2
void mac(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc){
	int final_rs = rs;
	int final_rt = rt;
	int final_rm = rm;

	HandleIMM(mips, &final_rs, &final_rt, &final_rm, imm1, imm2);
	mips[rd] = final_rs * final_rt * final_rm;

	(*pc)++;
}

// 3
void and(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc){
	int final_rs = rs;
	int final_rt = rt;
	int final_rm = rm;

	HandleIMM(mips, &final_rs, &final_rt, &final_rm, imm1, imm2);
	mips[rd] = final_rs & final_rt & final_rm;

	(*pc)++;
}

// 4
void or(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = rs;
	int final_rt = rt;
	int final_rm = rm;

	HandleIMM(mips, &final_rs, &final_rt, &final_rm, imm1, imm2);
	mips[rd] = final_rs | final_rt | final_rm;

	(*pc)++;
}

// 5
void xor(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = rs;
	int final_rt = rt;
	int final_rm = rm;

	HandleIMM(mips, &final_rs, &final_rt, &final_rm, imm1, imm2);
	mips[rd] = final_rs ^ final_rt ^ final_rm;

	(*pc)++;
}

// 6 NOT TESTED
void sll(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	int final_rs = rs;
	int final_rt = rt;
	int final_rm = rm;

	HandleIMM(mips, &final_rs, &final_rt, &final_rm, imm1, imm2);
	mips[rd] = mips[rs] << mips[rt];
	(*pc)++;
}

// 7 NOT TESTED | Arithmatic shift with sign extension
void sra(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	// Signed shift is arithmetic
	mips[rd] = mips[rs] >> mips[rt];
	(*pc)++;
}

//8 Not Test | Logical shift
void srl(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	// Get unsigned Shift right for logical shift to create
	// a mask for an arithmetic shift
	int mask = 0xFFFFFFFF >> mips[rt];

	// Shift right rs by rt, mask for logical shift
	mips[rd] = (mips[rs] >> mips[rt]) & mask;

	(*pc)++;
}

// 9 NOT TESTED
void beq(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	if (mips[rs] == mips[rt]) {
		(*pc) = mips[rm] & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}


//10 NOT TESTED
void bne(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	if (mips[rs] != mips[rt]) {
		(*pc) = mips[rm] & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

//11 NOT TESTED
void blt(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	if (mips[rs] < mips[rt]) {
		(*pc) = mips[rm] & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

//12 NOT TESTED
void bgt(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	if (mips[rs] > mips[rt]) {
		(*pc) = mips[rm] & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

//13 NOT TESTED
void ble(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	if (mips[rs] <= mips[rt]) {
		(*pc) = mips[rm] & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

//14 NOT TESTED
void bge(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	if (mips[rs] >= mips[rt]) {
		(*pc) = mips[rm] & LOW_BITS_MASK;
		return;
	}

	(*pc)++;
	return;
}

void jal(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc) {
	mips[rd] = (*pc) + 1;
	(*pc) = mips[rm] & LOW_BITS_MASK;
}

// 16 NOT TESTED
void lw(int* mips,int* memory, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc)
{
	mips[rd] = memory[mips[rs] + mips[rt] + mips[rm]];
	(*pc)++;
}

// 17 NOT TESTED
void sw(int* mips, int* memory, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc)
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