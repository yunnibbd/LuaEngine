#ifndef __LUA_STATE_H__
#define __LUA_STATE_H__
#include "lua_stack.h"
#include "cbuffer.h"
#include "common_type.h"
#include <inttypes.h>

typedef int LuaType;
typedef int ArithOp;
typedef int CompareOp;

typedef struct {
	LuaStack stack;
}*LuaState, StructLuaState;

typedef struct {
	int64_t(*integerFunc)(int64_t, int64_t);
	double(*floatFunc)(double, double);
} Operator;

extern Operator g_operators[14];

LuaState LuaStateAlloc();

void LuaStateFree(LuaState lua_state);

/* basic stack manipulation */
int LuaStateGetTop(LuaState lua_state);
int LuaStateAbsIndex(LuaState lua_state, int idx);
bool LuaStateCheckStack(LuaState lua_state, int n);
void LuaStatePop(LuaState lua_state, int n);
void LuaStateCopy(LuaState lua_state, int fromIdx, int toIdx);
void LuaStatePushValue(LuaState lua_state, int idx);
void LuaStateReplace(LuaState lua_state, int idx);
void LuaStateInsert(LuaState lua_state, int idx);
void LuaStateRemove(LuaState lua_state, int idx);
void LuaStateRotate(LuaState lua_state, int idx, int n);
void LuaStateSetTop(LuaState lua_state, int idx);
/* access functions(stack->Go) */
const char* LuaStateTypeName(LuaState lua_state, LuaType tp);
LuaType LuaStateType(LuaState lua_state, int idx);
bool LuaStateIsNone(LuaState lua_state, int idx);
bool LuaStateIsNil(LuaState lua_state, int idx);
bool LuaStateIsNoneOrNil(LuaState lua_state, int idx);
bool LuaStateIsBoolean(LuaState lua_state, int idx);
bool LuaStateIsInteger(LuaState lua_state, int idx);
bool LuaStateIsNumber(LuaState lua_state, int idx);
bool LuaStateIsString(LuaState lua_state, int idx);
bool LuaStateToBoolean(LuaState lua_state, int idx);
uint64_t LuaStateToInteger(LuaState lua_state, int idx);
Int64AndBool LuaStateToIntegerX(LuaState lua_state, int idx);
double LuaStateToNumber(LuaState lua_state, int idx);
DoubleAndBool LuaStateToNumberX(LuaState lua_state, int idx);
CBuffer LuaStateToString(LuaState lua_state, int idx);
StringAndBool LuaStateToStringX(LuaState lua_state, int idx);
/* push functions(Go->stack) */
void LuaStatePushNil(LuaState lua_state);
void LuaStatePushBoolean(LuaState lua_state, bool b);
void LuaStatePushInteger(LuaState lua_state, int64_t n);
void LuaStatePushNumber(LuaState lua_state, double n);
void LuaStatePushString(LuaState lua_state, CBuffer s);
void LuaStatePrint(LuaState lua_state);
void LuaStateArith(LuaState lua_state, ArithOp op);
bool LuaStateCompare(LuaState lua_state, int idx1, int idx2, CompareOp op);
void LuaStateLen(LuaState lua_state, int idx);
void LuaStateConcat(LuaState lua_state, int n);

#endif
