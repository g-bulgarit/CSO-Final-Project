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
} CommandOpcodes;

// Full opcode LUT
CommandOpcodes Opcode_LUT[] = {
		{"add", 0},
		{"sub", 1},
		{"mac", 2},
		{"and", 3},
		{"or", 4},
		{"xor", 5},
		{"sll", 6},
		{"sra", 7},
		{"srl", 8},
		{"beq", 9},
		{"bne", 10},
		{"blt", 11},
		{"bgt", 12},
		{"ble", 13},
		{"bge", 14},
		{"jal", 15},
		{"lw", 16},
		{"sw", 17},
		{"reti", 18},
		{"in", 19},
		{"out", 20},
		{"halt", 21}
};