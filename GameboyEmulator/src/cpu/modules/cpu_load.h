#pragma once
#include "../cpu.h"

namespace CPULoad {
	CPU::CounterAction LD_B_N8(CPU&, CPU::Instruction);
	CPU::CounterAction LD_C_N8(CPU&, CPU::Instruction);
	CPU::CounterAction LD_D_N8(CPU&, CPU::Instruction);
	CPU::CounterAction LD_E_N8(CPU&, CPU::Instruction);
	CPU::CounterAction LD_H_N8(CPU&, CPU::Instruction);
	CPU::CounterAction LD_L_N8(CPU&, CPU::Instruction);
	CPU::CounterAction LD_R_R(CPU&, CPU::Instruction);
	CPU::CounterAction LD_BC_A(CPU&, CPU::Instruction);
	CPU::CounterAction LD_DE_A(CPU&, CPU::Instruction);
	CPU::CounterAction LD_A_BC(CPU&, CPU::Instruction);
	CPU::CounterAction LD_A_DE(CPU&, CPU::Instruction);
	CPU::CounterAction LD_INC_HL_A(CPU&, CPU::Instruction);
	CPU::CounterAction LD_DEC_HL_A(CPU&, CPU::Instruction);
	CPU::CounterAction LD_A_INC_HL(CPU&, CPU::Instruction);
	CPU::CounterAction LD_A_DEC_HL(CPU&, CPU::Instruction);
	CPU::CounterAction LD_BC_N16(CPU&, CPU::Instruction);
	CPU::CounterAction LD_DE_N16(CPU&, CPU::Instruction);
	CPU::CounterAction LD_HL_N16(CPU&, CPU::Instruction);
	CPU::CounterAction LD_SP_N16(CPU&, CPU::Instruction);
	CPU::CounterAction LD_N16_A(CPU&, CPU::Instruction);
	CPU::CounterAction LD_A_N16(CPU&, CPU::Instruction);
	CPU::CounterAction LDH_N8_A(CPU&, CPU::Instruction);
	CPU::CounterAction LDH_A_N8(CPU&, CPU::Instruction);
	CPU::CounterAction LDH_C_A(CPU&, CPU::Instruction);
	CPU::CounterAction LDH_A_C(CPU&, CPU::Instruction);
};