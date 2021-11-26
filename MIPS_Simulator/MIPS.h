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

void add(int* mips, int rd, int rs, int rt, int rm);
void sub(int* mips, int rd, int rs, int rt, int rm);
void mac(int* mips, int rd, int rs, int rt, int rm);
void and(int* mips, int rd, int rs, int rt, int rm);
void or(int* mips, int rd, int rs, int rt, int rm);
void xor(int* mips, int rd, int rs, int rt, int rm);

char* registerTrace(int* mips, int pc, char* instruction);

#endif //MIPS_SIMULATOR_MIPS_H
