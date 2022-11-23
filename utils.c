#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

char* LoadFileContent(const char* file_name) {
	FILE* fp = fopen(file_name, "rb");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		int file_len = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* data = malloc(file_len);
		fread(data, file_len, 1, fp);
		return data;
	}
	else {
		printf("file %s not exists\n", file_name);
		return NULL;
	}
}

CBuffer LoadFileContentToCBuffer(const char* file_name) {
	FILE* fp = fopen(file_name, "rb");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		int file_len = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* data = malloc(file_len);
		fread(data, file_len, 1, fp);
		return CBufferFromStr(data, file_len);
	}
	else {
		printf("file %s not exists\n", file_name);
		return NULL;
	}
}
