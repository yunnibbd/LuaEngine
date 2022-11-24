#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

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

CVector CVectorAlloc(int vector_size, int vector_item_size, FreeFuncType free_func, CompareFuncType compare_func, CopyFuncType copy_func) {
	CVector vector = malloc(sizeof(StructCVector));
	vector->vector_root_ = malloc(vector_size * vector_item_size);
	vector->vector_size_ = vector_size;
	vector->vector_item_size_ = vector_item_size;
	vector->vector_data_size_ = 0;
	vector->free_func_ = free_func;
	vector->compare_func_ = compare_func;
	vector->copy_func_ = copy_func;
	return vector;
}

void CVectorFree(CVector vector) {
	if (vector != NULL) {
		if (vector->vector_root_ != NULL) {
			if (vector->free_func_ != NULL) {
				for (size_t i = 0; i < vector->vector_data_size_; ++i) {
					void* cur = ((char*)vector->vector_root_) + vector->vector_item_size_ * i;
					vector->free_func_(cur);
				}
			}
			free(vector->vector_root_);
		}
		free(vector);
	}
}

bool CVectorPushBack(CVector vector, void* data) {
	if (vector->vector_data_size_ >= vector->vector_size_) {
		vector->vector_size_ *= 2;
		vector->vector_root_ = realloc(vector->vector_root_, vector->vector_item_size_ * vector->vector_size_);
		assert(vector->vector_root_ != NULL);
		return false;
	}
	char* dst_addr = ((char*)vector->vector_root_) + vector->vector_item_size_ * vector->vector_data_size_;
	vector->copy_func_(dst_addr, data);
	++vector->vector_data_size_;
	return true;
}

int CVectorSize(CVector vector) {
	if (vector == NULL) {
		return 0;
	}
	return vector->vector_data_size_;
}

void* CVectorGet(CVector vector, int index) {
	if (index >= vector->vector_data_size_) {
		return NULL;
	}
	void* data = (char*)vector->vector_root_ + index * vector->vector_item_size_;
	return data;
}

void* CVectorGetLast(CVector vector) {
	return CVectorGet(vector, vector->vector_data_size_ - 1);
}
