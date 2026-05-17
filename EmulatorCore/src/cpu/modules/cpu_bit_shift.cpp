#include "cpu_bit_shift.h"
#include "../cpu_helper.h"
#include "../../utils/bit_utils.h"

namespace CPUBitShift {
	CPU::CounterAction BitShift(CPU&, CPU::Instruction) {
		return CPU::CounterAction::BitShift;
	}

	CPU::CounterAction ExecuteBitShift(CPU& cpu, CPU::Instruction instruction) {
		if (instruction.opcode >= 0x00 && instruction.opcode <= 0x07) {
			RLC_R(cpu, instruction);
		}
		else if (instruction.opcode >= 0x08 && instruction.opcode <= 0x0F) {
			RRC_R(cpu, instruction);
		}
		else if (instruction.opcode >= 0x10 && instruction.opcode <= 0x17) {
			RL_R(cpu, instruction);
		}
		else if (instruction.opcode >= 0x18 && instruction.opcode <= 0x1F) {
			SLA_R(cpu, instruction);
		}
		else if (instruction.opcode >= 0x20 && instruction.opcode <= 0x27) {
			SRA_R(cpu, instruction);
		}
		else if (instruction.opcode >= 0x28 && instruction.opcode <= 0x2F) {
			RR_R(cpu, instruction);
		}
		else if (instruction.opcode >= 0x30 && instruction.opcode <= 0x37) {
			SWAP_R(cpu, instruction);
		}
		else if (instruction.opcode >= 0x38 && instruction.opcode <= 0x3F) {
			SRL_R(cpu, instruction);
		}
		else if (instruction.opcode >= 0x40 && instruction.opcode <= 0x7F) {
			uint8_t bit = (instruction.opcode >> 3) & 0b111;
			BIT_B_R(cpu, instruction, bit);
		}
		else if (instruction.opcode >= 0x80 && instruction.opcode <= 0xBF) {
			uint8_t bit = (instruction.opcode >> 3) & 0b111;
			RES_B_R(cpu, instruction, bit);
		}
		else if (instruction.opcode >= 0xC0 && instruction.opcode <= 0xFF) {
			uint8_t bit = (instruction.opcode >> 3) & 0b111;
			SET_B_R(cpu, instruction, bit);
		}

		return CPU::CounterAction::Advance;
	}

	void RLC_R(CPU& cpu, CPU::Instruction instruction) {
		uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

		bool shiftedBit = BitUtils::GetMSB(b);
		uint8_t result = (b << 1) | shiftedBit;

		CPUHelper::FlagsType C = shiftedBit ? CPUHelper::FlagsType::True : CPUHelper::FlagsType::False;

		CPUHelper::UpdateFlags(cpu, result, 0, b, false, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
			CPUHelper::FlagsType::False, C);

		CPUHelper::SetRegisterValue(cpu, instruction, result);
	}

	void RRC_R(CPU& cpu, CPU::Instruction instruction) {
		uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

		bool shiftedBit = BitUtils::GetLSB(b);
		uint8_t result = (b >> 1) | (shiftedBit << 7);

		CPUHelper::FlagsType C = shiftedBit ? CPUHelper::FlagsType::True : CPUHelper::FlagsType::False;

		CPUHelper::UpdateFlags(cpu, result, shiftedBit, b, false, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
			CPUHelper::FlagsType::False, C);

		CPUHelper::SetRegisterValue(cpu, instruction, result);
	}

	void RL_R(CPU& cpu, CPU::Instruction instruction) {
		uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

		bool shiftedBit = BitUtils::GetMSB(b);
		uint8_t result = (b << 1) | cpu.state.FC;

		CPUHelper::FlagsType C = shiftedBit ? CPUHelper::FlagsType::True : CPUHelper::FlagsType::False;

		CPUHelper::UpdateFlags(cpu, result, 0, b, false, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
			CPUHelper::FlagsType::False, C);

		CPUHelper::SetRegisterValue(cpu, instruction, result);
	}

	void RR_R(CPU& cpu, CPU::Instruction instruction) {
		uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

		bool shiftedBit = BitUtils::GetLSB(b);
		uint8_t result = (b >> 1) | (cpu.state.FC << 7);

		CPUHelper::FlagsType C = shiftedBit ? CPUHelper::FlagsType::True : CPUHelper::FlagsType::False;

		CPUHelper::UpdateFlags(cpu, result, 0, b, false, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
			CPUHelper::FlagsType::False, C);

		CPUHelper::SetRegisterValue(cpu, instruction, result);
	}

	void SLA_R(CPU& cpu, CPU::Instruction instruction) {
		uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

		bool shiftedBit = BitUtils::GetMSB(b);
		uint8_t result = (b << 1) | 0;

		CPUHelper::FlagsType C = shiftedBit ? CPUHelper::FlagsType::True : CPUHelper::FlagsType::False;

		CPUHelper::UpdateFlags(cpu, result, 0, b, false, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
			CPUHelper::FlagsType::False, C);

		CPUHelper::SetRegisterValue(cpu, instruction, result);
	}

	void SRA_R(CPU& cpu, CPU::Instruction instruction) {
		uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

		bool shiftedBit = BitUtils::GetLSB(b);
		uint8_t result = (b >> 1) | (b & 0x80);

		CPUHelper::FlagsType C = shiftedBit ? CPUHelper::FlagsType::True : CPUHelper::FlagsType::False;

		CPUHelper::UpdateFlags(cpu, result, 0, b, false, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
			CPUHelper::FlagsType::False, C);

		CPUHelper::SetRegisterValue(cpu, instruction, result);
	}

	void SWAP_R(CPU& cpu, CPU::Instruction instruction) {
		uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

		uint8_t result = (b << 4) | (b >> 4);

		CPUHelper::UpdateFlags(cpu, result, 0, b, false, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
			CPUHelper::FlagsType::False, CPUHelper::FlagsType::False);

		CPUHelper::SetRegisterValue(cpu, instruction, result);
	}

	void SRL_R(CPU& cpu, CPU::Instruction instruction) {
		uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

		bool shiftedBit = BitUtils::GetLSB(b);
		uint8_t result = b >> 1;

		CPUHelper::FlagsType C = shiftedBit ? CPUHelper::FlagsType::True : CPUHelper::FlagsType::False;

		CPUHelper::UpdateFlags(cpu, result, 0, b, false, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
			CPUHelper::FlagsType::False, C);

		CPUHelper::SetRegisterValue(cpu, instruction, result);
	}

	void BIT_B_R(CPU& cpu, CPU::Instruction instruction, uint8_t bit) {
		uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);
		uint8_t result = (b >> bit) & 1;

		CPUHelper::UpdateFlags(cpu, result, 0, b, false, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
			CPUHelper::FlagsType::True, CPUHelper::FlagsType::None);
	}

	void SET_B_R(CPU& cpu, CPU::Instruction instruction, uint8_t bit) {
		uint8_t value = CPUHelper::GetRegisterValue(cpu, instruction, true);
		value |= (1 << bit);
		CPUHelper::SetRegisterValue(cpu, instruction, value);
	}

	void RES_B_R(CPU& cpu, CPU::Instruction instruction, uint8_t bit) {
		uint8_t value = CPUHelper::GetRegisterValue(cpu, instruction, true);
		value &= ~(1 << bit);
		CPUHelper::SetRegisterValue(cpu, instruction, value);
	}
}