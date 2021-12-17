#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"
#include "Constants.h"

char screen[SCREEN_SIZE][SCREEN_SIZE];

void initializeScreen() {
	for (int i = 0; i < SCREEN_SIZE; i++) {
		for (int j = 0; j < SCREEN_SIZE; j++) {
			screen[i][j] = 0;
		}
	}
}

void updatePixel(int row, int col, char color) {
	screen[row][col] = color;
}

void DumpMonitorFiles() {
	FILE* monTxt;
	FILE* monYuv;

	monYuv = fopen("monitor.yuv", "wb+");
	monTxt = fopen("monitor.txt", "w+");

	if (monYuv == NULL || monTxt == NULL) {
		printf("[ERROR] Failed to open monitor output files for writing.\nExiting.");
		exit(-1);
	}

	for (int row = 0; row < SCREEN_SIZE; row++)
	{
		for (int col = 0; col < SCREEN_SIZE; col++)
		{
			// For monitor.txt, write every pixel to a separate line.
			fprintf(monTxt, "%02x\n", 0xFF & screen[row][col]);
			// For monitor.yuv, write every pixel to a binary
			char byte_to_write = 0xFF & screen[row][col];
			fwrite(&byte_to_write, sizeof(byte_to_write), 1, monYuv);
		}
	}

	// Close files.
	fclose(monTxt);
	fclose(monYuv);
	
}