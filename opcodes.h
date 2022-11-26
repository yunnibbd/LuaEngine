#ifndef __OPCODES_H__
#define __OPCODES_H__
#include "cbuffer.h"

enum CodecMode {
	IABC,
	IABx,
	IAsBx,
	IAx,
};

enum OperatorCode {
	OP_MOVE,	OP_LOADK,	OP_LOADKX,	OP_LOADBOOL,
	OP_LOADNIL, OP_GETUPVAL,OP_GETTABUP,OP_GETTABLE,
	OP_SETTABUP,OP_SETUPVAL,OP_SETTABLE,OP_NEWTABLE,
	OP_SELF,	OP_ADD,		OP_SUB,		OP_MIL,
	OP_MOD,		OP_POW,		OP_DIV,		OP_IDIV,
	OP_BAND,	OP_BOR,		OP_BXOR,	OP_SHL,
	OP_SHR,		OP_UNM,		OP_BNOT,	OP_NOT,
	OP_LEN,		OP_CONCAT,	OP_JMP,		OP_EQ,
	OP_LT,		OP_LE,		OP_TEST,	OPTESTSET,
	OP_CALL,	OP_TALLCALL,OP_RETURN,	OP_FORLOOP,
	OP_FORPREP,	OP_TFORCALL,OP_TFORLOOP,OP_SSETLIST,
	OP_CLOSURE, OP_VARARG,	OP_EXTRAARG,
};

enum OperatorNum {
	OpArgN,	//argument is not used
	OpArgU, //argument is used
	OpArgR, //argument is a register or a jump offset
	OpArgK, //argument is a constant or register/constant
};

typedef struct {
	unsigned char testFlag; //operator is a test (next instruction muse be a jump)
	unsigned char setAFlag; //instruction set register A
	unsigned char argBMode; //B arg mode
	unsigned char argCMode; //C arg mode
	unsigned char opMode;	//op mode
	CBuffer name;
} OpCode;

extern OpCode g_opcodes[47];
#endif
