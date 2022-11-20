#include "binary_chunk.h"
#include "../cbuffer_stream.h"
#include "../cbuffer.h"
#include <assert.h>

static CBufferStream buffer_stream = NULL;

void BinaryChunkInit(CBuffer buffer) {
	assert(buffer_stream == NULL);
	buffer_stream = CBufferStreamFromCBuffer(buffer);
}

CBuffer BinaryChunkReadString() {
	assert(buffer_stream != NULL);
	//TODO Íê³É¶ÁÈ¡×Ö·û´®
}

bool BinaryChunkCheckHeader() {
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

		if (CBufferStreamReadInt32(buffer_stream) != LUAC_INT) {
			break;
		}

		if (CBufferStreamReadFloat(buffer_stream) != LUAC_NUM) {
			break;
		}
		return true;
	} while (0);
	return false;
}

Prototype* BinaryChunkReadProto() {
	assert(buffer_stream != NULL);
	Prototype* prototype = malloc(sizeof(Prototype));
	return prototype;
}
