#ifndef __LUA_STACK_H__
#define __LUA_STACK_H__
#include "lua_value.h"
#include <stdbool.h>

typedef void* CVector;

typedef void* LuaStack;

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

inline int LuaStackGetTop(LuaStack lua_stack);
              
inline CVector LuaStackGetSlots(LuaStack lua_stack);

#endif
