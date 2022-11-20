#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
	int size_;
	int data_size_;
	char* data_;
} *CBuffer, StructCBuffer;

CBuffer CBufferAlloc(int size) {
	CBuffer buffer = malloc(sizeof(StructCBuffer));
	buffer->size_ = size;
	buffer->data_ = malloc(sizeof(char) * size);
	buffer->data_size_ = 0;
	return buffer;
}

void CBufferFree(CBuffer buffer) {
	if (buffer != NULL) {
		if (buffer->data_ != NULL) {
			free(buffer->data_);
		}
		free(buffer);
		buffer = NULL;
	}
}

char* CBufferData(CBuffer buffer) {
	return buffer->data_;
}

int CBufferSize(CBuffer buffer) {
	return buffer->size_;
}

bool CBufferPush(CBuffer buffer, const char* data, int size) {
	if (buffer->data_size_ + size > buffer->size_) {
		return false;
	}
	memcpy(buffer->data_ + buffer->data_size_, data, size);
	buffer->data_size_ += size;
	return true;
}

void CBufferPop(CBuffer buffer, int size) {
	int remain_size = buffer->data_size_ - size;
	if (remain_size >= 0) {
		memcpy(buffer->data_, buffer->data_ + size, remain_size);
		buffer->data_size_ = remain_size;
	}
	else {
		buffer->data_size_ = 0;
	}
}

void CBufferClear(CBuffer buffer) {
	buffer->data_size_ = 0;
}