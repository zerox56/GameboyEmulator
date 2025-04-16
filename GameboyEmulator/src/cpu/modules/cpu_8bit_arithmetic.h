#pragma once
#include "../cpu.h"

namespace CPU8BitArithmetic {
	uint8_t GetRegister(CPU&, CPU::Instruction);
	uint8_t GetRegisterValue(CPU&, CPU::Instruction);

	CPU::CounterAction ADD_A_R(CPU&, CPU::Instruction);
	CPU::CounterAction ADC_A_R(CPU&, CPU::Instruction);
	CPU::CounterAction SUB_A_R(CPU&, CPU::Instruction);
	CPU::CounterAction SBC_A_R(CPU&, CPU::Instruction);
	CPU::CounterAction AND_A_R(CPU&, CPU::Instruction);
	CPU::CounterAction XOR_A_R(CPU&, CPU::Instruction);
	CPU::CounterAction OR_A_R(CPU&, CPU::Instruction);
	CPU::CounterAction CP_A_R(CPU&, CPU::Instruction);

	const uint8_t EightFunctionOffset = 0x08;
	const uint8_t EightINCStart = 0x04;
	const uint8_t EightDECStart = 0x05;
	CPU::CounterAction INC_R(CPU&, CPU::Instruction);
	CPU::CounterAction DEC_R(CPU&, CPU::Instruction);
};