#ifndef __LUA_VM_H__
#define __LUA_VM_H__
#include "lua_state.h"
#include "instruction.h"

typedef LuaState LuaVM;

inline void LuaVMMove(LuaVM vm, Instruction i) {
	TABC abc = InstructionABC(i);
	abc.a += 1;
	LuaStateCopy(vm, abc.b, abc.a);
}

inline void LuaVMJmp(LuaVM vm, Instruction i) {
	TAsBx asbx = InstructionAsBx(i);
	LuaStateAddPC(vm, asbx.sbx);
	if (asbx.a != 0) {
		printf("todo!");
		exit(-1);
	}
}

inline void LuaVMLoadNil(LuaVM vm, Instruction i) {
	TABC abc = InstructionABC(i);
	++abc.a;

	LuaStatePushNil(vm);
	int end = abc.a + abc.b;
	for (int i = abc.a; i <= end; ++i) {
		LuaStateCopy(vm, -1, i);
	}

	LuaStatePop(vm, 1);
}



#endif
