#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

typedef struct {
	int size_;
	char* data_;
	int write_pos_;
	int read_pos_;
} *CBufferStream, StructCBufferStream;

typedef struct {
	int size_;
	int data_size_;
	char* data_;
} *CBuffer, StructCBuffer;

#define BUFFER_STREAM_CAN_READ(pStream, len) (pStream->size_ - pStream->read_pos_ >= len)

#define BUFFER_STREAM_CAN_WRITE(pStream, len) (pStream->size_ - pStream->write_pos_ >= len)

#define BUFFER_STREAM_PUSH(pStream, len) (pStream->write_pos_ += len)

#define BUFFER_STREAM_POP(pStream, len) (pStream->read_pos_ += len)

CBufferStream CBufferStreamAlloc(int size) {
	CBufferStream buffer_stream = malloc(sizeof(StructCBufferStream));
	buffer_stream->size_ = size;
	buffer_stream->data_ = malloc(sizeof(char) * size);
	buffer_stream->write_pos_ = 0;
	buffer_stream->read_pos_ = 0;
	return buffer_stream;
}

CBufferStream CBufferStreamAllocFromCBuffer(CBuffer buffer) {
	CBufferStream buffer_stream = malloc(sizeof(StructCBufferStream));
	buffer_stream->size_ = buffer->size_;
	buffer_stream->data_ = buffer->data_;
	buffer_stream->write_pos_ = 0;
	buffer_stream->read_pos_ = 0;
	buffer_stream->write_pos_ = buffer->data_size_;
	buffer->data_ = NULL;
	return buffer_stream;
}

void CBufferStreamFree(CBufferStream buffer_stream) {
	if (buffer_stream != NULL) {
		if (buffer_stream->data_ != NULL) {
			free(buffer_stream->data_);
		}
		free(buffer_stream);
		buffer_stream = NULL;
	}
}

char* CBufferStreamData(CBufferStream buffer_stream) {
	return buffer_stream->data_;
}

int CBufferStreamSize(CBufferStream buffer_stream) {
	return buffer_stream->size_;
}

bool CBufferStreamRead(CBufferStream buffer_stream, void* buffer, int buffer_size) {
	if (BUFFER_STREAM_CAN_READ(buffer_stream, buffer_size)) {
		memcpy(buffer, buffer_stream->data_ + buffer_stream->read_pos_, buffer_size);
		BUFFER_STREAM_POP(buffer_stream, buffer_size);
		return true;
	}
	return false;
}

bool CBufferStreamPeep(CBufferStream buffer_stream, void* buffer, int buffer_size) {
	if (BUFFER_STREAM_CAN_READ(buffer_stream, buffer_size)) {
		memcpy(buffer_stream->data_ + buffer_stream->read_pos_, buffer, buffer_size);
		return true;
	}
	return false;
}

bool CBufferStreamReadToCBuffer(CBufferStream buffer_stream, CBuffer buffer) {
	return CBufferStreamRead(buffer_stream, buffer->data_, buffer->size_);
}

bool CBufferStreamWrite(CBufferStream buffer_stream, void* buffer, int buffer_size) {
	if (BUFFER_STREAM_CAN_WRITE(buffer_stream, buffer_size)) {
		memcpy(buffer_stream->data_ + buffer_stream->write_pos_, buffer, buffer_size);
		BUFFER_STREAM_PUSH(buffer_stream, buffer_size);
		return true;
	}
	return false;
}

int8_t CBufferStreamReadInt8(CBufferStream buffer_stream){
	int8_t ret = 0;
	CBufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

int16_t CBufferStreamReadInt16(CBufferStream buffer_stream) {
	int16_t ret = 0;
	CBufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

int32_t CBufferStreamReadInt32(CBufferStream buffer_stream) {
	int32_t ret = 0;
	CBufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

int64_t CBufferStreamReadInt64(CBufferStream buffer_stream) {
	int64_t ret = 0;
	CBufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

uint8_t CBufferStreamReadUInt8(CBufferStream buffer_stream) {
	uint8_t ret = 0;
	CBufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

uint16_t CBufferStreamReadUInt16(CBufferStream buffer_stream) {
	uint16_t ret = 0;
	CBufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

uint32_t CBufferStreamReadUInt32(CBufferStream buffer_stream) {
	uint32_t ret = 0;
	CBufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

uint64_t CBufferStreamReadUInt64(CBufferStream buffer_stream) {
	uint64_t ret = 0;
	CBufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

float CBufferStreamReadFloat(CBufferStream buffer_stream) {
	float ret = 0;
	CBufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

double CBufferStreamReadDouble(CBufferStream buffer_stream) {
	double ret = 0;
	CBufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

bool CBufferStreamWriteInt8(CBufferStream buffer_stream, int8_t data) {
	return CBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool CBufferStreamWriteInt16(CBufferStream buffer_stream, int16_t data) {
	return CBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool CBufferStreamWriteInt32(CBufferStream buffer_stream, int32_t data) {
	return CBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool CBufferStreamWriteInt64(CBufferStream buffer_stream, int64_t data) {
	return CBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool CBufferStreamWriteUInt8(CBufferStream buffer_stream, uint8_t data) {
	return CBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool CBufferStreamWriteUInt16(CBufferStream buffer_stream, uint16_t data) {
	return CBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool CBufferStreamWriteUInt32(CBufferStream buffer_stream, uint32_t data) {
	return CBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool CBufferStreamWriteUInt64(CBufferStream buffer_stream, uint64_t data) {
	return CBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool CBufferStreamWriteFloat(CBufferStream buffer_stream, float data) {
	return CBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool CBufferStreamWriteDouble(CBufferStream buffer_stream, double data) {
	return CBufferStreamWrite(buffer_stream, &data, sizeof(data));
}
