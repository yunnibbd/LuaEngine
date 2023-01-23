#ifndef __LUA_VM_H__
#define __LUA_VM_H__
#include "instruction.h"

typedef void* LuaVM;

void LuaVMMove(LuaVM vm, Instruction i);

void LuaVMJmp(LuaVM vm, Instruction i);

void LuaVMLoadNil(LuaVM vm, Instruction i);

void LuaVMLoadBool(LuaVM vm, Instruction i);

void LuaVMLoadK(LuaVM vm, Instruction i);

void LuaVMLoadKx(LuaVM vm, Instruction i);

void LuaVMAdd(LuaVM vm, Instruction i);
void LuaVMSub(LuaVM vm, Instruction i);
void LuaVMMul(LuaVM vm, Instruction i);
void LuaVMMod(LuaVM vm, Instruction i);
void LuaVMPow(LuaVM vm, Instruction i);
void LuaVMDiv(LuaVM vm, Instruction i);
void LuaVMIDiv(LuaVM vm, Instruction i);
void LuaVMBand(LuaVM vm, Instruction i);
void LuaVMBor(LuaVM vm, Instruction i);
void LuaVMBxor(LuaVM vm, Instruction i);
void LuaVMShl(LuaVM vm, Instruction i);
void LuaVMShr(LuaVM vm, Instruction i);
void LuaVMUnm(LuaVM vm, Instruction i);
void LuaVMBnot(LuaVM vm, Instruction i);
void LuaVMEq(LuaVM vm, Instruction i);
void LuaVMLt(LuaVM vm, Instruction i);
void LuaVMLe(LuaVM vm, Instruction i);
void LuaVMNot(LuaVM vm, Instruction i);

void LuaVMlength(LuaVM vm, Instruction i);
void LuaVMConcat(LuaVM vm, Instruction i);

void LuaVMTestSet(LuaVM vm, Instruction i);
void LuaVMTest(LuaVM vm, Instruction i);
void LuaVMForPrep(LuaVM vm, Instruction i);
void LuaVMForLoop(LuaVM vm, Instruction i);
#endif
