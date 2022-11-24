#ifndef __C_BUFFER_H__
#define __C_BUFFER_H__
#include <stdbool.h>

typedef void* CBuffer;

CBuffer CBufferAlloc(int size);

CBuffer CBufferFromCBuffer(CBuffer buffer);

CBuffer CBufferAllocFromCBuffer(CBuffer buffer);

CBuffer CBufferFromStr(const char* data, int data_size);

void CBufferFree(CBuffer buffer);

void CBufferCopy(CBuffer dst, CBuffer src);

char* CBufferData(CBuffer buffer);

int CBufferSize(CBuffer buffer);

void CBufferSetDataSize(CBuffer buffer, int size);

int CBufferDataSize(CBuffer buffer);

bool CBufferPush(CBuffer buffer, const char* data, int data_size);

void CBufferPop(CBuffer buffer, int size);

void CBufferClear(CBuffer buffer);

#endif