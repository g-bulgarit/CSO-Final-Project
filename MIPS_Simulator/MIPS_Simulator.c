#include <stdio.h>
#include <stdlib.h>
#include "MIPS.h"

int main(int argc, char *argv[]) {
	// Check to see that input files were indeed provided.
	if (argc != 3) {
		printf("Two files must be supplied...\nExiting without doing anything.");
		exit(1);
	}

	char * hexInstructionsFilePath = argv[1];
	char * memoryFilePath = argv[2]; // currently unused.

	// Define array to hold all MIPS registers.
	int mips_test[16] = {0};
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
	and(mips_test, 0, 1, 2, 3);
	printf("and1: %d\n", mips_test[0] == 1);
	or(mips_test, 0, 1, 2, 3);
	printf("or1:  %d\n", mips_test[0] == 3);
	xor(mips_test, 0, 1, 2, 3);
	printf("xor1: %d\n", mips_test[0] == 3);

	mips_test[1] = 7;  // 00..0000111
	mips_test[2] = 5;  // 00..0000101
	mips_test[3] = 13; // 00..0001101
	and(mips_test, 0, 1, 2, 3);
	printf("and2: %d\n", mips_test[0] == 5);
	or(mips_test, 0, 1, 2, 3);
	printf("or2:  %d\n", mips_test[0] == 15);
	xor(mips_test, 0, 1, 2, 3);
	printf("xor2: %d\n", mips_test[0] == 15);

	mips_test[1] = 1;  // 00..0000001
	mips_test[2] = 2;  // 00..0000010
	mips_test[3] = 4;  // 00..0000100
	and(mips_test, 0, 1, 2, 3);
	printf("and3: %d\n", mips_test[0] == 0);
	or(mips_test, 0, 1, 2, 3);
	printf("or3:  %d\n", mips_test[0] == 7);
	xor(mips_test, 0, 1, 2, 3);
	printf("xor3: %d\n", mips_test[0] == 7);


}