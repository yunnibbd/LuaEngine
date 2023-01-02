#include "lua_state.h"
#include "lua_value.h"
#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
	LuaStateSetTop(lua_state, -n - 1);
}

void LuaStateCopy(LuaState lua_state, int fromIdx, int toIdx) {
	LuaValue lua_value = LuaStackGet(lua_state->stack, fromIdx);
	LuaStackSet(lua_state->stack, toIdx, &lua_value);
}

void LuaStatePushValue(LuaState lua_state, int idx) {
	LuaValue lua_value = LuaStackGet(lua_state->stack, idx);
	LuaStackPush(lua_state->stack, &lua_value);
}

void LuaStateReplace(LuaState lua_state, int idx) {
	LuaValue lua_value = LuaStackPop(lua_state->stack);
	LuaStackSet(lua_state->stack, idx, &lua_value);
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
	if (n > 0) {
		m = t - n;
	}
	else {
		m = p - n - 1;
	}
	LuaStackReverse(lua_state->stack, p, m);
	LuaStackReverse(lua_state->stack, m + 1, t);
	LuaStackReverse(lua_state->stack, p, t);
}

void LuaStateSetTop(LuaState lua_state, int idx) {
	int new_top = LuaStackAbsIndex(lua_state->stack, idx);
	if (new_top < 0) {
		printf("stack underflow!");
		exit(-1);
	}
	int n = lua_state->stack->top - new_top;
	if (n > 0) {
		for (int i = 0; i < n; ++i) {
			LuaStackPop(lua_state->stack);
		}
	}
	else if (n < 0) {
		LuaValue val;
		val.type = LUA_TNIL;
		for (int i = 0; i > n; --i) {
			LuaStackPush(lua_state->stack, &val);
		}
	}
}

/* access functions(stack->Go) */
const char* LuaStateTypeName(LuaState lua_state, LuaType tp) {
	switch (tp) {
	case LUA_TNONE:     return "no value";
	case LUA_TNIL:      return "nil";
	case LUA_TBOOLEAN:  return "boolean";
	case LUA_TINTEGER:  return "integer";
	case LUA_TNUMBER:   return "number";
	case LUA_TSTRING:   return "string";
	case LUA_TTABLE:    return "table";
	case LUA_TFUNCTION: return "function";
	case LUA_TTHREAD:   return "thread";
	default:            return "userdata";
	}
}

LuaType LuaStateType(LuaState lua_state, int idx) {
	if (LuaStackIsVaild(lua_state->stack, idx)) {
		LuaValue val = LuaStackGet(lua_state->stack, idx);
		return TYPEOF(val);
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
	return LuaStateType(lua_state, idx) <= LUA_TNONE;
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
	LuaType type = LuaStateType(lua_state, idx);
	return type == LUA_TSTRING || type == LUA_TNUMBER || type == LUA_TINTEGER;
}

bool LuaStateToBoolean(LuaState lua_state, int idx) {
	LuaValue val = LuaStackGet(lua_state->stack, idx);
	return LuaValueConvertToBoolean(&val);
}

uint64_t LuaStateToInteger(LuaState lua_state, int idx) {
	Int64AndBool ret = LuaStateToIntegerX(lua_state, idx);
	return ret.i;
}

Int64AndBool LuaStateToIntegerX(LuaState lua_state, int idx) {
	LuaValue val = LuaStackGet(lua_state->stack, idx);
	Int64AndBool ret = {
		val.data.lua_integer,
		val.type == LUA_TINTEGER
	};
	return ret;
}

double LuaStateToNumber(LuaState lua_state, int idx) {
	DoubleAndBool ret = LuaStateToNumberX(lua_state, idx);
	return ret.d;
}

DoubleAndBool LuaStateToNumberX(LuaState lua_state, int idx) {
	LuaValue val = LuaStackGet(lua_state->stack, idx);
	return LuaValueConvertToFloat(&val);
}

CBuffer LuaStateToString(LuaState lua_state, int idx) {
	StringAndBool ret = LuaStateToStringX(lua_state, idx);
	return ret.s;
}

StringAndBool LuaStateToStringX(LuaState lua_state, int idx) {
	LuaValue val = LuaStackGet(lua_state->stack, idx);
	StringAndBool ret;
	switch (val.type) {
	case LUA_TSTRING:
		ret.b = true;
		ret.s = val.data.lua_string;
		break;
	case LUA_TNUMBER: {
		char str[8] = { 0 };
		sprintf(str, "%lf", val.data.lua_number);
		CBuffer buffer = CBufferFromStr(str, sizeof(str));
		break;
	}
	case LUA_TINTEGER: {
		char str[8] = { 0 };
		sprintf(str, "%lld", val.data.lua_integer);
		CBuffer buffer = CBufferFromStr(str, sizeof(str));
		break;
	}
	default:
		ret.b = false;
		ret.s = CBufferFromStr("", 1);
		break;
	}
	return ret;
}

/* push functions(Go->stack) */
void LuaStatePushNil(LuaState lua_state) {
	LuaValue val;
	val.type = LUA_TNIL;
	LuaStackPush(lua_state->stack, &val);
}

void LuaStatePushBoolean(LuaState lua_state, bool b) {
	LuaValue val;
	val.type = LUA_TBOOLEAN;
	val.data.lua_boolean = b;
	LuaStackPush(lua_state->stack, &val);
}

void LuaStatePushInteger(LuaState lua_state, int64_t n) {
	LuaValue val;
	val.type = LUA_TINTEGER;
	val.data.lua_integer = n;
	LuaStackPush(lua_state->stack, &val);
}

void LuaStatePushNumber(LuaState lua_state, double n) {
	LuaValue val;
	val.type = LUA_TNUMBER;
	val.data.lua_number = n;
	LuaStackPush(lua_state->stack, &val);
}

void LuaStatePushString(LuaState lua_state, CBuffer s) {
	LuaValue val;
	val.type = LUA_TSTRING;
	val.data.lua_string = s;
	LuaStackPush(lua_state->stack, &val);
}

void LuaStatePrint(LuaState lua_state) {
	int idx = -1;
	for (;;) {
		if (LuaStackIsVaild(lua_state->stack, idx)) {
			LuaValue val = LuaStackGet(lua_state->stack, idx);
			switch (TYPEOF(val)) {
			case LUA_TNIL:
				printf("nil\n");
				break;
			case LUA_TBOOLEAN:
				printf("%s\n", val.data.lua_boolean == 0 ? "false" : "true");
				break;
			case LUA_TINTEGER:
				printf("%lld\n", val.data.lua_integer);
				break;
			case LUA_TNUMBER:
				printf("%lf\n", val.data.lua_number);
				break;
			case LUA_TSTRING: {
				char buffer[1024] = { 0 };
				memcpy(buffer, CBufferData(val.data.lua_string), CBufferDataSize(val.data.lua_string));
				printf("%s\n", buffer);
				break;
			}
			default:
				break;
			}
		}
		else {
			break;
		}
		--idx;
	}
}

void LuaStateArith(LuaState lua_state, ArithOp op) {
	LuaValue b = LuaStackPop(lua_state->stack);
	LuaValue a, bluaValue;
	if (op != LUA_OPUNM && op != LUA_OPBNOT) {
		a = LuaStackPop(lua_state->stack);
	}
	else {
		a = b;
	}

	Operator operator = g_operators[op];

}

bool LuaStateCompare(LuaState lua_state, int idx1, int idx2, CompareOp op) {

}

void LuaStateLen(LuaState lua_state, int idx) {

}

void LuaStateConcat(LuaState lua_state, int n) {

}
