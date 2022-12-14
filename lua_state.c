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
		char* str = malloc(8);
		memset(str, 0, 8);
		sprintf(str, "%lf", val.data.lua_number);
		ret.b = true;
		ret.s = CBufferFromStr(str, strlen(str));
		break;
	}
	case LUA_TINTEGER: {
		char* str = malloc(8);
		memset(str, 0, 8);
		sprintf(str, "%lld", val.data.lua_integer);
		ret.b = true;
		ret.s = CBufferFromStr(str, strlen(str));
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

static LuaValue _arith(LuaValue* a, LuaValue* b, Operator op) {
	LuaValue ret;
	if (op.floatFunc == NULL) {
		Int64AndBool iab = LuaValueConvertToInteger(a);
		if (iab.b) {
			Int64AndBool iab2 = LuaValueConvertToInteger(b);
			if (iab2.b) {
				ret.data.lua_integer = op.integerFunc(iab.i, iab2.i);
				ret.type = LUA_TINTEGER;
				return ret;
			}
		}
	}
	else {
		if (op.integerFunc != NULL) {
			if (a->type == LUA_TINTEGER) {
				if (b->type == LUA_TINTEGER) {
					ret.data.lua_integer = op.integerFunc(a->data.lua_integer, b->data.lua_integer);
					ret.type = LUA_TINTEGER;
					return ret;
				}
			}
		}
		DoubleAndBool dab = LuaValueConvertToFloat(a);
		if (dab.b) {
			DoubleAndBool dab2 = LuaValueConvertToFloat(b);
			if (dab2.b) {
				ret.data.lua_number = op.floatFunc(dab.d, dab2.d);
				ret.type = LUA_TNUMBER;
				return ret;
			}
		}
	}
	ret.type = LUA_TNIL;
	return ret;
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
	LuaValue result = _arith(&a, &b, operator);
	if (result.type != LUA_TNIL) {
		LuaStackPush(lua_state->stack, &result);
	}
	else {
		printf("arithmetic error!\n");
		exit(-1);
	}
}

static bool _eq(LuaValue* a, LuaValue* b) {
	switch (a->type) {
		case LUA_TNIL:
			return b->type == LUA_TNIL;
		case LUA_TBOOLEAN:
			return a->data.lua_boolean == b->data.lua_boolean;
		case LUA_TSTRING:
			return CBufferCompare(a->data.lua_string, b->data.lua_string);
		case LUA_TINTEGER: {
			switch (b->type) {
				case LUA_TINTEGER:
					return a->data.lua_integer == b->data.lua_integer;
				case LUA_TNUMBER:
					return (double)a->data.lua_integer == b->data.lua_number;
				default:
					return false;
			}

		}
		case LUA_TNUMBER: {
			switch (b->type) {
			case LUA_TINTEGER:
				return a->data.lua_number == (double)b->data.lua_integer;
			case LUA_TNUMBER:
				return a->data.lua_number == b->data.lua_number;
			default:
				return false;
			}
		}
		default:
			return a == b;
	}
}

static bool _lt(LuaValue* a, LuaValue* b) {
	switch (a->type) {
		case LUA_TSTRING: {
			if (b->type == LUA_TSTRING) {
				return CBufferDataSize(a->data.lua_string) < CBufferDataSize(b->data.lua_string);
			}
			break;
		}
		case LUA_TINTEGER: {
			switch (b->type) {
				case LUA_TINTEGER:
					return a->data.lua_integer < b->data.lua_integer;
				case LUA_TNUMBER:
					return (double)a->data.lua_integer < b->data.lua_number;
			}
			break;
		}
		case LUA_TNUMBER: {
			switch (b->type) {
				case LUA_TNUMBER:
					return a->data.lua_number < b->data.lua_number;
				case LUA_TINTEGER:
					return a->data.lua_number < (double)b->data.lua_integer;
			}

			break;
		}
	}
	printf("comparsion error!");
	exit(-1);

}

static bool _le(LuaValue* a, LuaValue* b) {
	switch (a->type) {
	case LUA_TSTRING: {
		if (b->type == LUA_TSTRING) {
			return CBufferDataSize(a->data.lua_string) <= CBufferDataSize(b->data.lua_string);
		}
		break;
	}
	case LUA_TINTEGER: {
		switch (b->type) {
		case LUA_TINTEGER:
			return a->data.lua_integer <= b->data.lua_integer;
		case LUA_TNUMBER:
			return (double)a->data.lua_integer <= b->data.lua_number;
		}
		break;
	}
	case LUA_TNUMBER: {
		switch (b->type) {
		case LUA_TNUMBER:
			return a->data.lua_number <= b->data.lua_number;
		case LUA_TINTEGER:
			return a->data.lua_number <= (double)b->data.lua_integer;
		}

		break;
	}
	}
	printf("comparsion error!");
	exit(-1);

}

bool LuaStateCompare(LuaState lua_state, int idx1, int idx2, CompareOp op) {
	LuaValue a = LuaStackGet(lua_state->stack, idx1);
	LuaValue b = LuaStackGet(lua_state->stack, idx2);
	switch (op) {
		case LUA_OPEQ:
			return _eq(&a, &b);
		case LUA_OPLT:
			return _lt(&a, &b);
		case LUA_OPLE:
			return _le(&a, &b);
		default:
			printf("invaild compare op!\n");
			exit(-1);
	}

}

void LuaStateLen(LuaState lua_state, int idx) {
	LuaValue val = LuaStackGet(lua_state->stack, idx);
	if (val.type == LUA_TSTRING) {
		LuaValue i;
		i.type = LUA_TINTEGER;
		i.data.lua_integer = CBufferDataSize(val.data.lua_string);
		LuaStackPush(lua_state->stack, &i);
	}
	else {
		printf("length error!");
		exit(-1);
	}
}

void LuaStateConcat(LuaState lua_state, int n) {
	if (n == 0) {
		LuaValue s;
		s.type = LUA_TSTRING;
		s.data.lua_string = CBufferFromStr("", 1);
		LuaStackPush(lua_state->stack, &s);
	}
	else {
		for (int i = 1; i < n; ++i) {
			if (LuaStateIsString(lua_state, -1) && LuaStateIsString(lua_state, -2)) {
				CBuffer s2 = LuaStateToString(lua_state, -1);
				CBuffer s1 = LuaStateToString(lua_state, -2);
				LuaStackPop(lua_state->stack);
				LuaStackPop(lua_state->stack);
				CBuffer s = CBufferAlloc(CBufferDataSize(s1) + CBufferDataSize(s2));
				CBufferPush(s, CBufferData(s1), CBufferDataSize(s1));
				CBufferPush(s, CBufferData(s2), CBufferDataSize(s2));
				LuaValue val;
				val.type = LUA_TSTRING;
				val.data.lua_string = s;
				LuaStackPush(lua_state->stack, &val);
				/*CBufferFree(s1);
				CBufferFree(s2);*/
				continue;
			}
			printf("concatenation error!");
			exit(-1);
		}
	}
}
