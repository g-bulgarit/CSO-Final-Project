#pragma once
// Utility methods
char GetCommandOpcode_(char opcode[]);
char GetRegisterByte_(char register_number[]);

// Helper functions
char* FormatAsHex_(unsigned char opcode, unsigned char rd, unsigned char rs, unsigned char rt, unsigned char rm, int imm1_in, int imm2_in);
int Assemble(char* argv[]);
