#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__
#include <inttypes.h>

typedef uint32_t Instruction;

typedef struct {
	int a, b, c;
} TABC;

typedef struct {
	int a, bx;
} TABx;

typedef struct{
	int a, sbx;
} TAsBx;

int Opcode(Instruction i);

TABC ABC(Instruction i);

TABx ABx(Instruction i);

TAsBx AsBx(Instruction i);

int Ax(Instruction i);

#endif
