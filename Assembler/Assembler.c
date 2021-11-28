#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"

int main(int argc, char *argv[]) {
	// Open the file,
	// On first pass, hold the addresses of all labels and return
	// On second pass, replace labels with corresponding addresses, and write resulting bit-lines to file.
	
	if (argc != 2) {
		printf("ASM file must be supplied...\nExiting without doing anything.");
		exit(1);
	}
	else {
		char* inputFilePath = argv[1];
		int retval = Assemble(inputFilePath);
		if (retval != 0) {
			printf("Failed to open ASM file...\nExiting without doing anything.");
			exit(1);
		}
	}
}