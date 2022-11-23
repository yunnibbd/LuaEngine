#ifndef __C_BUFFER_H__
#define __C_BUFFER_H__
#include <stdbool.h>

typedef void* CBuffer;

CBuffer CBufferAlloc(int size);

CBuffer CBufferFromStr(char* data, int data_size);

void CBufferFree(CBuffer buffer);

char* CBufferData(CBuffer buffer);

int CBufferSize(CBuffer buffer);

bool CBufferPush(CBuffer buffer, const char* data, int size);

void CBufferPop(CBuffer buffer, int size);

void CBufferClear(CBuffer buffer);

#endif
