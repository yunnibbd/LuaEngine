#include <stdio.h>
#include "binary_chunk.h"
#include "instruction.h"
#include "cbuffer.h"
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
	switch (OpMode(instruction)){
		case IABC: {
			TABC tabc = ABC(instruction);
			printf("%d", tabc.a);
			if (BMode(instruction) != OpArgN) {
				if (tabc.b > 0xff) {
					printf(" %d", -1 - tabc.b & 0xff);
				}
				else {
					printf(" %d", tabc.b);
				}
			}
			
			if (CMode(instruction) != OpArgN) {
				if (tabc.c > 0xff) {
					printf(" %d", -1 - tabc.c & 0xff);
				}
				else {
					printf(" %d", tabc.c);
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
			else if(BMode(instruction) == OpArgU) {
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
			printf("%d", -1 - Ax(instruction));
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
			printf("\t%d\t[-]\t%s \t", i + 1, **line, OpName(instruction));
		}
		PrintOperands(instruction);
		printf("\n");
	}
}

void PrintConstant(ConstantType* constant, int i) {
	switch (constant->tag){
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
			printf("\t%d\t%d\n", i + 1, constant->data.tag_integer);
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

int main(int argc, const char* const* argv) {
	const char* file_name = "luac2.out";
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
	}
	return 0;
}
