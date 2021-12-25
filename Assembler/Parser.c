#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Parser.h"
#include "Constants.h"

CommandOpcodes* GetCommand(char* opcode) 
{
	// Convert command opcode to hex value
	for (CommandOpcodes* p = (CommandOpcodes*)Opcode_LUT; p->command_name != NULL; ++p) {
		if (!strcmp(p->command_name, opcode)) {
			return p;
		}
	}
	return NULL;
}

char GetCommandOpcode_(char opcode[]) {
	// Convert command opcode to hex value
	for (CommandOpcodes *p = (CommandOpcodes *)Opcode_LUT; p->command_name != NULL; ++p) {
		if (!strcmp(p->command_name, opcode)) {
			return p->value;
		}
	}
	return 255;
}

char GetRegisterByte_(char* register_name) {
	// Convert register name to hex value
	for (RegisterCodes *p = (RegisterCodes *)Register_LUT; p->register_name != NULL; ++p) {
		if (!strcmp(p->register_name, register_name)) {
			return p->value;
		}
	}
	return 255;
}

char* FormatAsHex_(unsigned char opcode, unsigned char rd, unsigned char rs,
				   unsigned char rt, unsigned char rm, int imm1_in,
				   int imm2_in) {
	// Map:
	//			                opcode        bundle1         bundle2       bundle3
	//	grouping		           8       ----8---        ----8----       ----24---
	//	bits		             47:40   39:36   35:32   31:28   27:24   23:12   11:0
	//	explanation		        opcode     rd      rs      rt      rm     imm1    imm2
	//			
	//	variable names  		opcode     rd      rs      rt      rm     c1   c2   c3

	char output[13];
	unsigned int bundle1, bundle2, bundle3, c1, c2, c3;

	// Do bit magic to constrain values to the format given
	bundle1 = (rd & 0xF) << 4 | (rs & 0xF);
	bundle2 = (rt & 0xF) << 4 | (rm & 0xF);

	// For bundle 3, do this in a few steps:
	bundle3 = (imm1_in & 0xFFF) << 12 | (imm2_in & 0xFFF);

	// Multiply by 0xFF here to keep enough information for 2 hex characters....
	c1 = (bundle3 >> 16) & 0xFF;
	c2 = (bundle3 >> 8) & 0xFF;
	c3 = bundle3 & 0xFF;

	#ifdef DEBUG
	printf("[!] Converted Instruction:\n%02X%02X%02X%02X%02X%02X\n", opcode, bundle1, bundle2, c1, c2, c3);
	#endif

	// Push formatted information to string and terminate it
	sprintf(output, "%02X%02X%02X%02X%02X%02X", opcode, bundle1, bundle2, c1, c2, c3);
	output[12] = '\0';
	return &(output[0]);
}

char* RightStrip(char* word, char* delim) {
	// Equivalent to right-stripping.
	char* token = strtok(word, delim);
	if (token == NULL) return word;
	return token;
}

void ReplaceHexWithInt(char* word) {
	// Check wordArray at the IMM locations, if hex number is found - replace with int.
	RightStrip(word, ",");
	if (word[1] == 'x' || word[1] == 'X') {
		long int value = strtol(word, NULL, 0);
		sprintf(word, "%d", (int)value);
	}
}

char** Split(char* stringToSplit, int* outArraySize) 
{
	(*outArraySize) = 1;
	char** stringParts;
	stringParts = (char**)malloc(sizeof(char*) * (*outArraySize));

	// Extract the first token
	char* token = strtok(stringToSplit, " \t");

	// loop through the string to extract all other tokens
	while (token != NULL && token[0] != '#') {
		stringParts[(*outArraySize) - 1] = token;
		(*outArraySize)++;
		stringParts = (char**)realloc(stringParts, sizeof(char*) * (*outArraySize));
		token = strtok(NULL, " \t");
	}

	(*outArraySize)--;
	return stringParts;
}

WordCommand** AddNewWordCommand(char* address, char*value,WordCommand** wordCommands, int wordCommandArraySize) {
	// Parse .word command into a struct

	WordCommand* newWordCommand = (WordCommand*)malloc(sizeof(WordCommand));

	ReplaceHexWithInt(address);
	newWordCommand->address = atoi(address);

	ReplaceHexWithInt(value);
	newWordCommand->value = atoi(value);

	wordCommands = (WordCommand**)realloc(wordCommands, sizeof(WordCommand*) * (wordCommandArraySize + 1));
	wordCommands[wordCommandArraySize] = newWordCommand;

	return wordCommands;
}

Label** AddNewLabel(char* labelText, int pc, Label** labels, int labelArraySize) {
	// Parse new label into a struct.

	Label* newLabel = (Label*)malloc(sizeof(Label));
	
	newLabel->tag = (char*)malloc(sizeof(labelText));
	strcpy(newLabel->tag, labelText);

	newLabel->targetAdress = pc;

	labels = (Label**)realloc(labels, sizeof(Label*) * (labelArraySize + 1));
	labels[labelArraySize] = newLabel;

	return labels;
}

CommandLine** AddNewCommandLine(char* commandName, char* fullCommand, int pc, CommandLine** commands, int commandArraySize) {
	// Parse new command into a struct.

	CommandLine* newCommand = (CommandLine*)malloc(sizeof(CommandLine));
	newCommand->command = GetCommand(commandName);

	newCommand->commandText = (char*)malloc(sizeof(char*) * strlen(fullCommand));
	strcpy(newCommand->commandText, fullCommand);

	newCommand->address = pc;

	commands = (CommandLine**)realloc(commands, sizeof(CommandLine*) * (commandArraySize + 1));
	commands[commandArraySize] = newCommand;

	return commands;
}

void ScanFile(FILE* filePointer, CommandLine*** commands, Label*** labels, int* commandAmount, int* labelAmount, WordCommand*** wordCommands, int* wordCommandAmount) {
	// Function to look up and store all labels in the source ASM to an array,
	// and all commands and their PC in another array.

	char buffer[LINE_LENGTH];
	int size = 0;
	int pc = 0;
	*labelAmount = 0;
	*commandAmount = 0;
	*wordCommandAmount = 0;

	(*labels) = (Label**)malloc(sizeof(Label*));
	(*commands) = (CommandLine**)malloc(sizeof(CommandLine*));
	(*wordCommands) = (WordCommand**)malloc(sizeof(WordCommand*));

	while (fgets(buffer, LINE_LENGTH - 1, filePointer))
	{
		// As long as there are more lines to parse, throw them
		// into the :ParseSingleLine: function.
		buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

#ifdef DEBUG
		printf("%s\n", buffer);
#endif
		char* bufferCopy = (char*)malloc(sizeof(buffer));
		strcpy(bufferCopy, buffer);

		char** commandWords = Split(bufferCopy, &size);
		if (size == 0) continue;

		if (size == 1 && commandWords[0][strlen(commandWords[0]) - 1] == ':') {
			// Found a label in a single line
			(*labels) = AddNewLabel(RightStrip(commandWords[0], ":"), pc, (*labels), *labelAmount);
			(*labelAmount)++;
		}
		else if (commandWords[0][0] == '.') {
			// Found a label in a single line
			(*wordCommands) = AddNewWordCommand(commandWords[1], commandWords[2], (*wordCommands), *wordCommandAmount);
			(*wordCommandAmount)++;
		}
		else if (size > 1 && commandWords[0][strlen(commandWords[0]) - 1] == ':') {
			// found a label with another command
			(*labels) = AddNewLabel(RightStrip(commandWords[0], ":"), pc, (*labels), *labelAmount);
			(*labelAmount)++;

			(*commands) = AddNewCommandLine(commandWords[1], buffer, pc, (*commands), *commandAmount);
			pc++;
			(*commandAmount)++;
		}
		else {
			// Found a command
			(*commands) = AddNewCommandLine(commandWords[0],buffer, pc, (*commands), *commandAmount);
			pc++;
			(*commandAmount)++;
		}
	}
}

Label* FindLabel(Label** labels, int labelAmount, char* labelTag) {
	// Function to return a ptr to a full label struct by it's name.
	for (int i = 0; i < labelAmount; i++) {
		Label* label = labels[i];
		if (!strcmp(label->tag, labelTag)) {
			return label;
		}
	}
	return NULL;
}

void ParserDebugPrints(CommandLine** commands, Label** labels, int commandAmount, int labelAmount) {
	// Function to print all labels and their addresses,
	// And all commands and their PC.

	printf("Labels: \n");
	for (int i = 0; i < labelAmount; i++)
	{
		printf("%s, %d\n", labels[i]->tag, labels[i]->targetAdress);
	}
	printf("Commands: \n");

	for (int i = 0; i < commandAmount; i++)
	{
		printf("%d: %s\n", commands[i]->address, commands[i]->command->command_name);
	}
}

char** ParseCommands(CommandLine** commands, Label** labels, int commandAmount, int labelAmount) 
{
	// Function to go over the lines of ASM code, translate them to MIPS-format machine code,
	// and replace all labels with the corresponding addresses to jump to.

	CommandLine* command;
	int size;
	int mipsAmount = 0;
	char** instrctionArray = (char**)malloc(sizeof(char*));

	for (int i = 0; i < commandAmount; i++)
	{
		command = commands[i];
		char** commandWords = Split(command->commandText, &size);
		char text[20];

		// Check to see if the IMM registers contain hex numbers, and replace with ints
		// in case it does.
		ReplaceHexWithInt(commandWords[5]);
		ReplaceHexWithInt(commandWords[6]);

		// Parse labels in IMM registers, to see if anything needs to be replaced by
		// an address.

		// Check if there is a label tag in $imm1
		Label* label = FindLabel(labels, labelAmount, commandWords[size - 2]);

		if (label != NULL) {
			itoa(label->targetAdress, text, 10);
			strcpy(commandWords[size - 2], text);
		}

		// Check if there is a label tag in $imm2
		label = FindLabel(labels, labelAmount, commandWords[size - 1]);

		if (label != NULL) {
			itoa(label->targetAdress, text, 10);
			strcpy(commandWords[size - 1], text);
		}

		// Parse individual values with lookup tables
		unsigned char opcode = command->command->value;
		unsigned char rd = GetRegisterByte_(RightStrip(commandWords[1],","));
		unsigned char rs = GetRegisterByte_(RightStrip(commandWords[2],","));
		unsigned char rt = GetRegisterByte_(RightStrip(commandWords[3],","));
		unsigned char rm = GetRegisterByte_(RightStrip(commandWords[4],","));

		int imm1 = strtoul(RightStrip(commandWords[5],","),NULL, 0);
		int imm2 = strtoul(RightStrip(commandWords[6], ","), NULL, 0);
		mipsAmount++;

		// Reallocate enough space for extending the array with 1 more instruction,
		// then allocate memory inside for a char* that will hold the line content.
		// The hardcoded value ALLOC_SINGLE_OUTPUT_LINE_SIZE is due to a bug we didn't
		// manage to fully understand, where doing the realloc operation after we 
		// call the FormatAsHex_ function resulted in weird behaviour that wiped all of 
		// our pointers clean.

		instrctionArray = (char**)realloc(instrctionArray, sizeof(char*) * mipsAmount);
		instrctionArray[mipsAmount - 1] = (char*)malloc(ALLOC_SINGLE_OUTPUT_LINE_SIZE);

		char* single_instruction = FormatAsHex_(opcode, rd, rs, rt, rm, imm1, imm2);

		// Check malloc retval...
		if (instrctionArray[mipsAmount - 1] == NULL) {;
			printf("[ERROR: Parser.c] Failed to allocate space for the string, can not proceed. Exiting.");
			exit(1);
		}
		// If we managed to allocate space for the new line, place a ptr to it in the array.
		strcpy(instrctionArray[mipsAmount - 1], single_instruction);
	}

	return instrctionArray;
}

void WriteMemoryDump(WordCommand** wordCommands, int wordArrayLength, char* argv[]) {
	// Function to write a memory dump consisting of all .word instructions.
	// The function directly outputs a file.

	int memoryDump[4096] = { 0 }; // Initialize array of zeros

	for (int i = 0; i < wordArrayLength; i++)
	{
		WordCommand* wd = wordCommands[i];
		memoryDump[wd->address] = wd->value;
	}

	FILE* wfp = fopen(argv[DMEMIN], "w+");

	// Cap length of the file.
	int previousI = 0;

	for (int i = 0; i < 4096; i++)
	{
		if (memoryDump[i] == 0) continue;

		for (int j = previousI; j <= i; j++)
		{
			char hexRep[9];
			sprintf(hexRep, "%08x", memoryDump[j]);
			fprintf(wfp, "%s\n", hexRep);  // Print to file with a newline
		}
		previousI = i + 1;
	}
}

int Assemble(char* argv[]) 
{
	// Function to parse an assembly file to our SIMP isa.
	// Takes in asm file path and outputs a text file containing
	// the assembled machine code commands as hex.

	int labelAmount;
	Label** labels;
	int commandAmount;
	CommandLine** commands;

	int wordCommandAmout;
	WordCommand** wordsCommands;

	char** commandArray = NULL;

	FILE* rfp;
	FILE* wfp;
	FILE* afp;

	int* PC = 0;

	rfp = fopen(argv[PROGRAM], "r");

	// Create an out.txt file if it does not exist, and erase
	// the content of it, if it exists.
	// Maybe this is not needed, and we can just "w+" without doing the
	// "a" mode later on.
	wfp = fopen(argv[IMEMIN], "w+");
	fclose(wfp);


	afp = fopen(argv[IMEMIN], "a"); // Reopen the outfile as "append".

	if (rfp == NULL) {
		// Failed to open file
		return 1;
	}

	ScanFile(rfp, &commands, &labels, &commandAmount, &labelAmount, &wordsCommands, &wordCommandAmout);

	#ifdef DEBUG
	ParserDebugPrints(commands, labels, commandAmount, labelAmount); // Print all labels and commands.
	#endif

	char** mips = ParseCommands(commands, labels, commandAmount, labelAmount);

	for (int i = 0; i < commandAmount; i++)
	{
		fprintf(afp, "%s\n", mips[i]);  // Print to file with a newline
	}

	fclose(rfp);
	fclose(afp);

	WriteMemoryDump(wordsCommands, wordCommandAmout, argv);
	return 0;
}