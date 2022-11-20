#include <stdio.h>
#include "binary_chunk.h"
#include "buffer_stream.h"

int main() {
	BufferStream buffer_stream = BufferStreamAlloc(1024);
	BufferStreamWrite(buffer_stream, LUA_SIGNATURE, strlen(LUA_SIGNATURE));
	BufferStreamWriteUInt8(buffer_stream, LUAC_VERSION);
	CheckHeader(buffer_stream);
	return 0;
}
