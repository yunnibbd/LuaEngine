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

OpCode g_opcodes[] = {
	/*     T  A     B       C    mode    name */
	OpCode{0, 1, OpArgR, OpArgN, IABC,  "MOVE     "},
	OpCode{0, 1, OpArgK, OpArgN, IABx,  "LOADK    "},
	OpCode{0, 1, OpArgN, OpArgN, IABx,  "LOADKX  "},
	OpCode{0, 1, OpArgU, OpArgU, IABC,  "LOADBOOL"},
	OpCode{0, 1, OpArgU, OpArgN, IABC,  "LOADNIL "},
	OpCode{0, 1, OpArgU, OpArgN, IABC,  "GETUPVAL"},
	OpCode{0, 1, OpArgU, OpArgK, IABC,  "GETTABUP"},
	OpCode{0, 1, OpArgR, OpArgK, IABC,  "GETTABLE"},
	OpCode{0, 0, OpArgK, OpArgK, IABC,  "SETTABUP"},
	OpCode{0, 0, OpArgU, OpArgN, IABC,  "SETUPVAL"},
	OpCode{0, 0, OpArgK, OpArgK, IABC,  "SETTABLE"},
	OpCode{0, 1, OpArgU, OpArgU, IABC,  "NEWTABLE"},
	OpCode{0, 1, OpArgR, OpArgK, IABC,  "SELF     "},
	OpCode{0, 1, OpArgK, OpArgK, IABC,  "ADD      "},
	OpCode{0, 1, OpArgK, OpArgK, IABC,  "SUB      "},
	OpCode{0, 1, OpArgK, OpArgK, IABC,  "MUL      "},
	OpCode{0, 1, OpArgK, OpArgK, IABC,  "MOD      "},
	OpCode{0, 1, OpArgK, OpArgK, IABC,  "POW      "},
	OpCode{0, 1, OpArgK, OpArgK, IABC,  "DIV      "},
	OpCode{0, 1, OpArgK, OpArgK, IABC,  "IDIV     "},
	OpCode{0, 1, OpArgK, OpArgK, IABC,  "BAND     "},
	OpCode{0, 1, OpArgK, OpArgK, IABC,  "BOR      "},
	OpCode{0, 1, OpArgK, OpArgK, IABC,  "BXOR     "},
	OpCode{0, 1, OpArgK, OpArgK, IABC,  "SHL      "},
	OpCode{0, 1, OpArgK, OpArgK, IABC,  "SHR      "},
	OpCode{0, 1, OpArgR, OpArgN, IABC,  "UNM      "},
	OpCode{0, 1, OpArgR, OpArgN, IABC,  "BNOT     "},
	OpCode{0, 1, OpArgR, OpArgN, IABC,  "NOT      "},
	OpCode{0, 1, OpArgR, OpArgN, IABC,  "LEN      "},
	OpCode{0, 1, OpArgR, OpArgR, IABC,  "CONCAT  "},
	OpCode{0, 0, OpArgR, OpArgN, IAsBx, "JMP      "},
	OpCode{1, 0, OpArgK, OpArgK, IABC,  "EQ       "},
	OpCode{1, 0, OpArgK, OpArgK, IABC,  "LT       "},
	OpCode{1, 0, OpArgK, OpArgK, IABC,  "LE       "},
	OpCode{1, 0, OpArgN, OpArgU, IABC,  "TEST     "},
	OpCode{1, 1, OpArgR, OpArgU, IABC,  "TESTSET "},
	OpCode{0, 1, OpArgU, OpArgU, IABC,  "CALL     "},
	OpCode{0, 1, OpArgU, OpArgU, IABC,  "TAILCALL"},
	OpCode{0, 0, OpArgU, OpArgN, IABC,  "RETURN  "},
	OpCode{0, 1, OpArgR, OpArgN, IAsBx, "FORLOOP "},
	OpCode{0, 1, OpArgR, OpArgN, IAsBx, "FORPREP "},
	OpCode{0, 0, OpArgN, OpArgU, IABC,  "TFORCALL"},
	OpCode{0, 1, OpArgR, OpArgN, IAsBx, "TFORLOOP"},
	OpCode{0, 0, OpArgU, OpArgU, IABC,  "SETLIST "},
	OpCode{0, 1, OpArgU, OpArgN, IABx,  "CLOSURE "},
	OpCode{0, 1, OpArgU, OpArgN, IABC,  "VARARG  "},
	OpCode{0, 0, OpArgU, OpArgU, IAx,    "EXTRAARG"},
};

#endif
