#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Parser.h"
#include "Constants.h"

char GetCommandOpcode_(char opcode[]) {
	for (CommandOpcodes *p = (CommandOpcodes *)Opcode_LUT; p->command_name != NULL; ++p) {
		if (!strcmp(p->command_name, opcode)) {
			return p->value;
		}
	}
	return 255;
}

char GetRegisterByte_(char register_number[]) {
	for (RegisterCodes *p = (RegisterCodes *)Register_LUT; p->register_name != NULL; ++p) {
		if (!strcmp(p->register_name, register_number)) {
			return p->value;
		}
	}
	return 255;
}

char FormatAsBits_(char opcode, char rd, char rs, char rt, char rm, char imm1_in, char imm2_in) {
	// Map:
	//   8       ----8---        ----8----       ----24---
	// 47:40   39:36   35:32   31:28   27:24   23:12   11:0
	//opcode     rd      rs      rt      rm     imm1    imm2

	char output[49];
	// TODO: Format the line here!
	output[48] = "\n";
	return output;
}

void ParseSingleLine(char *line) {
	// Takes in a line from the ASM code,
	// Returns a full instruction struct, ignoring comments.
	// ASM line structure:
	//
	// opcode rd, rs, rt, rm, imm1, imm2 # comments \n
	// add     $t3, $t2, $t1, $t0, 0, 0

	// Define string container variables
	char command[LINE_LENGTH];
	char rd_in[LINE_LENGTH];
	char rs_in[LINE_LENGTH];
	char rt_in[LINE_LENGTH];
	char rm_in[LINE_LENGTH];
	char imm1_in[LINE_LENGTH];
	char imm2_in[LINE_LENGTH];

	// Read line into containers, ignore comments including the # sign
	sscanf(line, "%s %[^,] , %[^,] , %[^,] , %[^,] , %[^,] , %[^,#]", command, rd_in, rs_in, rt_in, rm_in, imm1_in, imm2_in);

	#ifdef DEBUG
	printf("Input:\nOpcode: %s, RD: %s, RS: %s, RT: %s, RM: %s, IMM1:%s, IMM2:%s\n", command, rd_in, rs_in, rt_in, rm_in, imm1_in, imm2_in);
	#endif
	
	// Parse individual values with lookup tables
	char opcode = GetCommandOpcode_(command);
	char rd = GetRegisterByte_(rd_in);
	char rs = GetRegisterByte_(rs_in);
	char rt = GetRegisterByte_(rt_in);
	char rm = GetRegisterByte_(rm_in);

	char MIPSInstruction = FormatAsBits_(opcode, rd, rs, rt, rm, imm1_in, imm2_in);
	printf("%x %x %x %x %x", opcode, rd, rs, rt, rm);

	return;
}