#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__
#include <inttypes.h>
#include "cbuffer.h"
#include "opcodes.h"

#define MAXARG_Bx (1 << 18 - 1)
#define MAXARG_sBx (MAXARG_Bx >> 1)

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

inline int Opcode(Instruction i) {
	return i & 0x3f;
}

inline TABC ABC(Instruction i) {
	TABC ret = {
		i >> 6 & 0xff,
		i >> 14 & 0x1ff,
		i >> 23 & 0x1ff
	};
	return ret;
}

inline TABx ABx(Instruction i) {
	TABx ret = {
		i >> 6 & 0xff,
		i >> 14
	};
	return ret;
}

inline TAsBx AsBx(Instruction i) {
	TABx temp = ABx(i);
	TAsBx ret = {
		temp.a,
		temp.bx - MAXARG_sBx
	};
	return ret;
}

inline int Ax(Instruction i) {
	return i >> 6;
}

inline const char* OpName(Instruction i) {
	return g_opcodes[Opcode(i)].name;
}

inline uint32_t OpMode(Instruction i) {
	return g_opcodes[Opcode(i)].opMode;
}

inline uint32_t BMode(Instruction i) {
	return g_opcodes[Opcode(i)].argBMode;
}

inline uint32_t CMode(Instruction i) {
	return g_opcodes[Opcode(i)].argCMode;
}


#endif
