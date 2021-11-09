#include <stdio.h>
#include "Parser.h"

char* test_str = "sub $t3, $t2, $t1, $t0, 0, 0 #  this is a comment";

int main() {
	ParseSingleLine(test_str);
}