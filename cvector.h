#ifndef __C_VERTOR_H__
#define __C_VERTOR_H__
#include <stdbool.h>

typedef void* CVector;

typedef void(*FreeFuncType)(void* element);

typedef bool(*CompareFuncType)(void* element, void* element2);

typedef void(*CopyFuncType)(void* dst_addr, void* element);

CVector CVectorAlloc(int vector_size, int vector_item_size, FreeFuncType free_func, CompareFuncType compare_func, CopyFuncType copy_func);

void CVectorGrow(CVector vector);

void CVectorFree(CVector vector);

bool CVectorPushBack(CVector vector, void* data);

void* CVectorPop(CVector vector);

void CVectorDecDataSize(CVector vector, int size);

void CVectorSet(CVector vector, int index, void* data);

void* CVectorGet(CVector vector, int index);

void* CVectorGetLast(CVector vector);

int CVectorSize(CVector vector);

int CVectorAllSize(CVector vector);

void CVectorRemove(CVector vector, int start_index, int count);

#endif