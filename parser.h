
#ifndef __PARSER_H__
#define __PARSER_H__
#include "common_type.h"
#include "cbuffer.h"
#include "lua_value.h"

inline Int64AndBool ParseInteger(CBuffer str) {
	Int64AndBool ret;
	ret.i = _atoi64(CBufferData(str));
	ret.b = true;
	return ret;
}

inline DoubleAndBool ParseFloat(CBuffer str) {
	DoubleAndBool ret;
	ret.d = atof(CBufferData(str));
	ret.b = true;
	return ret;
}

#endif
