#pragma once
#include "../cpu.h"

namespace CPUBitShift {
	CPU::CounterAction BitShift(CPU&, CPU::Instruction);
	CPU::CounterAction ExecuteBitShift(CPU&, CPU::Instruction);

	void RLC_R(CPU&, CPU::Instruction);
};