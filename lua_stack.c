#include "lua_stack.h"
#include "lua_value.h"
#include "cvector.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define ABSINDEX(idx)\
do {\
	if (idx <= 0) {\
		idx = idx + lua_stack->top + 1;\
	}\
} while (0)

static void LuaValueFreeFunc(LuaValue* val) {
	switch (val->type){
		case LUA_TSTRING:
			CBufferFree(val->data.lua_string);
			break;
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
		LuaValue* tp1 = CVectorGet(lua_stack->slots , from);
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
		free = CVectorSize(lua_stack->slots) - lua_stack->top;
	}
}

void LuaStackPush(LuaStack lua_stack, LuaValue* lua_value) {
	if (CVectorAllSize(lua_stack->slots) == lua_stack->top) {
		printf("stack overflow!");
		exit(-1);
	}
	CVectorPushBack(lua_stack->slots, lua_value);
	++lua_stack->top;
}

LuaValue LuaStackPop(LuaStack lua_stack) {
	if (lua_stack->top < 1) {
		printf("stack underflow");
		exit(-1);
	}
	--lua_stack->top;
	LuaValue ret;
	LuaValue* p_val = CVectorGet(lua_stack->slots, lua_stack->top);
	LuaValue val;
	val.type = LUA_TNIL;
	ret = *p_val;
	CVectorSet(lua_stack->slots, lua_stack->top, &val);
	return ret;
}

int LuaStackAbsIndex(LuaStack lua_stack, int idx) {
	ABSINDEX(idx);
	return idx;
}

bool LuaStackIsValid(LuaStack lua_stack, int idx) {
	ABSINDEX(idx);
	return idx > 0 && idx <= lua_stack->top;
}

 LuaValue LuaStackGet(LuaStack lua_stack, int idx) {
	 ABSINDEX(idx);
	 LuaValue ret;
	 if (idx > 0 && idx <= lua_stack->top) {
		 LuaValue * val = CVectorGet(lua_stack->slots, idx - 1);
		 ret = *val;
	 }
	 else {
		 ret.type = LUA_TNIL;
	 }
	 return ret;
}

void LuaStackSet(LuaStack lua_stack, int idx, LuaValue lua_value) {
	ABSINDEX(idx);
	if (idx > 0 && idx <= lua_stack->top) {
		CVectorSet(lua_stack->slots, idx - 1, &lua_value);
		return;
	}
	printf("invaild index!\n");
	exit(-1);
}
