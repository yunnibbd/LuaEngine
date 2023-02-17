#include "binary_chunk.h"
#include "instruction.h"
#include "lua_state.h"
#include "cbuffer.h"
#include "lua_math.h"
#include "lua_vm.h"
#include "lua_value.h"
#include "cvector.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

OpCode g_opcodes[47] = {
	/*     T  A     B       C    mode    name */
	{0, 1, OpArgR, OpArgN, IABC,  "MOVE    ", LuaVMMove},
	{0, 1, OpArgK, OpArgN, IABx,  "LOADK   ", LuaVMLoadK},
	{0, 1, OpArgN, OpArgN, IABx,  "LOADKX  ", LuaVMLoadKx},
	{0, 1, OpArgU, OpArgU, IABC,  "LOADBOOL", LuaVMLoadBool},
	{0, 1, OpArgU, OpArgN, IABC,  "LOADNIL ", LuaVMLoadNil},
	{0, 1, OpArgU, OpArgN, IABC,  "GETUPVAL", NULL},
	{0, 1, OpArgU, OpArgK, IABC,  "GETTABUP", NULL},
	{0, 1, OpArgR, OpArgK, IABC,  "GETTABLE", NULL},
	{0, 0, OpArgK, OpArgK, IABC,  "SETTABUP", NULL},
	{0, 0, OpArgU, OpArgN, IABC,  "SETUPVAL", NULL},
	{0, 0, OpArgK, OpArgK, IABC,  "SETTABLE", NULL},
	{0, 1, OpArgU, OpArgU, IABC,  "NEWTABLE", NULL},
	{0, 1, OpArgR, OpArgK, IABC,  "SELF    ", NULL},
	{0, 1, OpArgK, OpArgK, IABC,  "ADD     ", LuaVMAdd},
	{0, 1, OpArgK, OpArgK, IABC,  "SUB     ", LuaVMSub},
	{0, 1, OpArgK, OpArgK, IABC,  "MUL     ", LuaVMMul},
	{0, 1, OpArgK, OpArgK, IABC,  "MOD     ", LuaVMMod},
	{0, 1, OpArgK, OpArgK, IABC,  "POW     ", LuaVMPow},
	{0, 1, OpArgK, OpArgK, IABC,  "DIV     ", LuaVMDiv},
	{0, 1, OpArgK, OpArgK, IABC,  "IDIV    ", LuaVMIDiv},
	{0, 1, OpArgK, OpArgK, IABC,  "BAND    ", LuaVMBand},
	{0, 1, OpArgK, OpArgK, IABC,  "BOR     ", LuaVMBor},
	{0, 1, OpArgK, OpArgK, IABC,  "BXOR    ", LuaVMBxor},
	{0, 1, OpArgK, OpArgK, IABC,  "SHL     ", LuaVMShl},
	{0, 1, OpArgK, OpArgK, IABC,  "SHR     ", LuaVMShr},
	{0, 1, OpArgR, OpArgN, IABC,  "UNM     ", LuaVMUnm},
	{0, 1, OpArgR, OpArgN, IABC,  "BNOT    ", LuaVMBnot},
	{0, 1, OpArgR, OpArgN, IABC,  "NOT     ", LuaVMNot},
	{0, 1, OpArgR, OpArgN, IABC,  "LEN     ", LuaVMlength},
	{0, 1, OpArgR, OpArgR, IABC,  "CONCAT  ", LuaVMConcat},
	{0, 0, OpArgR, OpArgN, IAsBx, "JMP     ", LuaVMJmp},
	{1, 0, OpArgK, OpArgK, IABC,  "EQ      ", LuaVMEq},
	{1, 0, OpArgK, OpArgK, IABC,  "LT      ", LuaVMLt},
	{1, 0, OpArgK, OpArgK, IABC,  "LE      ", LuaVMLe},
	{1, 0, OpArgN, OpArgU, IABC,  "TEST    ", LuaVMTest},
	{1, 1, OpArgR, OpArgU, IABC,  "TESTSET ", LuaVMTestSet},
	{0, 1, OpArgU, OpArgU, IABC,  "CALL    ", NULL},
	{0, 1, OpArgU, OpArgU, IABC,  "TAILCALL", NULL},
	{0, 0, OpArgU, OpArgN, IABC,  "RETURN  ", NULL},
	{0, 1, OpArgR, OpArgN, IAsBx, "FORLOOP ", LuaVMForLoop},
	{0, 1, OpArgR, OpArgN, IAsBx, "FORPREP ", LuaVMForPrep},
	{0, 0, OpArgN, OpArgU, IABC,  "TFORCALL", NULL},
	{0, 1, OpArgR, OpArgN, IAsBx, "TFORLOOP", NULL},
	{0, 0, OpArgU, OpArgU, IABC,  "SETLIST ", NULL},
	{0, 1, OpArgU, OpArgN, IABx,  "CLOSURE ", NULL},
	{0, 1, OpArgU, OpArgN, IABC,  "VARARG  ", NULL},
	{0, 0, OpArgU, OpArgU, IAx,   "EXTRAARG", NULL},
};

Operator g_operators[14] = {
	{lua_iadd, lua_fadd},
	{lua_isub, lua_fsub},
	{lua_imul, lua_fmul},
	{lua_imod, lua_fmod},
	{NULL, lua_pow},
	{NULL, lua_div},
	{lua_iidiv, lua_fidiv},
	{lua_band, NULL},
	{lua_bor, NULL},
	{lua_bxor, NULL},
	{lua_shl, NULL},
	{lua_shr, NULL},
	{lua_iunm, lua_funm},
	{lua_bnot, NULL}
};

void PrintHeader(Prototype* proto) {
	const char* func_type = "main";
	if (proto->LineDefined > 0) {
		func_type = "function";
	}
	const char * vararg_flag = "";
	if (proto->IsVararg > 0) {
		vararg_flag = "+";
	}

	char buffer[1024] = { 0 };
	memcpy(buffer, CBufferData(proto->Source), CBufferSize(proto->Source));
	printf("\n%s <%s:%d, %d> (%d instruction)\n", func_type, buffer, proto->LineDefined, proto->LastLineDefined, CVectorSize(proto->Code));
	printf("%d%s params, %d slots, %d upvalues, ", proto->NumParams, vararg_flag, proto->MaxStackSize, CVectorSize(proto->Upvalues));
	printf("%d locals, %d constants, %d functions\n", CVectorSize(proto->LocVars), CVectorSize(proto->Constants), CVectorSize(proto->Protos));
}

void PrintOperands(Instruction instruction) {
	switch (InstructionOpMode(instruction)) {
		case IABC: {
			TABC tabc = InstructionABC(instruction);
			printf("%d", tabc.a);
			if (InstructionCMode(instruction) != OpArgN) {
				if (tabc.c > 0xff) {
					printf(" %d", -1 - (tabc.c & 0xff));
				}
				else {
					printf(" %d", tabc.c);
				}
			}
			if (InstructionBMode(instruction) != OpArgN) {
				if (tabc.b > 0xff) {
					printf(" %d", -1 - (tabc.b & 0xff));
				}
				else {
					printf(" %d", tabc.b);
				}
			}
			break;
		}
		case IABx: {
			TABx tabx = InstructionABx(instruction);
			printf("%d", tabx.a);
			if (InstructionBMode(instruction) == OpArgK) {
				printf(" %d", -1 - tabx.bx);
			}
			else {
				printf(" %d", tabx.bx);
			}
			break;
		}
		case IAsBx: {
			TAsBx tasbx = InstructionAsBx(instruction);
			printf("%d %d", tasbx.a, tasbx.sbx);
			break;
		}
		case IAx: {
			int ax = InstructionAx(instruction);
			printf("%d", ax);
			break;
		}
	}
}

void PrintCode(Prototype* proto) {
	CVector codes = proto->Code;
	int len = CVectorSize(codes);
	bool flag = CVectorSize(proto->LineInfo) > 0;
	for (int i = 0; i < len; ++i) {
		uint32_t** val = CVectorGet(codes, i);
		uint32_t** line = CVectorGet(proto->LineInfo, i);
		Instruction instruction = **val;
		if (flag) {
			printf("\t%d\t[%d]\t%s \t", i + 1, **line, InstructionOpName(instruction));
		}
		else {
			printf("\t%d\t[-]\t%s \t", i + 1, InstructionOpName(instruction));
		}
		PrintOperands(instruction);
		printf("\n");
	}
}

void PrintConstant(ConstantType* constant, int i) {
	switch (constant->tag) {
		case CONSTANT_TAG_NIL:
			printf("\t%d\t%s\n", i + 1, "nil");
			break;
		case CONSTANT_TAG_BOOLEAN:
			printf("\t%d\t%s\n", i + 1, constant->data.tag_boolean == 0 ? "false" : "true");
			break;
		case CONSTANT_TAG_NUMBER:
			printf("\t%d\t%lf\n", i + 1, constant->data.tag_number);
			break;
		case CONSTANT_TAG_INTEGER:
			printf("\t%d\t%lld\n", i + 1, constant->data.tag_integer);
			break;
		case CONSTANT_TAG_STR: {
			char buffer[1024] = { 0 };
			memcpy(buffer, CBufferData(constant->data.tag_str), CBufferDataSize(constant->data.tag_str));
			printf("\t%d\t%s\n", i + 1, buffer);
			break;
		}
	}
}

void PrintDetail(Prototype* proto) {
	CVector constants = proto->Constants;
	printf("constants (%d):\n", CVectorSize(constants));
	int len = CVectorSize(constants);
	for (int i = 0; i < len; ++i) {
		PrintConstant(*(ConstantType**)CVectorGet(constants, i), i);
	}

	CVector locvars = proto->LocVars;
	len = CVectorSize(locvars);
	printf("locals (%d):\n", len);
	for (int i = 0; i < len; ++i) {
		LocVar* cur = CVectorGet(locvars, i);
		char buffer[1024] = { 0 };
		memcpy(buffer, CBufferData(cur->VarName), CBufferDataSize(cur->VarName));
		printf("\t%d\t%s\t%d\t%d\n", i, buffer, cur->StartPC + 1, cur->EndPC + 1);
	}

	CVector upval = proto->Upvalues;
	CVector upvalName = proto->UpvalueNames;
	len = CVectorSize(upval);
	printf("upvalues (%d):\n", len);

	bool flag = CVectorSize(upvalName) > 0;
	for (int i = 0; i < len; ++i) {
		Upvalue* cur = CVectorGet(upval, i);
		if (flag) {
			CBuffer** upname = CVectorGet(upvalName, i);
			char buffer[1024] = { 0 };
			memcpy(buffer, CBufferData(*upname), CBufferDataSize(*upname));
			printf("\t%d\t%s\t%d\t%d\n", i, buffer, cur->Instack, cur->Idx);
		}
		else {
			printf("\t%d\t-\t%d\t%d\n", i, cur->Instack, cur->Idx);
		}
	}
}

void PrintStack(LuaState lua_state) {
	int top = LuaStateGetTop(lua_state);
	for (int i = 1; i <= top; ++i) {
		LuaType type = LuaStateType(lua_state, i);
		switch (type) {
			case LUA_TBOOLEAN:
				printf("[%s]", LuaStateToBoolean(lua_state, i) == 0 ? "false" : "true");
				break;
			case LUA_TNUMBER:
				printf("[%lf]", LuaStateToNumber(lua_state, i));
				break;
			case LUA_TINTEGER:
				printf("[%lld]", LuaStateToInteger(lua_state, i));
				break;
			case LUA_TSTRING: {
				CBuffer str = LuaStateToString(lua_state, i);
				char buffer[1024] = { 0 };
				memcpy(buffer, CBufferData(str), CBufferDataSize(str));
				printf("[%s]", buffer);
				break;
			}
			default:
				printf("[%s]", LuaStateTypeName(lua_state, type));
				break;
		}

	}
	printf("\n");
}

void LuaMain(Prototype* proto) {
	int nRegs = proto->MaxStackSize;
	LuaState lua_state = LuaStateAlloc(nRegs + 8, proto);
	LuaStateSetTop(lua_state, nRegs);
	for (;;) {
		int pc = LuaStatePC(lua_state);
		Instruction inst = LuaStateFetch(lua_state);
		if (InstructionOpcode(inst) != OP_RETURN) {
			InstructionExecute(inst, lua_state);
			printf("[%02d] %s", pc + 1, InstructionOpName(inst));

			PrintStack(lua_state);
		}
		else {
			break;
		}
	}
}

int main(int argc, const char* const* argv) {
	printf("hello yunni\n");
	const char* file_name = "luac.out";
	if (argc >= 2) {
		file_name = argv[1];
	}
	CBuffer file_contont = LoadFileToCBuffer(file_name);
	Prototype* proto = BinaryChunkUnDump(file_contont);
	LuaMain(proto);
	return 0;
}
