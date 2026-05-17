#pragma once
#include "cpu.h"

#include <cstdint>
#include <vector>

namespace CPUHelper {
	// Stack helpers
	void PUSH(uint16_t value, CPU& cpu, std::vector<uint8_t>& memory);
	uint16_t POP(CPU& cpu, std::vector<uint8_t>& memory);

	// Register helpers
	uint8_t GetRegisterIndex(CPU::Instruction instruction);
	uint8_t GetALURegisterIndex(CPU::Instruction instruction);
	uint8_t GetRegisterValue(CPU& cpu, CPU::Instruction instruction, bool aluInstruction);
	void SetRegisterValue(CPU& cpu, CPU::Instruction instruction, uint8_t value);

	// Flag helpers
	enum class FlagsType {
		False,
		True,
		ValueZero,
		ValueNotZero,
		BasedOnValue,
		OverflowBit3,
		OverflowBit3WithFlag,
		OverflowBit7,
		OverflowBit7WithFlag,
		BorrowBit4,
		BorrowBit7,
		Invert,
		None
	};
	void UpdateFlag(uint8_t result, uint8_t a, uint8_t b, bool carryIn, FlagsType flagType, bool& flag);
	void UpdateFlags(CPU& cpu, uint8_t result, uint8_t a, uint8_t b, bool carryIn, FlagsType Z, FlagsType N, FlagsType H, FlagsType C);
};