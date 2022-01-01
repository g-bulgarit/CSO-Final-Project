#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"

int main(int argc, char *argv[]) {
	// Open the file,
	// On first pass, hold the addresses of all labels and return
	// On second pass, replace labels with corresponding addresses, and write resulting bit-lines to file.
	
	if (argc != 4) {
		printf("Please provide: <program_name>.asm imemin.txt dmemin.txt\nExiting without doing anything.");
		exit(1);
	}

	else {
		char* inputFilePath = argv[1];
		int retval = Assemble(argv);
		if (retval != 0) {
			printf("Failed to open files...\nExiting without doing anything.");
			exit(1);
		}
	}
}