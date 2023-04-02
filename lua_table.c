#include <stdlib.h>
#include "cvector.h"
#include "chash_int_map.h"
#include "lua_value.h"

typedef struct StructLuaTable {
	CVector arr; //LuaValue*
	CHashIntMap map; //key:int, val:LuaValue*
} *LuaTable;

static void LuaValueFreeFunc(void* val) {
	LuaValue* temp = *(LuaValue**)val;
	if (temp->type == LUA_TSTRING) {
		CBufferFree(temp->data.lua_string);
	}
	free(val);
}

static void LuaValueCopyFunc(void* addr, void* val) {
	memcpy(addr, val, sizeof(void*));
}

LuaTable LuaTableAlloc(int length) {
	LuaTable lua_table = (LuaTable)malloc(sizeof(struct StructLuaTable));
	lua_table->arr = CVectorAlloc(length, sizeof(void*), LuaValueFreeFunc, NULL, LuaValueCopyFunc);
	lua_table->map = CHashIntMapAlloc();

	return lua_table;
}

static void _floatToInteger(LuaValue* value) {
	if(value->type == LUA_TNUMBER) {
		Int64AndBool iab = FloatToInteger(value->data.lua_number);
		if(iab.b) {
			value->data.lua_integer = iab.i;
			value->type = LUA_TINTEGER;
		}
	}
}

LuaValue* LuaTableGet(LuaTable lua_table, LuaValue key) {
	_floatToInteger(&key);
	if(key.type == LUA_TINTEGER) {
		if(key.data.lua_integer >=1 && key.data.lua_integer <= CVectorSize(lua_table->arr)) {
			return CVectorGet(lua_table->arr, key.data.lua_integer);
		}
		else {
			return CHashIntMapGet(lua_table->map, key.data.lua_integer);
		}
	}

	//TODO 后续待调整
	return NULL;
}

static void _shirnkArray(LuaTable lua_table) {
	for(int i = CVectorSize(lua_table->arr) - 1; i >= 0; --i) {
		LuaValue* val = *(LuaValue**)CVectorGet(lua_table->arr, i);
		if(!val || val->type == LUA_TNIL) {
			CVectorRemove(lua_table->arr, i, 1);
		}
	}
}

static void _expandArray(LuaTable lua_table) {
	int idx = CVectorSize(lua_table->arr) + 1;
	for(;;) {
		LuaValue* val = CHashIntMapGet(lua_table->map, idx);
		if(val) {
			CHashIntMapRemoveByKeyWithoutFree(lua_table->map, idx);
			CVectorPushBack(lua_table->arr, &val);
		}
		else {
			break;
		}
	}
}

void LuaTablePut(LuaTable lua_table, LuaValue key, LuaValue* value) {
	if(key.type == LUA_TNIL) {
		PANIC("table index is nil!");
	}

	DoubleAndBool dab = LuaValueConvertToFloat(&key);
	if(!dab.b) {
		PANIC("table index is NaN!");
	}
	key.type = LUA_TNUMBER;
	key.data.lua_number = dab.d;

	_floatToInteger(&key);

	int idx = key.data.lua_integer;
	if(idx >= 1) {
		int arrLen = CVectorSize(lua_table->arr);
		if(idx <= arrLen) {
			CVectorSet(lua_table->arr, idx - 1, &value);
			if(idx == arrLen && value->type == LUA_TNIL) {
				_shirnkArray(lua_table->arr);
			}
			return;
		}
		if(idx == arrLen + 1) {
			CHashIntMapRemoveByKey(lua_table->map, value->data.lua_integer);
			if(value->type != LUA_TNIL) {
				CVectorPushBack(lua_table->arr, &value);
				_expandArray(lua_table);
			}
			return;
		}
	}

	if(value->type != LUA_TNIL) {
		if(lua_table->map == NULL) {
			lua_table->map = CHashIntMapAlloc();
		}
		CHashIntMapSet(lua_table->map, idx, &value);
	}
	else {
		CHashIntMapRemoveByKey(lua_table->map, idx);
	}
}

int LuaTableLen(LuaTable lua_table) {
	return CVectorSize(lua_table->arr);
}
