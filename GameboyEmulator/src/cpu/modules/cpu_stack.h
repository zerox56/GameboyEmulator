#pragma once
#include "../cpu.h"

namespace CPUStack {
	CPU::CounterAction PUSH_BC(CPU&, CPU::Instruction);
	CPU::CounterAction POP_BC(CPU&, CPU::Instruction);
	CPU::CounterAction PUSH_DE(CPU&, CPU::Instruction);
	CPU::CounterAction POP_DE(CPU&, CPU::Instruction);
	CPU::CounterAction PUSH_HL(CPU&, CPU::Instruction);
	CPU::CounterAction POP_HL(CPU&, CPU::Instruction);
	CPU::CounterAction PUSH_AF(CPU&, CPU::Instruction);
	CPU::CounterAction POP_AF(CPU&, CPU::Instruction);
};