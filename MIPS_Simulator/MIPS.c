#include "MIPS.h"

//
// Created by Itay Zilka on 17/11/2021.
//
void add(int* mips, int rd, int rs, int rt, int rm){
	mips[rd] = mips[rs] + mips[rt] + mips[rm];
}
