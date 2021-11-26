#include <stdio.h>
#include <stdlib.h>
#include "Constants.h"
#include "MIPS.h"

void testing(int argc, char* argv[]) {
	char* hexInstructionsFilePath = argv[1];
	char* memoryFilePath = argv[2]; // currently unused.

	// Define array to hold all MIPS registers.
	int mips_test[16] = { 0 };
	// Some tests...
	mips_test[0] = 7; // 00..0000111
	mips_test[1] = 3; // 00..0000011
	mips_test[2] = 1; // 00..0000001
	mips_test[3] = 1; // 00..0000001
	add(mips_test, 0, 1, 2, 3);
	printf("add:  %d\n", mips_test[0] == 5);
	sub(mips_test, 0, 1, 2, 3);
	printf("sub:  %d\n", mips_test[0] == 1);
	mac(mips_test, 0, 1, 2, 3);
	printf("mac:  %d\n", mips_test[0] == 4);
	and (mips_test, 0, 1, 2, 3);
	printf("and1: %d\n", mips_test[0] == 1);
	or (mips_test, 0, 1, 2, 3);
	printf("or1:  %d\n", mips_test[0] == 3);
	xor (mips_test, 0, 1, 2, 3);
	printf("xor1: %d\n", mips_test[0] == 3);

	mips_test[1] = 7;  // 00..0000111
	mips_test[2] = 5;  // 00..0000101
	mips_test[3] = 13; // 00..0001101
	and (mips_test, 0, 1, 2, 3);
	printf("and2: %d\n", mips_test[0] == 5);
	or (mips_test, 0, 1, 2, 3);
	printf("or2:  %d\n", mips_test[0] == 15);
	xor (mips_test, 0, 1, 2, 3);
	printf("xor2: %d\n", mips_test[0] == 15);

	mips_test[1] = 1;  // 00..0000001
	mips_test[2] = 2;  // 00..0000010
	mips_test[3] = 4;  // 00..0000100
	and (mips_test, 0, 1, 2, 3);
	printf("and3: %d\n", mips_test[0] == 0);
	or (mips_test, 0, 1, 2, 3);
	printf("or3:  %d\n", mips_test[0] == 7);
	xor (mips_test, 0, 1, 2, 3);
	printf("xor3: %d\n", mips_test[0] == 7);

	registerTrace(mips_test, 124, "1400201420132");
}

int partOfStringToInt(char* string,int start, int length) {
	char* part = (char*)malloc((size_t)length + 1);

	if (part == NULL) return 0;
	
	memcpy(part, &string[start], length);
	part[length] = '\0';
	int intPart = atoi(part);

	return intPart;
}

Command** AddNewCommand(char* command, Command** commands, int commandArraySize)
{
	Command* newCommand = (Command*)malloc(sizeof(Command));

	if (newCommand == NULL) return;

	newCommand->opcode = partOfStringToInt(command, 0, 2);
	newCommand->rd = partOfStringToInt(command, 2, 1);
	newCommand->rs = partOfStringToInt(command, 3, 1);
	newCommand->rt = partOfStringToInt(command, 4, 1);
	newCommand->rm = partOfStringToInt(command, 5, 1);
	newCommand->imm1 = partOfStringToInt(command, 6, 3);
	newCommand->imm2 = partOfStringToInt(command, 9, 3);

	commands = (Command**)realloc(commands, sizeof(Command*) * (commandArraySize + 1));
	commands[commandArraySize] = newCommand;

	return commands;
}

Command** ReadCommandFile(char* imemin, int* commandAmount)
{
	FILE* rfp = fopen(imemin, "r");
	char buffer[LINE_LENGTH];

	Command** commands = (Command**)malloc(sizeof(Command*));

	while (fgets(buffer, LINE_LENGTH - 1, rfp))
	{
		buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

#ifdef DEBUG
		printf("Command line : %s\n", buffer);
#endif

		commands = AddNewCommand(buffer, commands, *commandAmount);
		(*commandAmount)++;
	}

	return commands;
}

void ReadMemory(char* dmemin, int* memory) 
{
	FILE* rfp = fopen(dmemin, "r");
	char buffer[LINE_LENGTH];

	int i = 0;

	while (fgets(buffer, LINE_LENGTH - 1, rfp))
	{
		buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

#ifdef DEBUG
		printf("Memory line : %s\n", buffer);
#endif

		int memoryValue = strtoul(buffer, NULL, 0);

		memory[i] = memoryValue;
		i++;
	}
}

int main(int argc, char *argv[]) {
	// Check to see that input files were indeed provided.
	if (argc != 3) {
		printf("Two files must be supplied...\nExiting without doing anything.");
		exit(1);
	}
	
	char* imemin = argv[IMEMIN];
	char* dmemin = argv[DMEMIN];

	int commandAmount = 0;
	Command** commands = ReadCommandFile(imemin, &commandAmount);

	int mips[REGISTER_AMOUNT] = { 0 };

	int pc = 0;

	Command* command = commands[pc];
	int memory[MEM_SIZE] = { 0 };
	ReadMemory(dmemin, memory);

	while (command->opcode != HALT) {
		switch (command->opcode) {
		case ADD:
			add(mips, command->rd, command->rs, command->rt, command->rm, &pc);
			break;
		case SUB:
			sub(mips, command->rd, command->rs, command->rt, command->rm, &pc);
			break;
		case MAC:
			mac(mips, command->rd, command->rs, command->rt, command->rm, &pc);
			break;
		case AND:
			and (mips, command->rd, command->rs, command->rt, command->rm, &pc);
			break;
		case OR:
			or (mips, command->rd, command->rs, command->rt, command->rm, &pc);
			break;
		case XOR:
			xor (mips, command->rd, command->rs, command->rt, command->rm, &pc);
			break;
		}
	}

	return 0;
}