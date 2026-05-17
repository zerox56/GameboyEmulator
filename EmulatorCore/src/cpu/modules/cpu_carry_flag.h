#pragma once
#include "../cpu.h"

namespace CPUCarryFlag {
	CPU::CounterAction CCF(CPU&, CPU::Instruction);
	CPU::CounterAction SCF(CPU&, CPU::Instruction);
};