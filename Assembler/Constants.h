#pragma once

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#define LINE_LENGTH 500

// Comment this out to get no debug prints :)
#define DEBUG

// Define struct for looking up opcode char values based on string
typedef struct s_CommandOpcodes {
	const char* command_name;
	const char value;
	const int is_jump_command;
} CommandOpcodes;

typedef struct s_Label {
	const char* tag;
	const int* targetAdress;
} Label;

typedef struct s_CommandLine {
	const CommandOpcodes* command;
	const char* commandText;
	const int* adress;
} CommandLine;

// Full opcode LUT
CommandOpcodes Opcode_LUT[] = {
		{"add", 0,0},
		{"sub", 1,0},
		{"mac", 2,0},
		{"and", 3,0},
		{"or", 4,0},
		{"xor", 5,0},
		{"sll", 6,0},
		{"sra", 7,0},
		{"srl", 8,0},
		{"beq", 9,1},
		{"bne", 10,1},
		{"blt", 11,1},
		{"bgt", 12,1},
		{"ble", 13,1},
		{"bge", 14,1},
		{"jal", 15,1},
		{"lw", 16,0},
		{"sw", 17,0},
		{"reti", 18,0},
		{"in", 19,0},
		{"out", 20,0},
		{"halt", 21,0}
};

typedef struct s_RegisterCodes {
	const char* register_name;
	const char value;
} RegisterCodes;

RegisterCodes Register_LUT[] = {
		{"$zero", 0},
		{"$imm1", 1},
		{"$imm2", 2},
		{"$v0", 3},
		{"$a0", 4},
		{"$a1", 5},
		{"$a2", 6},
		{"$t0", 7},
		{"$t1", 8},
		{"$t2", 9},
		{"$s0", 10},
		{"$s1", 11},
		{"$s2", 12},
		{"$gp", 13},
		{"$sp", 14},
		{"$ra", 15},
};