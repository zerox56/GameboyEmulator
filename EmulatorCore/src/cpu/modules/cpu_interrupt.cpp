#include "cpu_interrupt.h"

namespace CPUInterrupt {
    CPU::CounterAction CPUInterrupt::HALT(CPU& cpu, CPU::Instruction instruction) {
        if (cpu.state.IME == 1 || cpu.state.IME == 0 &&
            cpu.state.GetInterruptFlags(instruction.memory) == 0) {
            cpu.state.halted = true;
        }

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUInterrupt::DI(CPU& cpu, CPU::Instruction) {
        cpu.state.IME = 0;
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUInterrupt::EI(CPU& cpu, CPU::Instruction) {
        cpu.state.pendingIME = true;
        return CPU::CounterAction::AdvanceSkipIME;
    }
}