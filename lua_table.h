#ifndef __LUA_TABLE_H__
#define __LUA_TABLE_H__

typedef void* LuaTable;

LuaTable LuaTableAlloc(int length);

LuaValue* LuaTableGet(LuaTable lua_table, LuaValue key);

void LuaTablePut(LuaTable lua_table, LuaValue key, LuaValue* value);

int LuaTableLen(LuaTable lua_table);

#endif
