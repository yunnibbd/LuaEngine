#ifndef __PARSER_H__
#define __PARSER_H__
#include "common_types.h"
#include "cbuffer.h"

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
