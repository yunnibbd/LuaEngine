#include "instruction.h"

int Opcode(Instruction i) {
	return i & 0x3f;
}

TABC ABC(Instruction i) {
	TABC ret = {
		i >> 6 & 0xff,
		i >> 14 & 0x1ff,
		i >> 23 & 0x1ff
	};
	return ret;
}

TABx ABx(Instruction i) {
	TABx ret = {
		i >> 6 & 0xff,
		i >> 14
	};
	return ret;
}

TAsBx AsBx(Instruction i) {
	TAsBx ret;/* = {

	};*/
	return ret;
}

int Ax(Instruction i) {

}
