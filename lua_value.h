#ifndef __LUA_VALUE_H__
#define __LUA_VALUE_H__
#include "lua_common_type.h"
#include "lua_math.h"

enum LuaType {
	LUA_TNONE,
	LUA_TNIL,
	LUA_TBOOLEAN,
	LUA_TLIGHTUSERDATA,
	LUA_TINTEGER,
	LUA_TNUMBER,
	LUA_TSTRING,
	LUA_TTABLE,
	LUA_TFUNCTION,
	LUA_TUSERDATA,
	LUA_TTHREAD,
};

typedef struct {
	union {
		unsigned char lua_boolearn;
		int64_t lua_integer;
		double lua_number;
		CBuffer lua_string;
	} data;
	enum LuaType type;
} LuaValue;

#define TYPEOF(val) (val.type)

inline bool LuaValueConvertToBoolean(LuaValue* val) {
	switch (val->type) {
		case LUA_TNIL:
			return false;
		case LUA_TBOOLEAN:
			return val->data.lua_boolearn;
		default:
			return true;
	}

}

inline DoubleAndBool LuaValueConvertToFloat(LuaValue* val) {
	DoubleAndBool ret;
	switch(val->type) {
		case LUA_TNUMBER:
			ret.b = true;
			ret.d = val->data.lua_number;
			break;
		case LUA_TINTEGER:
			ret.b = true;
			ret.d = (double)val->data.lua_integer;
			break;
		case LUA_TSTRING:
			ret.b = true;
			ret.d = atof(CBufferData(val->data.lua_string));
			break;
		default:
			ret.b = false;
			ret.d = 0.0f;
	}

	return ret;
}

inline Int64AndBool LuaValueConvertToInteger(LuaValue* val) {
	Int64AndBool ret;
	switch (val->type) {
		case LUA_TINTEGER:
			ret.b = true;
			ret.i = val->data.lua_integer;
			break;
		case LUA_TNUMBER:
			return FloatToInteger(val->data.lua_number);
		case LUA_TSTRING:
			ret.i = _atoi64(CBufferData(val->data.lua_string));
			ret.b = true;
			break;
		default:
			ret.b = false;
			ret.i = 0;
	}
	return ret;
}

#endif
