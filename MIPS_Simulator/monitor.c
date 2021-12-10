#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"

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