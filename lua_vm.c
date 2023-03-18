#include "lua_vm.h"
#include "lua_state.h"
#include "lua_math.h"
#include "lua_value.h"

static void _compare(LuaVM vm, Instruction i, CompareOp op);

void LuaVMMove(LuaVM vm, Instruction i) {
	TABC abc = InstructionABC(i);
	abc.a += 1;
	abc.b += 1;
	LuaStateCopy(vm, abc.b, abc.a);
}

void LuaVMJmp(LuaVM vm, Instruction i) {
	TAsBx asbx = InstructionAsBx(i);
	LuaStateAddPC(vm, asbx.sbx);
	if (asbx.a != 0) {
		PANIC("todo!");
	}
}

void LuaVMLoadNil(LuaVM vm, Instruction i) {
	TABC abc = InstructionABC(i);
	++abc.a;

	LuaStatePushNil(vm);
	int end = abc.a + abc.b;
	for (int i = abc.a; i <= end; ++i) {
		LuaStateCopy(vm, -1, i);
	}

	LuaStatePop(vm, 1);
}

void LuaVMLoadBool(LuaVM vm, Instruction i) {
	TABC abc = InstructionABC(i);
	abc.a += 1;
	LuaStatePushBoolean(vm, abc.b != 0);
	LuaStateReplace(vm, abc.a);
	if (abc.c != 0) {
		LuaStateAddPC(vm, 1);
	}
}

void LuaVMLoadK(LuaVM vm, Instruction i) {
	TABx abx = InstructionABx(i);
	abx.a += 1;
	LuaStateGetConst(vm, abx.bx);
	LuaStateReplace(vm, abx.a);
}

void LuaVMLoadKx(LuaVM vm, Instruction i) {
	TABx abx = InstructionABx(i);
	abx.a += 1;
	
	Instruction ax = InstructionAx(LuaStateFetch(vm));
	LuaStateGetConst(vm, ax);
	LuaStateReplace(vm, abx.a);
}

static void _binaryArith(LuaVM vm, Instruction i, ArithOp op) {
	TABC abc = InstructionABC(i);
	abc.a += 1;
	LuaStateGetRK(vm, abc.b);
	LuaStateGetRK(vm, abc.c);
	LuaStateArith(vm, op);
	LuaStateReplace(vm, abc.a);
}

static void _unaryArith(LuaVM vm, Instruction i, ArithOp op) {
	TABC abc = InstructionABC(i);
	abc.a += 1;
	abc.b += 1;
	LuaStatePushValue(vm, abc.b);
	LuaStateArith(vm, op);
	LuaStateReplace(vm, abc.a);
}

void LuaVMAdd(LuaVM vm, Instruction i) {
	_binaryArith(vm, i, LUA_OPADD);
}

void LuaVMSub(LuaVM vm, Instruction i) {
	_binaryArith(vm, i, LUA_OPSUB);
}

void LuaVMMul(LuaVM vm, Instruction i) {
	_binaryArith(vm, i, LUA_OPMUL);
}

void LuaVMMod(LuaVM vm, Instruction i) {
	_binaryArith(vm, i, LUA_OPMOD);
}

void LuaVMPow(LuaVM vm, Instruction i){
	_binaryArith(vm, i, LUA_OPPOW);
}

void LuaVMDiv(LuaVM vm, Instruction i) {
	_binaryArith(vm, i, LUA_OPDIV);
}

void LuaVMIDiv(LuaVM vm, Instruction i) {
	_binaryArith(vm, i, LUA_OPIDIV);
}

void LuaVMBand(LuaVM vm, Instruction i) {
	_binaryArith(vm, i, LUA_OPBAND);
}

void LuaVMBor(LuaVM vm, Instruction i) {
	_binaryArith(vm, i, LUA_OPBOR);
}

void LuaVMBxor(LuaVM vm, Instruction i) {
	_binaryArith(vm, i, LUA_OPBXOR);
}

void LuaVMShl(LuaVM vm, Instruction i) {
	_binaryArith(vm, i, LUA_OPSHL);
}

void LuaVMShr(LuaVM vm, Instruction i) {
	_binaryArith(vm, i, LUA_OPSHR);
}

void LuaVMUnm(LuaVM vm, Instruction i) {
	_binaryArith(vm, i, LUA_OPUNM);
}

void LuaVMBnot(LuaVM vm, Instruction i) {
	_binaryArith(vm, i, LUA_OPBNOT);
}

void LuaVMEq(LuaVM vm, Instruction i) {
	_compare(vm, i, LUA_OPEQ);
}

void LuaVMLt(LuaVM vm, Instruction i) {
	_compare(vm, i, LUA_OPLT);
}

void LuaVMLe(LuaVM vm, Instruction i) {
	_compare(vm, i, LUA_OPLE);
}

void LuaVMNot(LuaVM vm, Instruction i) {
	TABC abc = InstructionABC(i);
	abc.a += 1;
	abc.b += 1;
	LuaStatePushBoolean(vm, !LuaStateToBoolean(vm, abc.b));
	LuaStateReplace(vm, abc.a);
}

void LuaVMlength(LuaVM vm, Instruction i) {
	TABC abc = InstructionABC(i);
	abc.a += 1;
	abc.b += 1;
	LuaStateLen(vm, abc.b);
	LuaStateReplace(vm, abc.a);
}

void LuaVMConcat(LuaVM vm, Instruction i) {
	TABC abc = InstructionABC(i);
	abc.a += 1;
	abc.b += 1;
	abc.c += 1;
	int n = abc.c - abc.b + 1;
	LuaStateCheckStack(vm, n);
	for (int i = abc.b; i < abc.c; ++i) {
		LuaStatePushValue(vm, i);
	}
	LuaStateConcat(vm, n);
	LuaStateReplace(vm, abc.a);
}

static void _compare(LuaVM vm, Instruction i, CompareOp op) {
	TABC abc = InstructionABC(i);
	LuaStateGetRK(vm, abc.b);
	LuaStateGetRK(vm, abc.c);
	if (LuaStateCompare(vm, -2, -1, op) != (abc.a != 0)) {
		LuaStateAddPC(vm, 1);
	}
	LuaStatePop(vm, 2);
}

void LuaVMTestSet(LuaVM vm, Instruction i) {
	TABC abc = InstructionABC(i);
	abc.a += 1;
	abc.b += 1;
	if (LuaStateToBoolean(vm, abc.b) == (abc.c != 0)) {
		LuaStateCopy(vm, abc.b, abc.a);
	}
	else {
		LuaStateAddPC(vm, 1);
	}
}

void LuaVMTest(LuaVM vm, Instruction i) {
	TABC abc = InstructionABC(i);
	abc.a += 1;
	if (LuaStateToBoolean(vm, abc.a) != (abc.c != 0)) {
		LuaStateAddPC(vm, 1);
	}
}

void LuaVMForPrep(LuaVM vm, Instruction i) {
	TAsBx asbx = InstructionAsBx(i);
	asbx.a += 1;
	//R(A)-=R(A+2)
	LuaStatePushValue(vm, asbx.a);
	LuaStatePushValue(vm, asbx.a + 2);
	LuaStateArith(vm, LUA_OPSUB);
	LuaStateReplace(vm, asbx.a);

	//pc+=sBx
	LuaStateAddPC(vm, asbx.sbx);
}

void LuaVMForLoop(LuaVM vm, Instruction i) {
	TAsBx asbx = InstructionAsBx(i);
	asbx.a += 1;

	//R(A)+=R(A+2)
	LuaStatePushValue(vm, asbx.a + 2);
	LuaStatePushValue(vm, asbx.a);
	LuaStateArith(vm, LUA_OPADD);
	LuaStateReplace(vm, asbx.a);

	//R(A) <? = R(A+1)
	bool isPositiveStep = LuaStateToNumber(vm, asbx.a + 2) >= 0;
	if (
		isPositiveStep && LuaStateCompare(vm, asbx.a, asbx.a + 1, LUA_OPLE) ||
		!isPositiveStep && LuaStateCompare(vm, asbx.a + 1, asbx.a, LUA_OPLE)
	){
		LuaStateAddPC(vm, asbx.sbx);
		LuaStateCopy(vm, asbx.a, asbx.a + 3);
	}
}
