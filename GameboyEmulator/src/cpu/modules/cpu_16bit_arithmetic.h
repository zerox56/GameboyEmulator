#pragma once
#include "../cpu.h"

namespace CPU16BitArithmetic {
	CPU::CounterAction INC_BC(CPU&, CPU::Instruction);
	CPU::CounterAction INC_DE(CPU&, CPU::Instruction);
	CPU::CounterAction INC_HL(CPU&, CPU::Instruction);
	CPU::CounterAction INC_SP(CPU&, CPU::Instruction);
	CPU::CounterAction DEC_BC(CPU&, CPU::Instruction);
	CPU::CounterAction DEC_DE(CPU&, CPU::Instruction);
	CPU::CounterAction DEC_HL(CPU&, CPU::Instruction);
	CPU::CounterAction DEC_SP(CPU&, CPU::Instruction);
};