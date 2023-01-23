#ifndef __C_BUFFER_STREAM_H__
#define __C_BUFFER_STREAM_H__
#include <stdbool.h>
#include <inttypes.h>

typedef void* CBufferStream;
typedef void* CBuffer;

CBufferStream CBufferStreamAlloc(int size);

CBufferStream CBufferStreamAllocFromCBuffer(CBuffer buffer);

void CBufferStreamFree(CBufferStream buffer_stream);

char* CBufferStreamData(CBufferStream buffer_stream);

int CBufferStreamSize(CBufferStream buffer_stream);

bool CBufferStreamRead(CBufferStream buffer_stream, void* buffer, int buffer_size);

bool CBufferStreamPeep(CBufferStream buffer_stream, void* buffer, int buffer_size);

bool CBufferStreamReadToCBuffer(CBufferStream buffer_stream, CBuffer buffer);

bool CBufferStreamWrite(CBufferStream buffer_stream, void* buffer, int buffer_size);

int8_t CBufferStreamReadInt8(CBufferStream buffer_stream);

int16_t CBufferStreamReadInt16(CBufferStream buffer_stream);

int32_t CBufferStreamReadInt32(CBufferStream buffer_stream);

int64_t CBufferStreamReadInt64(CBufferStream buffer_stream);

uint8_t CBufferStreamReadUInt8(CBufferStream buffer_stream);

uint16_t CBufferStreamReadUInt16(CBufferStream buffer_stream);

uint32_t CBufferStreamReadUInt32(CBufferStream buffer_stream);

uint64_t CBufferStreamReadUInt64(CBufferStream buffer_stream);

float CBufferStreamReadFloat(CBufferStream buffer_stream);

double CBufferStreamReadDouble(CBufferStream buffer_stream);

bool CBufferStreamWriteInt8(CBufferStream buffer_stream, int8_t data);

bool CBufferStreamWriteInt16(CBufferStream buffer_stream, int16_t data);

bool CBufferStreamWriteInt32(CBufferStream buffer_stream, int32_t data);

bool CBufferStreamWriteInt64(CBufferStream buffer_stream, int64_t data);

bool CBufferStreamWriteUInt8(CBufferStream buffer_stream, uint8_t data);

bool CBufferStreamWriteUInt16(CBufferStream buffer_stream, uint16_t data);

bool CBufferStreamWriteUInt32(CBufferStream buffer_stream, uint32_t data);

bool CBufferStreamWriteUInt64(CBufferStream buffer_stream, uint64_t data);

bool CBufferStreamWriteFloat(CBufferStream buffer_stream, float data);

bool CBufferStreamWriteDouble(CBufferStream buffer_stream, double data);

#endif
