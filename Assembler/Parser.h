#pragma once
// Utility methods
char GetCommandOpcode_(char opcode[]);
char GetRegisterByte_(char register_number[]);

// Helper functions
char* FormatAsHex_(unsigned char opcode, unsigned char rd, unsigned char rs, unsigned char rt, unsigned char rm, unsigned int imm1_in, unsigned int imm2_in);

char* ParseSingleLine(char *line);
int ParseFile(char* InFilePath);
int Assemble(char* asmFilePath);