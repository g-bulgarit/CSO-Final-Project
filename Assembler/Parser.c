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
	return 'f';
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
	char rd[LINE_LENGTH];
	char rs[LINE_LENGTH];
	char rt[LINE_LENGTH];
	char rm[LINE_LENGTH];
	char imm1[LINE_LENGTH];
	char imm2[LINE_LENGTH];

	// Read line into containers, ignore comments including the # sign
	sscanf(line, "%s %[^,] , %[^,] , %[^,] , %[^,] , %[^,] , %[^,#]", command, rd, rs, rt, rm, imm1, imm2);

	#ifdef DEBUG
	printf("Input:\nOpcode: %s, RD: %s, RS: %s, RT: %s, RM: %s, IMM1:%s, IMM2:%s\n", command, rd, rs, rt, rm, imm1, imm2);
	#endif
	
	// Parse individual values with lookup tables
	char opcode = GetCommandOpcode_(command);
	printf("%x", opcode);

	return;
}