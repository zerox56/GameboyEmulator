#pragma once
#include "../cpu.h"

namespace CPUBitShift {
	CPU::CounterAction BitShift(CPU&, CPU::Instruction);
	CPU::CounterAction ExecuteBitShift(CPU&, CPU::Instruction);

	void RLC_R(CPU&, CPU::Instruction);
	void RRC_R(CPU&, CPU::Instruction);
	void RL_R(CPU&, CPU::Instruction);
	void RR_R(CPU&, CPU::Instruction);
	void SLA_R(CPU&, CPU::Instruction);
	void SRA_R(CPU&, CPU::Instruction);
	void SWAP_R(CPU&, CPU::Instruction);
	void SRL_R(CPU&, CPU::Instruction);
	void BIT_B_R(CPU&, CPU::Instruction, uint8_t bit);
	void RES_B_R(CPU&, CPU::Instruction, uint8_t bit);
	void SET_B_R(CPU&, CPU::Instruction, uint8_t bit);
};