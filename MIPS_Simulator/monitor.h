#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define SCREEN_SIZE 256

void updatePixel(int row, int col, char color);
void DumpMonitorFiles(char* txtFileName, char* yuvFileName);
