#include <stdio.h>
#include "binary_chunk.h"
#include "cbuffer.h"
#include "cvector.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

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

void PrintCode(Prototype* proto) {
	CVector codes = proto->Code;
	int len = CVectorSize(codes);
	bool flag = CVectorSize(proto->LineInfo) > 0;
	for (int i = 0; i < len; ++i) {
		 uint32_t* val = CVectorGet(codes, i);
		 uint32_t* line = CVectorGet(proto->LineInfo, i);
		 if (flag) {
			 printf("\t%d\t[%d]\t0x%08x\n", i + 1, *line, *val);
		 }
		 else {
			 printf("\t%d\t[-]\t0x%08x\n", i + 1, *val);
		 }
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
		PrintConstant((ConstantType*)CVectorGet(constants, i), i);
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
			CBuffer upname = CVectorGet(upvalName, i);
			char buffer[1024] = { 0 };
			memcpy(buffer, CBufferData(upname), CBufferDataSize(upname));
			printf("\t%d\t%s\t%d\t%d\n", i, buffer, cur->Instack, cur->Idx);
		}
		else {
			printf("\t%d\t-\t%d\t%d\n", i, cur->Instack, cur->Idx);
		}
	}
}

int main(int argc, const char* const* argv) {
	const char* file_name = "luac.out";
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
