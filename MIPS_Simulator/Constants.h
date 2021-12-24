#pragma once

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#define MEM_SIZE 4096
#define LINE_LENGTH 500

#define IMM1 1
#define IMM2 2

#define REGISTER_AMOUNT 16
#define HW_REGISTER_AMOUNT 23

#define IMEMIN 1
#define DMEMIN 2
#define DISKIN 3
#define IRQ2IN 4
#define DMEMOUT 5
#define REGOUT 6
#define TRACE 7
#define HWREGTRACE 8
#define CYCLES 9
#define LEDS 10
#define DISPLAY7SEG 11
#define DISKOUT 12
#define MONITOR_TXT 13
#define MONITOR_YUV 14

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
#define BGE 14
#define JAL 15
#define LW 16
#define SW 17
#define RETI 18
#define IN 19
#define OUT 20
#define HALT 21