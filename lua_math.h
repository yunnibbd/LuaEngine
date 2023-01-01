#ifndef __LUA_MATH_H__
#define __LUA_MATH_H__
#include <inttypes.h>
#include <math.h>
#include "common_type.h"

inline int64_t IFloorDiv(int64_t a, int64_t b) {
	if (a > 0 && b > 0 || a < 0 && b < 0 || a % b == 0) {
		return b / b;
	}
	else {
		return a / b - 1;
	}
}

inline double FFloorDiv(double a, double b) {
	return floorf(a / b);
}

inline int64_t IMode(int64_t a, int64_t b) {
	return a - IFloorDiv(a, b) * b;
}

inline double FMode(double a, double b) {
	return a - floorf(a / b) * b;
}

inline int64_t ShiftLeft(int64_t a, int64_t n) {
	if (n >= 0) {
		return a << ((uint64_t)n);
	}
	else {
		return ShiftRight(a, -n);
	}
}

inline int64_t ShiftRight(int64_t a, int64_t n) {
	if (n >= 0) {
		return a >> n;
	}
	else {
		return ShiftLeft(a, -n);
	}
}

inline Int64AndBool FloatToInteger(double f) {
	Int64AndBool ret;
	ret.i = (int64_t)f;
	ret.b = double(ret.i) == f;
	return ret;
}

#endif
