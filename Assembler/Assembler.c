#include <stdio.h>
#include "Parser.h"

int main() {
	// Open the file,
	// On first pass, hold the addresses of all labels and return
	// On second pass, replace labels with corresponding addresses, and write resulting bit-lines to file.
	int retval = ParseFile( /*Put file path here*/ );
}