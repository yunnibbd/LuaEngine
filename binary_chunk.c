#include "binary_chunk.h"

bool CheckHeader(BufferStream buffer_stream) {
	do {
		char buffer[10] = { 0 };
		int buffer_len = strlen(LUA_SIGNATURE);
		BufferStreamRead(buffer_stream, buffer, buffer_len);
		if (strncmp(buffer, LUA_SIGNATURE, buffer_len) != 0) {
			break;
		}

		if (BufferStreamReadUInt8(buffer_stream) != LUAC_VERSION) {
			break;
		}

		if (BufferStreamReadUInt8(buffer_stream) != LUAC_FORMAT) {
			break;
		}

		buffer_len = strlen(LUAC_DATA);
		BufferStreamRead(buffer_stream, buffer, buffer_len);
		if (strncmp(buffer, LUAC_DATA, buffer_len) != 0) {
			break;
		}

		if (BufferStreamReadUInt8(buffer_stream) != CINT_SIZE) {
			break;
		}

		if (BufferStreamReadUInt8(buffer_stream) != CSIZET_SIZE) {
			break;
		}

		if (BufferStreamReadUInt8(buffer_stream) != INSTRUCTION_SIZE) {
			break;
		}

		if (BufferStreamReadUInt8(buffer_stream) != LUA_INTEGER_SIZE) {
			break;
		}

		if (BufferStreamReadUInt8(buffer_stream) != LUA_NUMBER_SIZE) {
			break;
		}

		if (BufferStreamReadInt32(buffer_stream) != LUAC_INT) {
			break;
		}

		if (BufferStreamReadFloat(buffer_stream) != LUAC_NUM) {
			break;
		}
		return true;
	} while (0);
	return false;
}

Prototype* ReadProto(BufferStream buffer_stream) {
	Prototype* prototype = malloc(sizeof(Prototype));
	prototype->Source = NULL;
	//prototype->LineDefined = 
	return prototype;
}
