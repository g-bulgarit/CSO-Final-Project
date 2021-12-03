#include <stdio.h>
#include <stdlib.h>
#include "Shared.h"

void ReadMemoryFile(char* fileName, int* memoryBuffer)
{
	// Read memory frmo dmemin.txt file into a preallocated array of 4096 zeros.
	FILE* rfp = fopen(fileName, "r");
	char buffer[LINE_LENGTH];

	int i = 0;

	while (fgets(buffer, LINE_LENGTH - 1, rfp))
	{
		buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

#ifdef DEBUG
		printf("Memory line : %s\n", buffer);
#endif

		int memoryValue = strtoul(buffer, NULL, 0);

		memoryBuffer[i] = memoryValue;
		i++;
	}
}