#include "cpu_misc.h"
#include "../cpu_helper.h"

namespace CPUMisc {
    CPU::CounterAction NOP(CPU&, CPU::Instruction) {
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction DAA(CPU& cpu, CPU::Instruction) {
        uint8_t value = 0;
        bool setC = false;

        if (!cpu.state.FN) {
            if (cpu.state.FH || (cpu.state.A & 0x0F) > 9) value |= 0x06;
            if (cpu.state.FC || cpu.state.A > 0x99) {
                value |= 0x60;
                setC = true;
            }
            cpu.state.A += value;
        }
        else {
            if (cpu.state.FH) value |= 0x06;
            if (cpu.state.FC) value |= 0x60;
            cpu.state.A -= value;
        }

        CPUHelper::FlagsType C = setC ? CPUHelper::FlagsType::True : CPUHelper::FlagsType::False;

        CPUHelper::UpdateFlags(cpu, cpu.state.A, value, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::None,
            CPUHelper::FlagsType::False, C);

        return CPU::CounterAction::Advance;
    }
}