#include <stdio.h>
#include "Parser.h"

char* test_str = "sub $t2, $t1, $t0, $s2, 0, 0 #  this is a comment";

int main() {
	// Open the file,
	// On first pass, hold the addresses of all labels and return
	// On second pass, replace labels with corresponding addresses, and write resulting bit-lines to file.
	ParseSingleLine(test_str);
}