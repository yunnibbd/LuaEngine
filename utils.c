#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

#define OPENANDREAD(file_name, buffer, len)\
do {\
	FILE* fp = fopen(file_name, "rb");\
	if (fp == NULL) {\
		printf("file %s not exists!\n", file_name);\
		return NULL;\
	}\
	fseek(fp, 0, SEEK_END);\
	len = ftell(fp);\
	fseek(fp, 0, SEEK_SET);\
	buffer = malloc(len);\
	fread(buffer, len, 1, fp);\
	fclose(fp);\
} while (0)

char* LoadFileContent(const char* file_name) {
	char *buffer = NULL;
	int len = 0;
	OPENANDREAD(file_name, buffer, len);
	return buffer;
}

CBuffer LoadFromToCBuffer(const char* file_name) {
	char *buffer = NULL;
	int len = 0;
	OPENANDREAD(file_name, buffer, len);
	return CBufferFromStr(buffer, len);
}

