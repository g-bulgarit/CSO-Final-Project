#include <stdio.h>
#include <stdlib.h>
#include "MIPS.h"

int main() {
	int mips_p[16] = {0};

	mips_p[11] = 7;
	mips_p[12] = 3;
	add(mips_p, 10, 11, 12, 0);
	printf("%d", mips_p[10]);
}