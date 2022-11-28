#ifndef __LUA_STACK_H__
#define __LUA_STACK_H__
#include "lua_value.h"
#include <stdbool.h>

typedef void* LuaStack;

LuaStack LuaStackAlloc(int size);

void LuaStackCheck(LuaStack lua_stack, int n);

void LuaStackPush(LuaStack lua_stack, LuaValue lua_value);

LuaValue LuaStackPop(LuaStack lua_stack);

int LuaStackAbsIndex(LuaStack lua_stack, int idx);

bool LuaStackIsValid(LuaStack lua_stack, int idx);

LuaValue LuaStackGet(LuaStack lua_stack, int idx);

void LusStackSet(LuaStack lua_stack, int idx, LuaValue lua_value);

#endif
