#include "lua_stack.h"
#include "lua_value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ABSINDEX(lua_stack, i)\
do{\
	if (idx < 0) {\
		i = i + lua_stack->top + 1;\
	}\
}while(0)

static void LuaValueFreeFunc(LuaValue* val) {
	if (val->type == LUA_TSTRING) {
		CBufferFree(val->data.lua_string);
	}
}

static void LuaValueCopyFunc(void* addr, LuaValue* val) {
	memcpy(addr, val, sizeof(LuaValue));
}

LuaStack LuaStackAlloc(int size) {
	LuaStack lua_stack = malloc(sizeof(StructLuaStack));
	lua_stack->slots = CVectorAlloc(size, sizeof(LuaValue), LuaValueFreeFunc, NULL, LuaValueCopyFunc);
	lua_stack->top = 0;
	return lua_stack;
}

void LuaStackFree(LuaStack lua_stack) {
	CVectorFree(lua_stack->slots);
	free(lua_stack);
}

void LuaStackReverse(LuaStack lua_stack, int from, int to) {
	while (from < to) {
		LuaValue* tp1 = CVectorGet(lua_stack->slots, from);
		LuaValue* tp2 = CVectorGet(lua_stack->slots, to);
		CVectorSet(lua_stack->slots, from, tp2);
		CVectorSet(lua_stack->slots, to, tp1);
		++from;
		--to;
	}
}

void LuaStackCheck(LuaStack lua_stack, int n) {
	int free = CVectorAllSize(lua_stack->slots) - lua_stack->top;
	while (free < n) {
		CVectorGrow(lua_stack->slots);
		free = CVectorAllSize(lua_stack->slots) - lua_stack->top;
	}
}

void LuaStackPush(LuaStack lua_stack, LuaValue* val) {
	if (CVectorAllSize(lua_stack->slots) == lua_stack->top) {
		printf("stack overflow!");
		exit(-1);
	}
	CVectorPushBack(lua_stack->slots, val);
	++lua_stack->top;
}

LuaValue LuaStackPop(LuaStack lua_stack) {
	if (lua_stack->top < 1) {
		printf("stack underflow!");
		exit(-1);
	}
	--lua_stack->top;
	LuaValue* pVal = CVectorGet(lua_stack->slots, lua_stack->top);
	LuaValue ret = *pVal;
	pVal->type = LUA_TNIL;
	return ret;
}

int LuaStackAbsIndex(LuaStack lua_stack, int idx) {
	ABSINDEX(lua_stack, idx);
	return idx;
}

bool LuaStackIsVaild(LuaStack lua_stack, int idx) {
	ABSINDEX(lua_stack, idx);
	return idx > 0 && idx <= lua_stack->top;
}

LuaValue LuaStackGet(LuaStack lua_stack, int idx) {
	ABSINDEX(lua_stack, idx);
	LuaValue ret;
	if (idx > 0 && idx <= lua_stack->top) {
		LuaValue* val = CVectorGet(lua_stack->slots, idx - 1);
		ret = *val;
	}
	else {
		ret.type = LUA_TNIL;
	}
	return ret;
}

void LuaStackSet(LuaStack lua_stack, int idx, LuaValue* val) {
	ABSINDEX(lua_stack, idx);
	if (idx > 0 && idx <= lua_stack->top) {
		CVectorSet(lua_stack->slots, idx - 1, val);
		return;
	}
	printf("invaild index!");
	exit(-1);;
}
