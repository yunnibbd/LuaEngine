#include "binary_chunk.h"
#include "../cbuffer_stream.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static CBufferStream buffer_stream;

static uint32_t* BinaryChunkReadCode();
static ConstantType BinaryChunkReadConstant();
static ConstantType* BinaryChunkReadConstants();
static Upvalue* BinaryChunkReadUpvalues();
static Prototype* BinaryChunkReadProtos();
static uint32_t* BinaryChunkReadLineInfo();
static LocVar* BinaryChunkReadLocVars();
static CBuffer* BinaryChunkReadUpvalueNames();

void BinaryChunkInit(CBuffer buffer) {
	assert(buffer_stream == NULL);
	buffer_stream = CBufferStreamFromCBuffer(buffer);
	if (!BinaryChunkCheckHead()) {
		printf("parse error");
		abort();
	}
}

CBuffer BinaryChunkReadString() {
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
	CBufferStreamRead(buffer_stream, buffer, (int)len - 1);

	return  buffer;
}

bool BinaryChunkCheckHead() {
	assert(buffer_stream != NULL);
	do {
		char buffer[10] = { 0 };
		int buffer_len = strlen(LUA_SIGNATURE);
		CBufferStreamRead(buffer_stream, buffer, buffer_len);
		if (strncmp(buffer, LUA_SIGNATURE, buffer_len) != 0) {
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != LUAC_VERSION) {
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != LUAC_FORMAT) {
			break;
		}

		buffer_len = strlen(LUAC_DATA);
		CBufferStreamRead(buffer_stream, buffer, buffer_len);
		if (strncmp(buffer, LUAC_DATA, buffer_len) != 0) {
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != CINT_SIZE) {
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != CSIZET_SIZE) {
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != INSTRUCTION_SIZE) {
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != LUA_INTEGER_SIZE) {
			break;
		}

		if (CBufferStreamReadUInt8(buffer_stream) != LUA_NUMBER_SIZE) {
			break;
		}

		if (CBufferStreamReadInt64(buffer_stream) != LUAC_INT) {
			break;
		}

		if (CBufferStreamReadDouble(buffer_stream) != LUAC_NUM) {
			break;
		}
		return true;
	} while (0);
	return false;
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

Prototype* BinaryChunkReadProto() {
	Prototype* proto = malloc(sizeof(Prototype));
	proto->Source = BinaryChunkReadString();
	proto->LineDefined = CBufferStreamReadUInt32(buffer_stream);
	proto->LastLineDefined = CBufferStreamReadUInt32(buffer_stream);
	proto->NumParams = CBufferStreamReadUInt8(buffer_stream);
	proto->IsVararg = CBufferStreamReadUInt8(buffer_stream);
	proto->MaxStackSize = CBufferStreamReadUInt8(buffer_stream);
	proto->Code = BinaryChunkReadCode();
	proto->Constants = BinaryChunkReadConstants();
	proto->Upvalues = BinaryChunkReadUpvalues();
	proto->Protos = BinaryChunkReadProtos();
	proto->LineInfo = BinaryChunkReadLineInfo();
	proto->LocVars = BinaryChunkReadLocVars();
	proto->UpvalueNames = BinaryChunkReadUpvalueNames();
	return proto;
}

static uint32_t* BinaryChunkReadCode() {
	uint32_t code_element_len = CBufferStreamReadUInt32(buffer_stream);
	//TODO ×¢ÒâÄÚ´æÊÍ·Å
	size_t code_len = sizeof(uint32_t) * code_element_len;
	uint32_t* code = malloc(code_len);
	CBufferStreamRead(buffer_stream, code, code_len);
	return code;
}

static ConstantType BinaryChunkReadConstant() {
	uint8_t tag = CBufferStreamReadUInt8(buffer_stream);
	ConstantType type;
	switch (tag){
	case TAG_NIL:
		type.tag_nil = 0;
		break;
	case TAG_BOOLEAN:
		type.tag_boolean = CBufferStreamReadUInt8(buffer_stream) != 0;
		break;
	case TAG_INTEGER:
		type.tag_integer = CBufferStreamReadUInt64(buffer_stream);
		break;
	case TAG_NUMBER:
		type.tag_number = CBufferStreamReadDouble(buffer_stream);
		break;
	case TAG_SHORT_STR:
	case TAG_LONG_STR:
		type.tag_str = BinaryChunkReadString();
		break;
	default:
		assert(0);
		break;
	}
	return type;
}

static ConstantType* BinaryChunkReadConstants() {
	uint32_t constant_element_len = CBufferStreamReadUInt32(buffer_stream);
	size_t constants_len = sizeof(ConstantType) * constant_element_len;
	ConstantType* constants = malloc(constants_len);
	for (size_t i = 0; i < constant_element_len; ++i) {
		ConstantType* cur = constants++;
		*cur = BinaryChunkReadConstant();
	}
	return constants;
}

static Upvalue* BinaryChunkReadUpvalues() {
	uint32_t upvalue_element_len = CBufferStreamReadUInt32(buffer_stream);
	size_t upvalue_len = sizeof(Upvalue) * upvalue_element_len;
	Upvalue* upvalues = malloc(upvalue_len);
	CBufferStreamRead(buffer_stream, upvalues, upvalue_len);
	return upvalues;
}

static Prototype* BinaryChunkReadProtos() {
	uint32_t protos_element_len = CBufferStreamReadUInt32(buffer_stream);
	size_t proto_len = sizeof(Prototype) * protos_element_len;
	Prototype* protos = malloc(proto_len);
	CBufferStreamRead(buffer_stream, protos, proto_len);
	return protos;
}

static uint32_t* BinaryChunkReadLineInfo() {
	uint32_t lineInfo_element_len = CBufferStreamReadUInt32(buffer_stream);
	size_t lineInfo_len = sizeof(uint32_t) * lineInfo_element_len;
	uint32_t* lineInfos = malloc(lineInfo_len);
	CBufferStreamRead(buffer_stream, lineInfos, lineInfo_len);
	return lineInfos;
}

static LocVar* BinaryChunkReadLocVars() {
	uint32_t locvar_element_len = CBufferStreamReadUInt32(buffer_stream);
	size_t locvar_len = sizeof(LocVar) * locvar_element_len;
	LocVar* locvars = malloc(locvar_len);
	for (size_t i = 0; i < locvar_len; ++i) {
		LocVar* cur = locvars++;
		cur->VarName = BinaryChunkReadString();
		cur->StartPC = CBufferStreamReadUInt32(buffer_stream);
		cur->EndPC = CBufferStreamReadUInt32(buffer_stream);
	}
	return locvars;
}

static CBuffer* BinaryChunkReadUpvalueNames() {
	uint32_t upvaluenames_element_size = CBufferStreamReadUInt32(buffer_stream);
	size_t upvaluenames_len = sizeof(CBuffer) * upvaluenames_element_size;
	CBuffer* upvaluenames = malloc(upvaluenames_len);
	for (size_t i = 0; i < upvaluenames_len; ++i) {
		CBuffer* cur = upvaluenames++;
		*cur = BinaryChunkReadString();
	}
	return upvaluenames;
}
