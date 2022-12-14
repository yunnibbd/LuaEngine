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

CBuffer CBufferFromCBuffer(CBuffer buffer) {
	CBuffer ret = malloc(sizeof(StructCBuffer));
	ret->size_ = buffer->size_;
	ret->data_ = buffer->data_;
	ret->data_size_ = buffer->data_size_;
	buffer->data_ = NULL;
	return ret;
}

CBuffer CBufferAllocFromCBuffer(CBuffer in_buffer) {
	CBuffer buffer = malloc(sizeof(StructCBuffer));
	buffer->size_ = in_buffer->size_;
	buffer->data_ = malloc(in_buffer->size_);
	buffer->data_size_ = in_buffer->data_size_;
	return buffer;
}

CBuffer CBufferFromStr(const char* data, int data_size) {
	CBuffer buffer = malloc(sizeof(StructCBuffer));
	buffer->size_ = data_size;
	buffer->data_ = data;
	buffer->data_size_ = data_size;
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

void CBufferCopy(CBuffer dst, CBuffer src) {
	dst->data_size_ = src->data_size_;
	dst->size_ = src->size_;
	dst->data_ = malloc(src->size_);
	memcpy(dst->data_, src->data_, src->data_size_);
}

char* CBufferData(CBuffer buffer) {
	return buffer->data_;
}

int CBufferSize(CBuffer buffer) {
	return buffer->size_;
}

void CBufferSetDataSize(CBuffer buffer, int size) {
	buffer->data_size_ = size;
}

int CBufferDataSize(CBuffer buffer) {
	return buffer->data_size_;
}

bool CBufferPush(CBuffer buffer, const char* data, int data_size) {
	if (buffer->data_size_ + data_size > buffer->size_) {
		return false;
	}
	memcpy(buffer->data_ + buffer->data_size_, data, data_size);
	buffer->data_size_ += data_size;
	return true;
}

void CBufferPop(CBuffer buffer, int size) {
	int remain_len = buffer->data_size_ - size;
	if (remain_len >= 0) {
		memcpy(buffer->data_, buffer->data_ + size, remain_len);
		buffer->data_size_ = remain_len;
	}
	else {
		buffer->data_size_ = 0;
	}
}

void CBufferClear(CBuffer buffer) {
	buffer->data_size_ = 0;
}
