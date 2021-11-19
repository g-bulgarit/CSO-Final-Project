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

	char output[15];
	unsigned int bundle1, bundle2, bundle3, c1, c2, c3;

	// Do bit magic to constrain values to the format given
	bundle1 = (rd & 0xF) << 4 | (rs & 0xF);
	bundle2 = (rt & 0xF) << 4 | (rm & 0xF);

	// For bundle 3, do this in a few steps:
	bundle3 = (imm1_in & 0xFFF) << 12 | (imm2_in & 0xFFF);

	// Multiply by 0xFF here to keep enough information for 2 hex characters...
	c1 = (bundle3 >> 16) & 0xFF;
	c2 = (bundle3 >> 8) & 0xFF;
	c3 = bundle3 & 0xFF;

	#ifdef DEBUG
	printf("[!] Converted Instruction:\n%02X%02X%02X%02X%02X%02X\n", opcode, bundle1, bundle2, c1, c2, c3);
	#endif

	// Push formatted information to string and terminate it
	sprintf(output, "%02X%02X%02X%02X%02X%02X", opcode, bundle1, bundle2, c1, c2, c3);
	output[14] = "\0";
	return output;
}

char* BritMila(char* word, char* delim)
{
	char* token = strtok(word, delim);

	if (token == NULL) return word;
	return token;
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

Label** AddNewLabel(char* labelText, int pc, Label** labels, int labelArraySize) 
{
	Label* newLabel = (Label*)malloc(sizeof(Label));
	
	newLabel->tag = (char*)malloc(sizeof(labelText));
	strcpy(newLabel->tag, labelText);

	newLabel->targetAdress = pc;

	labels = (Label**)realloc(labels, sizeof(Label*) * (labelArraySize + 1));
	labels[labelArraySize] = newLabel;

	return labels;
}

CommandLine** AddNewCommandLine(char* commandName, char* fullCommand, int pc, CommandLine** commands, int commandArraySize)
{
	CommandLine* newCommand = (CommandLine*)malloc(sizeof(CommandLine));
	newCommand->command = GetCommand(commandName);

	newCommand->commandText = (char*)malloc(sizeof(char*) * strlen(fullCommand));
	strcpy(newCommand->commandText, fullCommand);

	newCommand->address = pc;

	commands = (Label**)realloc(commands, sizeof(CommandLine*) * (commandArraySize + 1));
	commands[commandArraySize] = newCommand;

	return commands;
}

void ScanFile(FILE* filePointer, CommandLine*** commands, Label*** labels, int* commandAmount, int* labelAmount)
{
	char buffer[LINE_LENGTH];
	int size = 0;
	int pc = 0;
	*labelAmount = 0;
	*commandAmount = 0;

	(*labels) = (Label**)malloc(sizeof(Label*));
	(*commands) = (CommandLine**)malloc(sizeof(CommandLine*));

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
			(*labels) = AddNewLabel(BritMila(commandWords[0], ":"), pc, (*labels), *labelAmount);
			(*labelAmount)++;
		}
		else if (size > 1 && commandWords[0][strlen(commandWords[0]) - 1] == ':') {
			// found a label with another command
		}
		else {
			// Found a command
			(*commands) = AddNewCommandLine(commandWords[0],buffer, pc, (*commands), *commandAmount);
			pc++;
			(*commandAmount)++;
		}
	}
}

Label* FindLabel(Label** labels, int labelAmount, char* labelTag)
{
	for (int i = 0; i < labelAmount; i++) {
		Label* label = labels[i];
		if (!strcmp(label->tag, labelTag)) {
			return label;
		}
	}
	return NULL;
}

char** ParseCommands(CommandLine** commands, Label** labels, int commandAmount, int labelAmount) 
{
	CommandLine* command;
	char* rd_in;
	char* rs_in;
	char* rt_in;
	char* rm_in;
	int imm1_in;
	int imm2_in;
	int size;
	int mipsAmount = 0;
	char** MipsInstructions = (char**)malloc(sizeof(char*));

	for (int i = 0; i < commandAmount; i++)
	{
		command = commands[i];

		char** commandWords = Split(command->commandText, &size);

		char text[20];
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
		unsigned char rd = GetRegisterByte_(BritMila(commandWords[1],","));
		unsigned char rs = GetRegisterByte_(BritMila(commandWords[2],","));
		unsigned char rt = GetRegisterByte_(BritMila(commandWords[3],","));
		unsigned char rm = GetRegisterByte_(BritMila(commandWords[4],","));

		char imm1 = atoi(BritMila(commandWords[5],","));
		char imm2 = atoi(BritMila(commandWords[6],","));

		mipsAmount++;
		MipsInstructions = (char**)realloc(MipsInstructions, sizeof(char*) * mipsAmount);

		char* instruction = FormatAsHex_(opcode, rd, rs, rt, rm, imm1, imm2);

		MipsInstructions[mipsAmount - 1] = (char*)malloc(strlen(instruction));

		strcpy(MipsInstructions[mipsAmount - 1], instruction);
	}

	return MipsInstructions;
}

int Assemble(char* asmFilePath) 
{
	int labelAmount;
	Label** labels;
	int commandAmount;
	CommandLine** commands;

	// Function to parse an assembly file to our SIMP isa.
	// Takes in asm file path and outputs a text file containing
	// the assembled machine code commands as hex.

	char** commandArray = NULL;

	FILE* rfp;
	FILE* wfp;
	FILE* afp;
	char buffer[LINE_LENGTH];

	int* PC = 0;

	rfp = fopen(asmFilePath, "r");

	// Create an out.txt file if it does not exist, and erase
	// the content of it, if it exists.
	// Maybe this is not needed, and we can just "w+" without doing the
	// "a" mode later on.
	wfp = fopen("imemin.txt", "w+");
	fclose(wfp);


	afp = fopen("imemin.txt", "a"); // Reopen the outfile as "append".

	if (rfp == NULL) {
		// Failed to open file
		return 1;
	}

	ScanFile(rfp, &commands, &labels, &commandAmount, &labelAmount);

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

	char** mips = ParseCommands(commands, labels, commandAmount, labelAmount);

	for (int i = 0; i < commandAmount; i++)
	{
		fprintf(afp, "%s\n", mips[i]);  // Print to file with a newline
	}

	fclose(rfp);
	fclose(afp);
	return 0;
}