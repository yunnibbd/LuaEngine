#include "lua_stack.h"
#include "cbuffer.h"
#include "cvector.h"
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef int LuaType;
typedef struct {
	CVector slots; //LuaValue
	int32_t top;
} *LuaStack, StructLuaStack;
typedef struct {
	LuaStack stack;
}*LuaState, StructLuaState;
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

LuaState LuaStateAlloc() {
	LuaState lua_state = malloc(sizeof(StructLuaState));
	lua_state->stack = LuaStackAlloc(20);
	return lua_state;
}

void LuaStateFree(LuaState lua_state) {
	LuaStackFree(lua_state->stack);
	free(lua_state);
}

/* basic stack manipulation */
int LuaStateGetTop(LuaState lua_state) {
	return lua_state->stack->top;
}

int LuaStateAbsIndex(LuaState lua_state, int idx) {
	return LuaStackAbsIndex(lua_state->stack, idx);
}

bool LuaStateCheckStack(LuaState lua_state, int n) {
	LuaStackCheck(lua_state->stack, n);
	return true;
}

void LuaStatePop(LuaState lua_state, int n) {
	LuaStateSetTop(lua_state , -n - 1);
}

void LuaStateCopy(LuaState lua_state, int fromIdx, int toIdx) {
	LuaValue lua_value = LuaStackGet(lua_state->stack, fromIdx);
	LuaStackSet(lua_state->stack, toIdx, lua_value);
}

void LuaStatePushValue(LuaState lua_state, int idx) {
	LuaValue lua_value = LuaStackGet(lua_state->stack, idx);
	LuaStackPush(lua_state->stack, &lua_value);
}

void LuaStateReplace(LuaState lua_state, int idx) {
	LuaValue lua_value = LuaStackPop(lua_state->stack);
	LuaStackSet(lua_state->stack, idx, lua_value);
}

void LuaStateInsert(LuaState lua_state, int idx) {
	LuaStateRotate(lua_state, idx, 1);
}

void LuaStateRemove(LuaState lua_state, int idx) {
	LuaStateRotate(lua_state, idx, -1);
	LuaStatePop(lua_state, 1);
}

void LuaStateRotate(LuaState lua_state, int idx, int n) {
	int t = lua_state->stack->top - 1;
	int p = LuaStackAbsIndex(lua_state->stack, idx) - 1;
	int m;
	if (n >= 0) {
		m = t - n;
	}
	else {
		p - n - 1;
	}
	LuaStackReverse(lua_state->stack, p, m);
	LuaStackReverse(lua_state->stack, m + 1, t);
	LuaStackReverse(lua_state->stack, p, t);
}

void LuaStateSetTop(LuaState lua_state, int idx) {
	int new_top = LuaStackAbsIndex(lua_state->stack, idx);
	if (new_top < 0) {
		printf("stack underflow\n");
		exit(-1);
	}
	int n = lua_state->stack->top - new_top;
	if (n > 0) {
		for (int i = 0; i < n; ++i) {
			LuaStackPop(lua_state->stack);
		}
	}
	else if(n < 0){
		LuaValue val;
		val.type = LUA_TNIL;
		for (int i = 0; i > n; ++i) {
			LuaStackPush(lua_state->stack, &val);
		}
	}
}

/* access functions(stack->Go) */
CBuffer LuaStateTypeName(LuaState lua_state, LuaType tp) {
	switch (tp){
		case LUA_TNONE:
			return "no value";
		case LUA_TNIL:
			return "nil";
		case LUA_TBOOLEAN:
			return "boolean";
		case LUA_TNUMBER:
			return "number";
		case LUA_TSTRING:
			return "string";
		case LUA_TTABLE:
			return "table";
		case LUA_TFUNCTION:
			return "function";
		case LUA_TTHREAD:
			return "thread";
		default:
			return "userdata";
	}
}

LuaType LuaStateType(LuaState lua_state, int idx) {
	if (LuaStackIsValid(lua_state->stack, idx)) {
		LuaValue v = LuaStackGet(lua_state->stack, idx);
		return TYPEOF(v);
	}
	return LUA_TNONE;
}

bool LuaStateIsNone(LuaState lua_state, int idx) {
	return LuaStateType(lua_state, idx) == LUA_TNONE;
}

bool LuaStateIsNil(LuaState lua_state, int idx) {
	return LuaStateType(lua_state, idx) == LUA_TNIL;
}

bool LuaStateIsNoneOrNil(LuaState lua_state, int idx) {
	return LuaStateType(lua_state, idx) <= LUA_TNIL;
}

bool LuaStateIsBoolean(LuaState lua_state, int idx) {
	return LuaStateType(lua_state, idx) == LUA_TBOOLEAN;
}

bool LuaStateIsInteger(LuaState lua_state, int idx) {
	return LuaStateType(lua_state, idx) == LUA_TINTEGER;
}

bool LuaStateIsNumber(LuaState lua_state, int idx) {
	return LuaStateType(lua_state, idx) == LUA_TNUMBER;
}

bool LuaStateIsString(LuaState lua_state, int idx) {
	enum LuaType type = LuaStateType(lua_state, idx);
	return type == LUA_TSTRING || type == LUA_TNUMBER;
}

bool LuaStateToBoolean(LuaState lua_state, int idx) {
	LuaValue val = LuaStackGet(lua_state->stack, idx);
	
}

uint64_t LuaStateToInteger(LuaState lua_state, int idx) {

}

Int64AndBool LuaStateToIntegerX(LuaState lua_state, int idx) {

}

double LuaStateToNumber(LuaState lua_state, int idx) {

}

DoubleAndBool LuaStateToNumberX(LuaState lua_state, int idx) {

}

CBuffer LuaStateToString(LuaState lua_state, int idx) {

}

StringAndBool LuaStateToStringX(LuaState lua_state, int idx) {

}

/* push functions(Go->stack) */
void LuaStatePushNil(LuaState lua_state) {
	LuaValue val;
	val.type = LUA_TNIL;
	LuaStackPush(lua_state->stack, &val);
}

void LuaStatePushBoolean(LuaState lua_state, bool b) {
	LuaValue val;
	val.data.lua_boolean = b;
	val.type = LUA_TBOOLEAN;
	LuaStackPush(lua_state->stack, &val);
}

void LuaStatePushInteger(LuaState lua_state, int64_t n) {
	LuaValue val;
	val.data.lua_integer = n;
	val.type = LUA_TINTEGER;
	LuaStackPush(lua_state->stack, &val);
}

void LuaStatePushNumber(LuaState lua_state, double n) {
	LuaValue val;
	val.data.lua_number = n;
	val.type = LUA_TNUMBER;
	LuaStackPush(lua_state->stack, &val);
}

void LuaStatePushString(LuaState lua_state, CBuffer s) {
	LuaValue val;
	val.data.lua_string = s;
	val.type = LUA_TSTRING;
	LuaStackPush(lua_state->stack, &val);
}
