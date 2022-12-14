#ifndef __LUA_VALUE_H__
#define __LUA_VALUE_H__
#include <inttypes.h>
#include <stdbool.h>
#include "cbuffer.h"

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
		unsigned char lua_boolean;
		int64_t lua_integer;
		double lua_number;
		CBuffer lua_string;
	} data;
	enum  LuaType type;
} LuaValue;

#define TYPEOF(val) val.type

inline bool LuaValueConvertToBoolean(LuaValue val) {
	switch (TYPEOF(val)){
		case LUA_TNIL:
			return false;
		case LUA_TBOOLEAN:
			return val.data.lua_boolean;
		default:
			return true;
	}
}

#endif
