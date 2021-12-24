//
// Created by Itay Zilka on 17/11/2021.
//

#ifndef MIPS_SIMULATOR_MIPS_H
#define MIPS_SIMULATOR_MIPS_H
#define _CRT_SECURE_NO_WARNINGS

#define DEBUG

typedef struct s_Command {
	char commandText[15];
	int opcode;
	int rd;
	int rs;
	int rt;
	int rm;
	int imm1;
	int imm2;
} Command;

void add(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void sub(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void mac(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void and(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void or(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void xor(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void sll(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void sra(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void srl(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void beq(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void bne(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void blt(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void bgt(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void ble(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void bge(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void jal(int* mips, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void lw(int* mips, int* memory, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void sw(int* mips, int* memory, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc);
void reti(int* IO, int* pc);
void in(int* mips, int* IORegs, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc, unsigned long long cycle);
void out(int* mips, int* IORegs, int rd, int rs, int rt, int rm, int imm1, int imm2, int* pc, unsigned long long cycle);
void ShutdownMIPS(int* mips, unsigned long long cycles, Command** commands, int* memoryDump, char** TraceArray, int TraceArrayLength, int pc, char* argv[]);

char** commitRegisterTrace(int* mips, int pc, char* hexInstruction, char** TraceArray, int* TraceArrayLength);
#endif //MIPS_SIMULATOR_MIPS_H
