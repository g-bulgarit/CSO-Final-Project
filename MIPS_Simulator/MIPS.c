#include "MIPS.h"

//
// Created by Itay Zilka on 17/11/2021.
//
void add(int* mips, int rd, int rs, int rt, int rm){
	mips[rd] = mips[rs] + mips[rt] + mips[rm];
}

void sub(int* mips, int rd, int rs, int rt, int rm){
	mips[rd] = mips[rs] - mips[rt] - mips[rm];
}

void mac(int* mips, int rd, int rs, int rt, int rm){
	mips[rd] = mips[rs] * mips[rt] + mips[rm];
}

void and(int* mips, int rd, int rs, int rt, int rm){
	mips[rd] = mips[rs] & mips[rt] & mips[rm];
}

void or(int* mips, int rd, int rs, int rt, int rm) {
	mips[rd] = mips[rs] | mips[rt] | mips[rm];
}

void xor(int* mips, int rd, int rs, int rt, int rm) {
	mips[rd] = mips[rs] ^ mips[rt] ^ mips[rm];
}