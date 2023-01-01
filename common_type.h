#ifndef __COMMON_DEFINE_H__
#define __COMMON_DEFINE_H__
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
	int64_t i;
	bool b;
}Int64AndBool;
typedef struct {
	double d;
	bool b;
}DoubleAndBool;
typedef struct {
	CBuffer s;
	bool b;
}StringAndBool;

#endif
