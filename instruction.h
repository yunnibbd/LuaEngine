#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__
#include <inttypes.h>
#include "lua_common_type.h"
#include "opcodes.h"

#define MAXARG_Bx ((1 << 18) - 1)
#define MAXARG_sBx ((MAXARG_Bx) >> 1)

typedef struct {
	int a, b, c;
} TABC;

typedef struct {
	int a, bx;
} TABx;

typedef struct{
	int a, sbx;
} TAsBx;

inline int InstructionOpcode(Instruction i) {
	return i & 0x3f;
}

inline TABC InstructionABC(Instruction i) {
	TABC ret = {
		(i >> 6) & 0xff,
		(i >> 14) & 0x1ff,
		(i >> 23) & 0x1ff
	};
	return ret;
}

inline TABx InstructionABx(Instruction i) {
	TABx ret = {
		(i >> 6) & 0xff,
		i >> 14
	};
	return ret;
}

inline TAsBx InstructionAsBx(Instruction i) {
	TABx temp = InstructionABx(i);
	TAsBx ret = {
		temp.a,
		temp.bx - MAXARG_sBx
	};
	return ret;
}

inline int InstructionAx(Instruction i) {
	return i >> 6;
}

inline const char* InstructionOpName(Instruction i) {
	return g_opcodes[InstructionOpcode(i)].name;
}

inline uint32_t InstructionOpMode(Instruction i) {
	return g_opcodes[InstructionOpcode(i)].opMode;
}

inline uint32_t InstructionBMode(Instruction i) {
	return g_opcodes[InstructionOpcode(i)].argBMode;
}

inline uint32_t InstructionCMode(Instruction i) {
	return g_opcodes[InstructionOpcode(i)].argCMode;
}

inline void InstructionExecute(Instruction i, LuaVM vm) {
	InstructoinActionFuncType action = g_opcodes[InstructionOpcode(i)].action;
	if (action != NULL) {
		action(vm, i);
	}
	else {
		printf("%s\n", InstructionOpName(i));
		exit(-1);
	}
}

#endif
