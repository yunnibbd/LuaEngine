#include <stdio.h>
#include "binary_chunk.h"
#include "instruction.h"
#include "lua_state.h"
#include "cbuffer.h"
#include "lua_math.h"
#include "cvector.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

OpCode g_opcodes[47] = {
	/*     T  A     B       C    mode    name */
	{0, 1, OpArgR, OpArgN, IABC,  "MOVE    "},
	{0, 1, OpArgK, OpArgN, IABx,  "LOADK   "},
	{0, 1, OpArgN, OpArgN, IABx,  "LOADKX  "},
	{0, 1, OpArgU, OpArgU, IABC,  "LOADBOOL"},
	{0, 1, OpArgU, OpArgN, IABC,  "LOADNIL "},
	{0, 1, OpArgU, OpArgN, IABC,  "GETUPVAL"},
	{0, 1, OpArgU, OpArgK, IABC,  "GETTABUP"},
	{0, 1, OpArgR, OpArgK, IABC,  "GETTABLE"},
	{0, 0, OpArgK, OpArgK, IABC,  "SETTABUP"},
	{0, 0, OpArgU, OpArgN, IABC,  "SETUPVAL"},
	{0, 0, OpArgK, OpArgK, IABC,  "SETTABLE"},
	{0, 1, OpArgU, OpArgU, IABC,  "NEWTABLE"},
	{0, 1, OpArgR, OpArgK, IABC,  "SELF    "},
	{0, 1, OpArgK, OpArgK, IABC,  "ADD     "},
	{0, 1, OpArgK, OpArgK, IABC,  "SUB     "},
	{0, 1, OpArgK, OpArgK, IABC,  "MUL     "},
	{0, 1, OpArgK, OpArgK, IABC,  "MOD     "},
	{0, 1, OpArgK, OpArgK, IABC,  "POW     "},
	{0, 1, OpArgK, OpArgK, IABC,  "DIV     "},
	{0, 1, OpArgK, OpArgK, IABC,  "IDIV    "},
	{0, 1, OpArgK, OpArgK, IABC,  "BAND    "},
	{0, 1, OpArgK, OpArgK, IABC,  "BOR     "},
	{0, 1, OpArgK, OpArgK, IABC,  "BXOR    "},
	{0, 1, OpArgK, OpArgK, IABC,  "SHL     "},
	{0, 1, OpArgK, OpArgK, IABC,  "SHR     "},
	{0, 1, OpArgR, OpArgN, IABC,  "UNM     "},
	{0, 1, OpArgR, OpArgN, IABC,  "BNOT    "},
	{0, 1, OpArgR, OpArgN, IABC,  "NOT     "},
	{0, 1, OpArgR, OpArgN, IABC,  "LEN     "},
	{0, 1, OpArgR, OpArgR, IABC,  "CONCAT  "},
	{0, 0, OpArgR, OpArgN, IAsBx, "JMP     "},
	{1, 0, OpArgK, OpArgK, IABC,  "EQ      "},
	{1, 0, OpArgK, OpArgK, IABC,  "LT      "},
	{1, 0, OpArgK, OpArgK, IABC,  "LE      "},
	{1, 0, OpArgN, OpArgU, IABC,  "TEST    "},
	{1, 1, OpArgR, OpArgU, IABC,  "TESTSET "},
	{0, 1, OpArgU, OpArgU, IABC,  "CALL    "},
	{0, 1, OpArgU, OpArgU, IABC,  "TAILCALL"},
	{0, 0, OpArgU, OpArgN, IABC,  "RETURN  "},
	{0, 1, OpArgR, OpArgN, IAsBx, "FORLOOP "},
	{0, 1, OpArgR, OpArgN, IAsBx, "FORPREP "},
	{0, 0, OpArgN, OpArgU, IABC,  "TFORCALL"},
	{0, 1, OpArgR, OpArgN, IAsBx, "TFORLOOP"},
	{0, 0, OpArgU, OpArgU, IABC,  "SETLIST "},
	{0, 1, OpArgU, OpArgN, IABx,  "CLOSURE "},
	{0, 1, OpArgU, OpArgN, IABC,  "VARARG  "},
	{0, 0, OpArgU, OpArgU, IAx,   "EXTRAARG"},
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
	switch (OpMode(instruction)) {
	case IABC: {
		TABC tabc = ABC(instruction);
		printf("%d", tabc.a);
		if (CMode(instruction) != OpArgN) {
			if (tabc.c > 0xff) {
				printf(" %d", -1 - (tabc.c & 0xff));
			}
			else {
				printf(" %d", tabc.c);
			}
		}
		if (BMode(instruction) != OpArgN) {
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
		TABx tabx = ABx(instruction);
		printf("%d", tabx.a);
		if (BMode(instruction) == OpArgK) {
			printf(" %d", -1 - tabx.bx);
		}
		else {
			printf(" %d", tabx.bx);
		}
		break;
	}
	case IAsBx: {
		TAsBx tasbx = AsBx(instruction);
		printf("%d %d", tasbx.a, tasbx.sbx);
		break;
	}
	case IAx: {
		int ax = Ax(instruction);
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
			printf("\t%d\t[%d]\t%s \t", i + 1, **line, OpName(instruction));
		}
		else {
			printf("\t%d\t[-]\t%s \t", i + 1, OpName(instruction));
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

int main(int argc, const char* const* argv) {
	/*const char* file_name = "luac2.out";
	if (argc > 1) {
		file_name = argv[1];
	}
	if (file_name != NULL) {
		CBuffer buffer = LoadFromToCBuffer(file_name);
		Prototype* proto = BinaryChunkUnDump(buffer);
		PrintHeader(proto);
		PrintCode(proto);
		PrintDetail(proto);
		printf("\nhello yunni\n");
	}*/

	//const char* greet = "hello yunni";


	//LuaState lua_state = LuaStateAlloc();
	//LuaStatePushBoolean(lua_state, true);								 PrintStack(lua_state);
	///*LuaStatePrint(lua_state);
	//printf("======================================\n");*/
	//LuaStatePushInteger(lua_state, 10);									 PrintStack(lua_state);
	///*LuaStatePrint(lua_state);
	//printf("======================================\n");*/
	//LuaStatePushNil(lua_state);											 PrintStack(lua_state);
	///*LuaStatePrint(lua_state);
	//printf("======================================\n");*/
	//LuaStatePushString(lua_state, CBufferFromStr(greet, strlen(greet))); PrintStack(lua_state);
	///*LuaStatePrint(lua_state);
	//printf("======================================\n");*/
	//LuaStatePushValue(lua_state, -4);									 PrintStack(lua_state);
	///*LuaStatePrint(lua_state);
	//printf("======================================\n");*/
	//LuaStateReplace(lua_state, 3);										 PrintStack(lua_state);
	///*LuaStatePrint(lua_state);
	//printf("======================================\n");*/
	//LuaStateSetTop(lua_state, 6);										 PrintStack(lua_state);
	///*LuaStatePrint(lua_state);
	//printf("======================================\n");*/
	//LuaStateRemove(lua_state, -3);										 PrintStack(lua_state);
	///*LuaStatePrint(lua_state);
	//printf("======================================\n");*/
	//LuaStateSetTop(lua_state, -5);										 PrintStack(lua_state);
	///*LuaStatePrint(lua_state);
	//printf("======================================\n");*/
	//LuaStateFree(lua_state);

	printf("hello yunni\n");
	LuaState lua_state = LuaStateAlloc();

	LuaStatePushInteger(lua_state, 1);
	LuaStatePushString(lua_state, CBufferFromStr("2.0", 3));
	LuaStatePushString(lua_state, CBufferFromStr("3.0", 3));
	LuaStatePushNumber(lua_state, 4.0);
	PrintStack(lua_state);

	LuaStateArith(lua_state, LUA_OPADD);  PrintStack(lua_state);
	LuaStateArith(lua_state, LUA_OPBNOT); PrintStack(lua_state);
	LuaStateLen(lua_state, 2);	PrintStack(lua_state);
	LuaStateConcat(lua_state, 3); PrintStack(lua_state);
	LuaStatePushBoolean(lua_state, LuaStateCompare(lua_state, 1, 2, LUA_OPEQ));
	PrintStack(lua_state);

	LuaStateFree(lua_state);
	return 0;
}
