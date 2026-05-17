#pragma once
#include "../cpu.h"

namespace CPUInterrupt {
	CPU::CounterAction HALT(CPU&, CPU::Instruction);
	CPU::CounterAction DI(CPU&, CPU::Instruction);
	CPU::CounterAction EI(CPU&, CPU::Instruction);
};