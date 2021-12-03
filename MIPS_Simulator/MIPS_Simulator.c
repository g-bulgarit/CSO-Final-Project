#include <stdio.h>
#include <stdlib.h>
#include "Constants.h"
#include "MIPS.h"
#include <string.h>
#include "IO.h"

int PartOfStringToInt(char* string,int start, int length) {
	// Function to split string to substrings and convert these substrings to integers.

	char* part = (char*)malloc((size_t)length + 1);
	if (part == NULL) return 0; // Nothing to split
	
	memcpy(part, &string[start], length);
	part[length] = '\0'; // Null terminate the string before we can use atoi
	int intPart = atoi(part);
	return intPart;
}

Command** AddNewCommand(char* command, Command** commands, int commandArraySize) {
	// Allocate memory and push command string to a struct.

	Command* newCommand = (Command*)malloc(sizeof(Command));
	if (newCommand == NULL) return NULL;

	newCommand->opcode = PartOfStringToInt(command, 0, 2);
	newCommand->rd = PartOfStringToInt(command, 2, 1);
	newCommand->rs = PartOfStringToInt(command, 3, 1);
	newCommand->rt = PartOfStringToInt(command, 4, 1);
	newCommand->rm = PartOfStringToInt(command, 5, 1);
	newCommand->imm1 = PartOfStringToInt(command, 6, 3);
	newCommand->imm2 = PartOfStringToInt(command, 9, 3);

	commands = (Command**)realloc(commands, sizeof(Command*) * (commandArraySize + 1));
	commands[commandArraySize] = newCommand;

	return commands;
}

Command** ReadCommandFile(char* imemin, int* commandAmount)
{
	// Read instructions from imemin.txt file into a dynamic array.

	FILE* rfp = fopen(imemin, "r");
	char buffer[LINE_LENGTH];

	Command** commands = (Command**)malloc(sizeof(Command*));

	while (fgets(buffer, LINE_LENGTH - 1, rfp))
	{
		// As long as there are more lines to read, read them
		buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

#ifdef DEBUG
		printf("Command line : %s\n", buffer);
#endif
		// Pass each new line into a command constructor.
		commands = AddNewCommand(buffer, commands, *commandAmount);
		(*commandAmount)++;
	}

	return commands;
}

void ReadMemory(char* dmemin, int* memory) 
{
	// Read memory frmo dmemin.txt file into a preallocated array of 4096 zeros.
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
	if (argc != 5) {
		printf("Four files must be supplied (Instuctions, Memory Dump, Disk Dump and IRQ2 cycles)...\nExiting without doing anything.");
		exit(1);
	}
	
	// Parse argv
	char* imemin = argv[IMEMIN];
	char* dmemin = argv[DMEMIN];
	char* diskin = argv[DISKIN]; // TODO: make use of this.
	char* irq2in = argv[IRQ2IN];
	
	// Initialize MIPS registers as array of integers.
	int mips[REGISTER_AMOUNT] = { 0 };
	int pc = 0;
	
	// Read commands and input memory dump.
	int commandAmount = 0;
	Command** commands = ReadCommandFile(imemin, &commandAmount);
	int memory[MEM_SIZE] = { 0 };
	unsigned long long cycle = 0; // Can count pretty high :)
	ReadMemory(dmemin, memory);

	// Initialize IRQ2 interrupt cycles array to be used later.
	InitializeIRQ2Cycles(irq2in);
	

	// Fetch command as current PC
	Command* command = commands[pc];
	while (command->opcode != HALT) {

		// Increment clock-cycle count and check if there is an interrupt.
		cycle++;
		Interrupt(&pc, cycle);

		// Decode opcode and values and execute them.
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

		command = commands[pc]; // Fetch next command.
	}

	return 0;
}