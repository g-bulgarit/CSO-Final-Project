#pragma once

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#define MEM_SIZE 4096
#define LINE_LENGTH 500


#define REGISTER_AMOUNT 16
#define HW_REGISTER_AMOUNT 22

#define IMEMIN 1
#define DMEMIN 2
#define DISKIN 3
#define IRQ2IN 4

#define ADD 0
#define SUB 1
#define MAC 2
#define AND 3
#define OR 4
#define XOR 5
#define SLL 6
#define SRA 7
#define SRL 8
#define BEQ 9
#define BNE 10
#define BLT 11
#define BGT 12
#define BLE 13
#define BJE 14
#define JAL 15
#define LW 16
#define SW 17
#define RETI 18
#define IN 19
#define OUT 20
#define HALT 21