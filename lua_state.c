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
	for (int i = 0; i < n; ++i) {
		LuaStackPop(lua_state->stack);
	}
}

void LuaStateCopy(LuaState lua_state, int fromIdx, int toIdx) {
	LuaValue lua_value = LuaStackGet(lua_state->stack, fromIdx);
	LuaStackSet(lua_state->stack, toIdx, lua_value);
}

void LuaStatePushValue(LuaState lua_state, int idx) {
	LuaValue lua_value = LuaStackGet(lua_state->stack, idx);
	LuaStackPush(lua_state->stack, lua_value);
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

}

void LuaStateSetTop(LuaState lua_state, int idx) {

}

/* access functions(stack->Go) */
CBuffer LuaStateTypeName(LuaState lua_state, LuaType tp) {

}

LuaType LuaStateType(LuaState lua_state, int idx) {

}

bool LuaStateIsNone(LuaState lua_state, int idx) {

}

bool LuaStateIsNil(LuaState lua_state, int idx) {

}

bool LuaStateIsNoneOrNil(LuaState lua_state, int idx) {

}

bool LuaStateIsBoolean(LuaState lua_state, int idx) {

}

bool LuaStateIsInteger(LuaState lua_state, int idx) {

}

bool LuaStateIsNumber(LuaState lua_state, int idx) {

}

bool LuaStateIsString(LuaState lua_state, int idx) {

}

bool LuaStateToBoolean(LuaState lua_state, int idx) {

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

}

void LuaStatePushBoolean(bool b) {

}

void LuaStatePushInteger(LuaState lua_state, int64_t n) {

}

void LuaStatePushNumber(LuaState lua_state, double n) {

}

void LuaStatePushString(LuaState lua_state, CBuffer s) {

}
