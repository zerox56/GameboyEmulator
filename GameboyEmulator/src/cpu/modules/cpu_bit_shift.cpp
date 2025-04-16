#include "cpu_bit_shift.h"
#include "../cpu_helper.h"
#include "../../utils/bit_utils.h"

namespace CPUBitShift {
	CPU::CounterAction BitShift(CPU&, CPU::Instruction) {
		return CPU::CounterAction::BitShift;
	}

	CPU::CounterAction ExecuteBitShift(CPU& cpu, CPU::Instruction instruction) {
		if (instruction.opcode >= 0x00 && instruction.opcode < 0x08) {
			RLC_R(cpu, instruction);
		}

		return CPU::CounterAction::Advance;
	}

	void RLC_R(CPU& cpu, CPU::Instruction instruction) {
		uint8_t value = CPUHelper::GetRegisterValue(cpu, instruction);

		bool shiftedBit = BitUtils::GetMSB(value);
		value <<= 1;

		CPUHelper::UpdateFlags(cpu, value, value, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
			CPUHelper::FlagsType::False, CPUHelper::FlagsType::BasedOnValue);

		CPUHelper::SetRegisterValue(cpu, instruction, value);
	}
}