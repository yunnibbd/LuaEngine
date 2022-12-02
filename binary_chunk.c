#include "binary_chunk.h"
#include "cbuffer_stream.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static CBufferStream buffer_stream;

static void BinaryChunkInit(CBuffer buffer);
static CBuffer BinaryChunkReadString();
static CVector BinaryChunkReadCode();
static ConstantType BinaryChunkReadConstant();
static CVector BinaryChunkReadConstants();
static CVector BinaryChunkReadUpvalues();
static CVector BinaryChunkReadProtos(CBuffer parent);
static CVector BinaryChunkReadLineInfos();
static CVector BinaryChunkReadLocVars();
static CVector BinaryChunkReadUpvalueNames();
static Prototype* BinaryChunkReadProto(CBuffer parent);


Prototype* BinaryChunkUnDump(CBuffer buffer) {
	BinaryChunkInit(buffer);
	if (!BinaryChunkCheckHead()) {
		exit(-1);
	}
	BinaryChunkReadByte();
	return BinaryChunkReadProto(NULL);
}

static void BinaryChunkInit(CBuffer buffer) {
	assert(buffer_stream == NULL);
	buffer_stream = CBufferStreamAllocFromCBuffer(buffer);
}

static CBuffer BinaryChunkReadString() {
	assert(buffer_stream != NULL);
	uint64_t len = CBufferStreamReadUInt8(buffer_stream);
	if (len == 0) {
		return NULL;
	}
	CBuffer buffer = NULL;
	if (len == 0xff) {
		len = CBufferStreamReadUInt64(buffer_stream);
	}

	buffer = CBufferAlloc((int)len - 1);
	CBufferStreamRead(buffer_stream, CBufferData(buffer), (int)len - 1);
	CBufferSetDataSize(buffer, len - 1);
	return  buffer;
}

bool BinaryChunkCheckHead() {
	assert(buffer_stream != NULL);
	do {
		char buffer[10] = { 0 };
		int buffer_len = strlen(LUA_SIGNATURE);
		CBufferStreamRead(buffer_stream, buffer, buffer_len);
		if (strncmp(buffer, LUA_SIGNATURE, buffer_len) != 0) {
			printf("LUA_SIGNATURE error");
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != LUAC_VERSION) {
			printf("LUAC_VERSION error");
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != LUAC_FORMAT) {
			printf("LUAC_FORMAT error");
			break;
		}

		buffer_len = strlen(LUAC_DATA);
		CBufferStreamRead(buffer_stream, buffer, buffer_len);
		if (strncmp(buffer, LUAC_DATA, buffer_len) != 0) {
			printf("LUAC_DATA error");
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != CINT_SIZE) {
			printf("CINT_SIZE error");
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != CSIZET_SIZE) {
			printf("CSIZET_SIZE error");
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != INSTRUCTION_SIZE) {
			printf("INSTRUCTION_SIZE error");
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != LUA_INTEGER_SIZE) {
			printf("LUA_INTEGER_SIZE error");
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != LUA_NUMBER_SIZE) {
			printf("LUA_NUMBER_SIZE error");
			break;
		}

		if (CBufferStreamReadInt64(buffer_stream) != LUAC_INT) {
			printf("LUAC_INT error");
			break;
		}

		if (CBufferStreamReadDouble(buffer_stream) != LUAC_NUM) {
			printf("LUAC_NUM error");
			break;
		}
		return true;
	} while (0);
	return false;
}


static void CodeFreeFunc(uint32_t* element) {
	free(element);
}

static void CodeCopyFunc(void* addr, uint32_t* element) {
	memcpy(addr, element, sizeof(uint32_t));
}

static CVector BinaryChunkReadCode() {
	uint32_t code_element_len = CBufferStreamReadUInt32(buffer_stream);
	CVector codes = CVectorAlloc(code_element_len, sizeof(uint32_t), CodeFreeFunc, NULL, CodeCopyFunc);
	uint32_t* temp;
	for (int i = 0; i < code_element_len; ++i) {
		temp = malloc(sizeof(uint32_t));
		*temp = CBufferStreamReadUInt32(buffer_stream);
		CVectorPushBack(codes, &temp);
	}
	return codes;
}

static ConstantType BinaryChunkReadConstant() {
	uint8_t tag = CBufferStreamReadUInt8(buffer_stream);
	ConstantType type;
	switch (tag) {
	case TAG_NIL:
		type.tag = CONSTANT_TAG_NIL;
		type.data.tag_nil = 0;
		break;
	case TAG_BOOLEAN:
		type.tag = CONSTANT_TAG_BOOLEAN;
		type.data.tag_boolean = (CBufferStreamReadUInt8(buffer_stream) != 0);
		break;
	case TAG_INTEGER:
		type.tag = CONSTANT_TAG_INTEGER;
		type.data.tag_integer = CBufferStreamReadUInt64(buffer_stream);
		break;
	case TAG_NUMBER:
		type.tag = CONSTANT_TAG_NUMBER;
		type.data.tag_number = CBufferStreamReadDouble(buffer_stream);
		break;
	case TAG_SHORT_STR:
	case TAG_LONG_STR:
		type.tag = CONSTANT_TAG_STR;
		type.data.tag_str = BinaryChunkReadString();
		break;
	default:
		assert(0);
		break;
	}
	return type;
}

static ConstantCopyFunc(void* addr, ConstantType* constant) {
	memcpy(addr, constant, sizeof(ConstantType));
}

static ConstantFreeFunc(ConstantType* constant) {
	switch (constant->tag) {
	case CONSTANT_TAG_STR:
		CBufferFree(constant->data.tag_str);
		break;
	}
	free(constant);
}

static CVector BinaryChunkReadConstants() {
	uint32_t constant_element_len = CBufferStreamReadUInt32(buffer_stream);
	CVector constants = CVectorAlloc(constant_element_len, sizeof(ConstantType), ConstantFreeFunc, NULL, ConstantCopyFunc);
	ConstantType* temp;
	for (size_t i = 0; i < constant_element_len; ++i) {
		temp = malloc(sizeof(ConstantType));
		*temp = BinaryChunkReadConstant();
		CVectorPushBack(constants, temp);
	}
	return constants;
}

static void UpvalueFreeFunc(Upvalue* constant) {
	free(constant);
}

static void UpvalueCopyFunc(void* addr, Upvalue* constant) {
	memcpy(addr, constant, sizeof(Upvalue));
}

static CVector BinaryChunkReadUpvalues() {
	uint32_t upvalue_element_len = CBufferStreamReadUInt32(buffer_stream);
	CVector upvalues = CVectorAlloc(upvalue_element_len, sizeof(Upvalue), UpvalueFreeFunc, NULL, UpvalueCopyFunc);
	Upvalue* temp;
	for (size_t i = 0; i < upvalue_element_len; ++i) {
		temp = malloc(sizeof(Upvalue));
		temp->Instack = BinaryChunkReadByte();
		temp->Idx = BinaryChunkReadByte();
		CVectorPushBack(upvalues, temp);
	}
	return upvalues;
}

static void ProtoTypeCopyFunc(void* addr, Prototype* val) {
	memcpy(addr, val, sizeof(Prototype*));
}

static CVector BinaryChunkReadProtos(CBuffer parent) {
	uint32_t protos_element_len = CBufferStreamReadUInt32(buffer_stream);
	if (protos_element_len == 0) {
		return NULL;
	}
	CVector protos = CVectorAlloc(protos_element_len, sizeof(Prototype), NULL, NULL, ProtoTypeCopyFunc);
	Prototype* temp;
	for (size_t i = 0; i < protos_element_len; ++i) {
		temp = BinaryChunkReadProto(parent);
		CVectorPushBack(protos, temp);
	}
	return protos;
}

static void LineInfosFreeFunc(uint32_t* val) {
	free(val);
}

static void LineInfosCopyFunc(void* addr, uint32_t* val) {
	memcpy(addr, val, sizeof(uint32_t));
}

static CVector BinaryChunkReadLineInfos() {
	uint32_t lineinfo_element_len = CBufferStreamReadUInt32(buffer_stream);
	CVector lineinfos = CVectorAlloc(lineinfo_element_len, sizeof(uint32_t), LineInfosFreeFunc, NULL, LineInfosCopyFunc);
	uint32_t* temp;
	for (size_t i = 0; i < lineinfo_element_len; ++i) {
		temp = malloc(sizeof(uint32_t));
		*temp = CBufferStreamReadUInt32(buffer_stream);
		CVectorPushBack(lineinfos, &temp);
	}
	return lineinfos;
}

static void LocVarsFreeFunc(LocVar* val) {
	CBufferFree(val->VarName);
	free(val);
}

static void LocVarsCopyFunc(void* addr, LocVar* val) {
	memcpy(addr, val, sizeof(LocVar));
}

static CVector BinaryChunkReadLocVars() {
	uint32_t locvar_element_len = CBufferStreamReadUInt32(buffer_stream);
	CVector locvars = CVectorAlloc(locvar_element_len, sizeof(LocVar), LocVarsFreeFunc, NULL, LocVarsCopyFunc);
	LocVar* locvar;
	for (size_t i = 0; i < locvar_element_len; ++i) {
		locvar = malloc(sizeof(LocVar));
		locvar->VarName = BinaryChunkReadString();
		locvar->StartPC = CBufferStreamReadUInt32(buffer_stream);
		locvar->EndPC = CBufferStreamReadUInt32(buffer_stream);
		CVectorPushBack(locvars, locvar);
	}
	return locvars;
}

static void UpvalueNamesFreeFunc(CBuffer* val) {
	CBufferFree(*val);
	free(val);
}

static void UpvalueNamesCopyFunc(void* addr, CBuffer* val) {
	memcpy(addr, val, sizeof(CBuffer));
}

static CVector BinaryChunkReadUpvalueNames() {
	uint32_t upvaluenames_element_len = CBufferStreamReadUInt32(buffer_stream);
	CVector upvaluenames = CVectorAlloc(upvaluenames_element_len, sizeof(CBuffer), UpvalueNamesFreeFunc, NULL, UpvalueNamesCopyFunc);
	CBuffer* temp;
	for (size_t i = 0; i < upvaluenames_element_len; ++i) {
		temp = malloc(sizeof(CBuffer*));
		*temp = BinaryChunkReadString();
		CVectorPushBack(upvaluenames, temp);
	}
	return upvaluenames;
}

static Prototype* BinaryChunkReadProto(CBuffer parent) {
	CBuffer source = BinaryChunkReadString();
	if (source == NULL) {
		source = parent;
	}
	Prototype* proto = malloc(sizeof(Prototype));
	proto->Source = source;
	proto->LineDefined = CBufferStreamReadUInt32(buffer_stream);
	proto->LastLineDefined = CBufferStreamReadUInt32(buffer_stream);
	proto->NumParams = CBufferStreamReadUInt8(buffer_stream);
	proto->IsVararg = CBufferStreamReadUInt8(buffer_stream);
	proto->MaxStackSize = CBufferStreamReadUInt8(buffer_stream);
	proto->Code = BinaryChunkReadCode();
	proto->Constants = BinaryChunkReadConstants();
	proto->Upvalues = BinaryChunkReadUpvalues();
	proto->Protos = BinaryChunkReadProtos(parent);
	proto->LineInfo = BinaryChunkReadLineInfos();
	proto->LocVars = BinaryChunkReadLocVars();
	proto->UpvalueNames = BinaryChunkReadUpvalueNames();
	return proto;
}

uint8_t BinaryChunkReadByte() {
	return CBufferStreamReadUInt8(buffer_stream);
}

uint64_t BinaryChunkReadLuaInteger() {
	return CBufferStreamReadUInt64(buffer_stream);
}

double BinaryChunkReadLuaNumber() {
	return CBufferStreamReadDouble(buffer_stream);
}
