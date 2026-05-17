#pragma once
#include "../cpu.h"

namespace CPUJump {
	// Jump instructions
	CPU::CounterAction JP_N16(CPU&, CPU::Instruction);
	CPU::CounterAction JR_N16(CPU&, CPU::Instruction);
	CPU::CounterAction JR_NZ_N16(CPU&, CPU::Instruction);
	CPU::CounterAction JR_Z_N16(CPU&, CPU::Instruction);
	CPU::CounterAction JR_NC_N16(CPU&, CPU::Instruction);
	CPU::CounterAction JR_C_N16(CPU&, CPU::Instruction);

	const uint8_t RSTOffset = 0x08;
	const uint8_t RSTStart = 0xC7;
	CPU::CounterAction RST(CPU&, CPU::Instruction);

	// Subroutine instructions
	CPU::CounterAction CALL_N16(CPU&, CPU::Instruction);
	CPU::CounterAction CALL_NZ_N16(CPU&, CPU::Instruction);
	CPU::CounterAction CALL_Z_N16(CPU&, CPU::Instruction);
	CPU::CounterAction CALL_NC_N16(CPU&, CPU::Instruction);
	CPU::CounterAction CALL_C_N16(CPU&, CPU::Instruction);
	CPU::CounterAction RET(CPU&, CPU::Instruction);
	CPU::CounterAction RET_NZ(CPU&, CPU::Instruction);
	CPU::CounterAction RET_Z(CPU&, CPU::Instruction);
	CPU::CounterAction RET_NC(CPU&, CPU::Instruction);
	CPU::CounterAction RET_C(CPU&, CPU::Instruction);
	CPU::CounterAction RETI(CPU&, CPU::Instruction);
};