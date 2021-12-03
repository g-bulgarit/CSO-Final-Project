#pragma once
#include "Constants.h"
unsigned int hw_reg[HW_REGISTER_AMOUNT] = { 0 };

#define DISK_CYCLE_USAGE 1024;

#define DISKCMD 14;
#define DISKSECTOR 15;
#define DISKBUFFER 16;
#define DISKSTATUS 17;
