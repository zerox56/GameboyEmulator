#pragma once
#include "../cpu.h"

namespace CPUMisc {
	CPU::CounterAction NOP(CPU&, CPU::Instruction);
	CPU::CounterAction DAA(CPU&, CPU::Instruction);
};