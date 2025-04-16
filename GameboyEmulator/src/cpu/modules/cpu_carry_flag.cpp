#include "cpu_carry_flag.h"
#include "../cpu_helper.h"

namespace CPUCarryFlag {
	CPU::CounterAction CCF(CPU& cpu, CPU::Instruction) {
		CPUHelper::UpdateFlags(cpu, 0, 0, CPUHelper::FlagsType::None, CPUHelper::FlagsType::False,
			CPUHelper::FlagsType::False, CPUHelper::FlagsType::Invert);
		return CPU::CounterAction::Advance;
	}

	CPU::CounterAction SCF(CPU& cpu, CPU::Instruction) {
		CPUHelper::UpdateFlags(cpu, 0, 0, CPUHelper::FlagsType::None, CPUHelper::FlagsType::False,
			CPUHelper::FlagsType::False, CPUHelper::FlagsType::True);
		return CPU::CounterAction::Advance;
	}
};