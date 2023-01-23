#ifndef __LUA_MATH_H__
#define __LUA_MATH_H__
#include "lua_common_type.h"
#include <math.h>

enum LuaMathOp {
	LUA_OPADD,		   // +
	LUA_OPSUB,         // -
	LUA_OPMUL,         // гк
	LUA_OPMOD,         // %
	LUA_OPPOW,         // ^
	LUA_OPDIV,         // /
	LUA_OPIDIV,        // //
	LUA_OPBAND,        // &
	LUA_OPBOR,         // |
	LUA_OPBXOR,        // бл
	LUA_OPSHL,         // <<
	LUA_OPSHR,         // >>
	LUA_OPUNM,         // - (unary minus)
	LUA_OPBNOT,        // бл
};

enum LuaCompareOp {
	LUA_OPEQ,
	LUA_OPLT,
	LUA_OPLE,
};

typedef struct {
	int64_t(*integerFunc)(int64_t, int64_t);
	double(*floatFunc)(double, double);
} Operator;

extern Operator g_operators[14];

inline int64_t lua_iadd(int64_t a, int64_t b) {
	return a + b;
}

inline double lua_fadd(double a, double b) {
	return a + b;
}

inline int64_t lua_isub(int64_t a, int64_t b) {
	return a - b;
}

inline double lua_fsub(double a, double b) {
	return a - b;
}

inline int64_t lua_imul(int64_t a, int64_t b) {
	return a * b;
}

inline double lua_fmul(double a, double b) {
	return  a * b;
}

inline int64_t lua_imod(int64_t a, int64_t b) {
	return a % b;
}

inline double lua_fmod(double a, double b) {
	return fmod(a, b);
}

inline double lua_pow(double a, double b) {
	return pow(a, b);
}

inline double lua_div(double a, double b) {
	return a / b;
}

inline int64_t lua_iidiv(int64_t a, int64_t b) {
	return a / b;
}

inline double lua_fidiv(double a, double b) {
	return a / b;
}

inline int64_t lua_band(int64_t a, int64_t b) {
	return a & b;
}

inline int64_t lua_bor(int64_t a, int64_t b) {
	return a | b;
}

inline int64_t lua_bxor(int64_t a, int64_t b) {
	return a ^ b;
}

#define lua_shl ShiftLeft
#define lua_shr ShiftRight

inline int64_t lua_iunm(int64_t a, int64_t _) {
	return -a;
}

inline double lua_funm(double a, double _) {
	return -a;
}

inline int64_t lua_bnot(int64_t a, int64_t _) {
	return ~a;
}

inline int64_t IFloorDiv(int64_t a, int64_t b) {
	if (a > 0 && b > 0 || a < 0 && b < 0 || a % b == 0) {
		return a / b;
	}
	else {
		return a / b - 1;
	}
}

inline double FFloorDiv(double a, double b) {
	return floorl(a / b);
}

inline int64_t IMod(int64_t a, int64_t b) {
	return a - IFloorDiv(a, b) * b;
}

inline double FMod(double a, double b) {
	return a - floorl(a / b) * b;
}

inline int64_t ShiftRight(int64_t a, int64_t n);

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
	ret.b = ((double)ret.i) == f;
	return ret;
}

#endif
