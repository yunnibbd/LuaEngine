#ifndef __C_VERTOR_H__
#define __C_VERTOR_H__
#include <stdbool.h>

typedef void* CVector;

typedef void(*FreeFuncType)(void* element);

typedef bool(*CompareFuncType)(void* element, void* element2);

typedef void(*CopyFuncType)(void* dst_addr, void* element);

CVector CVectorAlloc(int vector_size, int vector_item_size, FreeFuncType free_func, CompareFuncType compare_func, CopyFuncType copy_func);

void CVectorGrow(CVector vector);

void CVectorChange(CVector vector, int idx1, int idx2);

void CVectorFree(CVector vector);

bool CVectorPushBack(CVector vector, void* data);

void CVectorSet(CVector vector, int index, void* data);

void* CVectorGet(CVector vector, int index);

void* CVectorGetLast(CVector vector);

int CVectorSize(CVector vector);

#endif