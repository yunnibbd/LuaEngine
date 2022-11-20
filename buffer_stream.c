#include "buffer_stream.h"
#include <math.h>

struct BufferStream {
	int size_;
	char* data_;
	int write_pos_;
	int read_pos_;
};

#define BUFFER_STREAM_CAN_READ(pStream, len) (pStream->size_ - pStream->read_pos_ >= len)

#define BUFFER_STREAM_CAN_WRITE(pStream, len) (pStream->size_ - pStream->write_pos_ >= len)

#define BUFFER_STREAM_PUSH(pStream, len) (pStream->write_pos_ += len)

#define BUFFER_STREAM_POP(pStream, len) (pStream->read_pos_ += len)

BufferStream BufferStreamAlloc(int size){
	struct BufferStream* self = malloc(sizeof(struct BufferStream));
	memset(self, 0, sizeof(struct BufferStream));
	self->size_ = size;
	self->data_ = malloc(sizeof(char) * size);
	return (BufferStream)self;
}

void BufferStreamFree(BufferStream buffer_stream){
	if (buffer_stream != NULL) {
		free(buffer_stream);
	}
}

char* BufferStreamData(BufferStream buffer_stream) {
	struct BufferStream* self = (struct BufferStream*)buffer_stream;
	return self->data_;
}

int BufferStreamLength(BufferStream buffer_stream) {
	struct BufferStream* self = (struct BufferStream*)buffer_stream;
	return self->size_;
}

bool BufferStreamRead(BufferStream buffer_stream, void* buffer, int buffer_len) {
	struct BufferStream* self = (struct BufferStream*)buffer_stream;
	if (BUFFER_STREAM_CAN_READ(self, buffer_len)) {
		memcpy(buffer, self->data_ + self->read_pos_, buffer_len);
		BUFFER_STREAM_POP(self, buffer_len);
		return true;
	}
	return false;
}

bool BufferStreamPeep(BufferStream buffer_stream, char* buffer, int buffer_len) {
	struct BufferStream* self = (struct BufferStream*)buffer_stream;
	if (BUFFER_STREAM_CAN_READ(self, buffer_len)) {
		memcpy(buffer, self->data_ + self->read_pos_, buffer_len);
		return true;
	}
	return false;
}

bool BufferStreamWrite(BufferStream buffer_stream, void* buffer, int buffer_len) {
	struct BufferStream* self = (struct BufferStream*)buffer_stream;
	if (BUFFER_STREAM_CAN_WRITE(self, buffer_len)) {
		memcpy(self->data_ + self->write_pos_, buffer, buffer_len);
		BUFFER_STREAM_PUSH(self, buffer_len);
		return true;
	}
	return false;
}

int8_t BufferStreamReadInt8(BufferStream buffer_stream) {
	int8_t ret = 0;
	BufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

int16_t BufferStreamReadInt16(BufferStream buffer_stream) {
	int16_t ret = 0;
	BufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

int32_t BufferStreamReadInt32(BufferStream buffer_stream) {
	int32_t ret = 0;
	BufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

int64_t BufferStreamReadInt64(BufferStream buffer_stream) {
	int64_t ret = 0;
	BufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}


uint8_t BufferStreamReadUInt8(BufferStream buffer_stream) {
	uint8_t ret = 0;
	BufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

uint16_t BufferStreamReadUInt16(BufferStream buffer_stream) {
	uint16_t ret = 0;
	BufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

uint32_t BufferStreamReadUInt32(BufferStream buffer_stream) {
	uint32_t ret = 0;
	BufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

uint64_t BufferStreamReadUInt64(BufferStream buffer_stream) {
	uint64_t ret = 0;
	BufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

float BufferStreamReadFloat(BufferStream buffer_stream) {
	float ret = 0;
	BufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

double BufferStreamReadDouble(BufferStream buffer_stream) {
	double ret = 0;
	BufferStreamRead(buffer_stream, &ret, sizeof(ret));
	return ret;
}

bool BufferStreamWriteInt8(BufferStream buffer_stream, int8_t data) {
	return BufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool BufferStreamWriteInt16(BufferStream buffer_stream, int16_t data) {
	return BufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool BufferStreamWriteInt32(BufferStream buffer_stream, int32_t data) {
	return BufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool BufferStreamWriteInt64(BufferStream buffer_stream, int64_t data) {
	return BufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool BufferStreamWriteUInt8(BufferStream buffer_stream, uint8_t data) {
	return BufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool BufferStreamWriteUInt16(BufferStream buffer_stream, uint16_t data) {
	return BufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool BufferStreamWriteUInt32(BufferStream buffer_stream, uint32_t data) {
	return BufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool BufferStreamWriteUInt64(BufferStream buffer_stream, uint64_t data) {
	return BufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool BufferStreamWriteFloat(BufferStream buffer_stream, float data) {
	return BufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool BufferStreamWriteDouble(BufferStream buffer_stream, double data) {
	return BufferStreamWrite(buffer_stream, &data, sizeof(data));
}
