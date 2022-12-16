#ifndef __LUA_STACK_H__
#define __LUA_STACK_H__
#include "cvector.h"
#include "lua_value.h"
#include <stdbool.h>

typedef struct {
	CVector slots;
	int top;
} *LuaStack, StructLuaStack;

LuaStack LuaStackAlloc(int size);

void LuaStackFree(LuaStack lua_stack);

void LuaStackReverse(LuaStack lua_stack, int from, int to);

void LuaStackCheck(LuaStack lua_stack, int n);

void LuaStackPush(LuaStack lua_stack, LuaValue* val);

LuaValue LuaStackPop(LuaStack lua_stack);

int LuaStackAbsIndex(LuaStack lua_stack, int idx);

bool LuaStackIsVaild(LuaStack lua_stack, int idx);

LuaValue LuaStackGet(LuaStack lua_stack, int idx);

void LuaStackSet(LuaStack lua_stack, int idx, LuaValue* val);

#endif
