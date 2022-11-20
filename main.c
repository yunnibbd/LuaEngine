#include <stdio.h>
#include "./BinChunk/binary_chunk.h"
#include "cbuffer.h"
#include <string.h>
#include <windows.h>

int main() {
	CBuffer buffer = CBufferAlloc(1024);
	CBufferPush(buffer, LUA_SIGNATURE, strlen(LUA_SIGNATURE));
	unsigned char luac_version = LUAC_VERSION;
	CBufferPush(buffer, &luac_version, sizeof(LUAC_VERSION));

	BinaryChunkInit(buffer);
	BinaryChunkCheckHeader();

	CBufferFree(buffer);
	system("pause");
	return 0;
}
