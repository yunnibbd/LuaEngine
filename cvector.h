#ifndef __C_VERTOR_H__
#define __C_VERTOR_H__
#include <stdbool.h>

typedef void(*FreeFuncType)(void* element);

typedef bool(*CompareFuncType)(void* element, void* element2);

typedef void(*CopyFuncType)(void* dst_addr, void* element);

typedef struct {
	void* vector_root_;
	int vector_size_;
	int vector_data_size_;
	int vector_item_size_;
	FreeFuncType free_func_;
	CompareFuncType compare_func_;
	CopyFuncType copy_func_;
} *CVector, StructCVector;

CVector CVectorAlloc(int vector_size, int vector_item_size, FreeFuncType free_func, CompareFuncType compare_func, CopyFuncType copy_func);

void CVectorGrow(CVector vector);

void CVectorFree(CVector vector);

bool CVectorPushBack(CVector vector, void* data);

void CVectorSet(CVector vector, int index, void* data);

void* CVectorGet(CVector vector, int index);

void* CVectorGetLast(CVector vector);

int CVectorSize(CVector vector);

int CVectorAllSize(CVector vector);

#endif