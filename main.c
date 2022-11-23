#include <stdio.h>
#include "./BinChunk/binary_chunk.h"
#include "utils.h"

void PrintHeader(Prototype* proto) {
	const char* func_type = "main";
	if (proto->LineDefined > 0) {
		func_type = "function";
	}

	const char* vararg_flag = "";
	if (proto->IsVararg > 0) {
		vararg_flag = "+";
	}

	printf("\n%s <%s:%d,%d> (%d instructions)\n", func_type, proto->Source, proto->LineDefined, proto->LastLineDefined, proto->MaxStackSize);
}

void PrintCode(Prototype* proto) {

}

void PrintDetail(Prototype* proto) {

}

int main(int argc, const char* const* argv) {
	//if (argc > 1) {
		//const char* file_name = argv[1];
		const char* file_name = "luac.out";
		CBuffer buffer = LoadFileContentToCBuffer(file_name);
		if (buffer != NULL) {
			BinaryChunkInit(buffer);
			BinaryChunkReadByte();
			Prototype* proto = BinaryChunkReadProto();
			PrintHeader(proto);
			printf("hello yunni\n");
		}
	//}
	return 0;
}
