#ifndef __BINARY_CHUNK_H__
#define __BINARY_CHUNK_H__
#include <inttypes.h>
#include "../cbuffer.h"
#include <stdbool.h>

typedef struct {
	unsigned char Instack;
	unsigned char Idx;
} Upvalue;

typedef struct {
	CBuffer VarName;
	uint32_t StartPC;
	uint32_t EndPC;
} LocVar;

typedef struct {
	unsigned char signature[4];
	unsigned char version;
	unsigned char format;
	unsigned char luaCData[6];
	unsigned char cintSize;
	unsigned char sizetSize;
	unsigned char instructionSize;
	unsigned char luaIntegerSize;
	unsigned char luaNumberSize;
	int64_t luacInt;
	double luacNum;
} Header;

typedef struct Prototype {
	CBuffer Source;
	uint32_t LineDefined;
	uint32_t LastLineDefined;
	unsigned char NumParams;
	unsigned char IsVararg;
	unsigned char MaxStackSize;
	uint32_t* Code;
	void* Constants;
	Upvalue* Upvalues;
	struct Prototype* Protos;
	uint32_t* LineInfo;
	LocVar* LocVars;
	CBuffer* UpvalueNames;
} Prototype;

typedef struct {
	Header header;
	unsigned char sizeUpvalues;
	Prototype* mainFunc;
} BinaryChunk;

#define LUA_SIGNATURE "\x1bLua"
#define LUAC_VERSION 0x53
#define LUAC_FORMAT 0
#define LUAC_DATA "\x19\x93\r\n\x1a\n"
#define CINT_SIZE 4
#define CSIZET_SIZE 8
#define INSTRUCTION_SIZE 4
#define LUA_INTEGER_SIZE 8
#define LUA_NUMBER_SIZE 8
#define LUAC_INT 0x5678
#define LUAC_NUM 370.5f

#define TAG_NIL 0x00
#define TAG_BOOLEAN 0x01
#define TAG_NUMBER 0x03
#define TAG_INTEGER 0x13
#define TAG_SHORT_STR 0x04
#define TAG_LONG_STR 0x14

void BinaryChunkInit(CBuffer buffer);

CBuffer BinaryChunkReadString();

bool BinaryChunkCheckHead();

uint32_t* BinaryChunkReadCode();

void BinaryChunkReadConstant();

void* BinaryChunkReadConstants();

Upvalue* BinaryChunkReadUpvalues();

Prototype* BinaryChunkReadProtos();

uint32_t* BinaryChunkReadLineInfo();

LocVar* BinaryChunkReadLocVars();

CBuffer* BinaryChunkReadUpvalueNames();

Prototype* BinaryChunkReadProto();

#endif
