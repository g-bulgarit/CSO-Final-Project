//
// Created by Itay Zilka on 17/11/2021.
//

#ifndef MIPS_SIMULATOR_MIPS_H
#define MIPS_SIMULATOR_MIPS_H
#define _CRT_SECURE_NO_WARNINGS

#define DEBUG

typedef struct s_Command {
	int opcode;
	int rd;
	int rs;
	int rt;
	int rm;
	int imm1;
	int imm2;
} Command;

void add(int* mips, int rd, int rs, int rt, int rm, int* pc);
void sub(int* mips, int rd, int rs, int rt, int rm, int* pc);
void mac(int* mips, int rd, int rs, int rt, int rm, int* pc);
void and(int* mips, int rd, int rs, int rt, int rm, int* pc);
void or(int* mips, int rd, int rs, int rt, int rm, int* pc);
void xor(int* mips, int rd, int rs, int rt, int rm, int* pc);

void lw(int* mips, int* memory, int rd, int rs, int rt, int rm, int* pc);
void sw(int* mips, int* memory, int rd, int rs, int rt, int rm, int* pc);

char* registerTrace(int* mips, int pc, char* instruction);

#endif //MIPS_SIMULATOR_MIPS_H
