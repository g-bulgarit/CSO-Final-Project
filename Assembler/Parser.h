#pragma once
char GetCommandOpcode_(char opcode[]);
void ParseSingleLine(char *line);
char* FormatAsHex_(unsigned char opcode, unsigned char rd, unsigned char rs, unsigned char rt, unsigned char rm, unsigned int imm1_in, unsigned int imm2_in);
