#ifndef __COMMON_TYPES_H__
#define __COMMON_TYPES_H__
#include "cbuffer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <math.h>

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

typedef uint32_t Instruction;

#define PANIC(fmt, ...) printf("file:[%s], line:[%d] panic:"fmt"\n", __FILE__, __LINE__, ##__VA_ARGS__); exit(-1);

#endif
