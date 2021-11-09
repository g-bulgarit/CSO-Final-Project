#pragma once
char GetCommandOpcode_(char opcode[]);
void ParseSingleLine(char *line);
char FormatAsBits_(char opcode, char rd, char rs, char rt, char rm, char imm1_in, char imm2_in);
