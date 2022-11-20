#ifndef __BUFFER_STREAM_H__
#define __BUFFER_STREAM_H__
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void* BufferStream;

BufferStream BufferStreamAlloc(int size);

void BufferStreamFree(BufferStream buffer_stream);

char* BufferStreamData(BufferStream buffer_stream);

int BufferStreamLength(BufferStream buffer_stream);

bool BufferStreamRead(BufferStream buffer_stream, void* buffer, int buffer_len);

bool BufferStreamPeep(BufferStream buffer_stream, char* buffer, int buffer_len);

bool BufferStreamWrite(BufferStream buffer_stream, void* buffer, int buffer_len);

int8_t BufferStreamReadInt8(BufferStream buffer_stream);

int16_t BufferStreamReadInt16(BufferStream buffer_stream);

int32_t BufferStreamReadInt32(BufferStream buffer_stream);

int64_t BufferStreamReadInt64(BufferStream buffer_stream);

uint8_t BufferStreamReadUInt8(BufferStream buffer_stream);

uint16_t BufferStreamReadUInt16(BufferStream buffer_stream);

uint32_t BufferStreamReadUInt32(BufferStream buffer_stream);

uint64_t BufferStreamReadUInt64(BufferStream buffer_stream);

float BufferStreamReadFloat(BufferStream buffer_stream);

double BufferStreamReadDouble(BufferStream buffer_stream);

bool BufferStreamWriteInt8(BufferStream buffer_stream, int8_t data);

bool BufferStreamWriteInt16(BufferStream buffer_stream, int16_t data);

bool BufferStreamWriteInt32(BufferStream buffer_stream, int32_t data);

bool BufferStreamWriteInt64(BufferStream buffer_stream, int64_t data);

bool BufferStreamWriteUInt8(BufferStream buffer_stream, uint8_t data);

bool BufferStreamWriteUInt16(BufferStream buffer_stream, uint16_t data);

bool BufferStreamWriteUInt32(BufferStream buffer_stream, uint32_t data);

bool BufferStreamWriteUInt64(BufferStream buffer_stream, uint64_t data);

bool BufferStreamWriteFloat(BufferStream buffer_stream, float data);

bool BufferStreamWriteDouble(BufferStream buffer_stream, double data);

#endif
