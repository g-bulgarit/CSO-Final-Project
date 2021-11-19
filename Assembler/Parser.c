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

char GetRegisterByte_(char register_number[]) {
	// Convert register name to hex value
	for (RegisterCodes *p = (RegisterCodes *)Register_LUT; p->register_name != NULL; ++p) {
		if (!strcmp(p->register_name, register_number)) {
			return p->value;
		}
	}
	return 255;
}

char* FormatAsHex_(unsigned char opcode, unsigned char rd, unsigned char rs,
				   unsigned char rt, unsigned char rm, unsigned int imm1_in,
				   unsigned int imm2_in) {
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
	bundle3 = (imm1_in) << 12 | (imm2_in);

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

Label** AddNewLabel(char* labelText, int* pc, Label** labels, int labelArraySize) 
{
	Label* newLabel = (Label*)malloc(sizeof(Label));

	newLabel->tag = (char*)malloc(sizeof(labelText));
	strcpy(newLabel->tag, labelText);

	newLabel->targetAdress = pc;

	labels = (Label**)realloc(labels, sizeof(Label*) * (labelArraySize + 1));
	labels[labelArraySize] = newLabel;

	return labels;
}

CommandLine** AddNewCommandLine(char* commandName, char* fullCommand, int* pc, CommandLine** commands, int commandArraySize)
{
	CommandLine* newCommand = (CommandLine*)malloc(sizeof(CommandLine));
	newCommand->command = GetCommand(commandName);

	newCommand->commandText = (char*)malloc(sizeof(char*) * strlen(fullCommand));
	strcpy(newCommand->commandText, fullCommand);

	newCommand->adress = pc;

	commands = (Label**)realloc(commands, sizeof(CommandLine*) * (commandArraySize + 1));
	commands[commandArraySize] = newCommand;

	return commands;
}

void ScanFile(FILE* filePointer, CommandLine*** commands, Label*** labels)
{
	char buffer[LINE_LENGTH];
	int size = 0;
	int pc = 0;
	int labelAmount = 0;
	int commandAmount = 0;

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

		char** commandWords = Split(buffer, &size);
		if (size == 0) continue;

		if (size == 1 && commandWords[0][strlen(commandWords[0]) - 1] == ':') {
			// Found a label in a single line
			pc++;
			(*labels) = AddNewLabel(commandWords[0], &pc, (*labels), labelAmount);
			labelAmount++;
		}
		else if (size > 1 && commandWords[0][strlen(commandWords[0]) - 1] == ':') {
			// found a label with another command
		}
		else {
			// Found a command
			(*commands) = AddNewCommandLine(commandWords[0],buffer, &pc, (*commands), commandAmount);
			pc++;
			commandAmount++;
		}
	}
}

char* ParseSingleLine(char *line) {
	// Takes in a line from the ASM code,
	// Returns a full instruction struct, ignoring comments.
	// ASM line structure:
	//
	// opcode rd, rs, rt, rm, imm1, imm2 # comments \n
	// add     $t3, $t2, $t1, $t0, 0, 0

	// Define string container variables
	char* command;
	char* rd_in;
	char* rs_in;
	char* rt_in;
	char* rm_in;
	int imm1_in;
	int imm2_in;
	int size;

	char** commandWords = Split(line, &size);

	if (size == 0) return NULL;
	


	command = commandWords[0];
	rd_in = commandWords[1];
	rs_in = commandWords[2];
	rt_in = commandWords[3];
	rm_in = commandWords[4];
	imm1_in = commandWords[5];
	imm2_in = commandWords[6];

	#ifdef DEBUG
		printf("[!] Input:\nOpcode: %s, RD: %s, RS: %s, RT: %s, RM: %s, IMM1:%d, IMM2:%d\n", command, rd_in, rs_in, rt_in, rm_in, imm1_in, imm2_in);
	#endif

	CommandOpcodes* commandInfo = GetCommand(command);

	// Parse individual values with lookup tables
	unsigned char opcode = commandInfo->value;
	unsigned char rd = GetRegisterByte_(rd_in);
	unsigned char rs = GetRegisterByte_(rs_in);
	unsigned char rt = GetRegisterByte_(rt_in);
	unsigned char rm = GetRegisterByte_(rm_in);

	char* MIPSInstruction = FormatAsHex_(opcode, rd, rs, rt, rm, imm1_in, imm2_in);
	return MIPSInstruction;

	

	//// Read line into containers, ignore comments including the # sign
	//sscanf(line, "%s %[^,] , %[^,] , %[^,] , %[^,] , %d, %d", command, rd_in, rs_in, rt_in, rm_in, &imm1_in, &imm2_in);

	//// Try to check if line is a definition of GOTO statement by parsing by checking
	//// if :command: ends with :
	//if (command[strlen(command) - 1] == ':') {
	//	// This is a loop definition.
	//	// Store the loop name along with the current PC so we can return here later.
	//	#ifdef DEBUG
	//	printf("Found loop definition called %s\n", command);
	//	#endif	
	//}
	//

	//#ifdef DEBUG
	//printf("[!] Input:\nOpcode: %s, RD: %s, RS: %s, RT: %s, RM: %s, IMM1:%d, IMM2:%d\n", command, rd_in, rs_in, rt_in, rm_in, imm1_in, imm2_in);
	//#endif
	//
	//// Parse individual values with lookup tables
	//unsigned char opcode = GetCommandOpcode_(command);
	//unsigned char rd = GetRegisterByte_(rd_in);
	//unsigned char rs = GetRegisterByte_(rs_in);
	//unsigned char rt = GetRegisterByte_(rt_in);
	//unsigned char rm = GetRegisterByte_(rm_in);

	//char* MIPSInstruction = FormatAsHex_(opcode, rd, rs, rt, rm, imm1_in, imm2_in);
	//return MIPSInstruction;
}

int ParseFile(char* asmFilePath) {
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

	int size = 0;

	while (fgets(buffer, LINE_LENGTH - 1, rfp))
	{
		// As long as there are more lines to parse, throw them
		// into the :ParseSingleLine: function.
		buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

		#ifdef DEBUG
		printf("%s\n", buffer);
		#endif

		char* outline = ParseSingleLine(buffer);

		// If the return value is null the row was empty
		if (outline != NULL) 
		{
			//commandArray = AddNewCommandToList(outline, commandArray, ++size);
			//fprintf(afp, "%s\n", outline);  // Print to file with a newline
		}
	}

	fclose(rfp);
	fclose(afp);
	return 0;
}

int Assemble(char* asmFilePath) 
{
	Label** labels;
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

	ScanFile(rfp, &commands, &labels);

	return 0;
}