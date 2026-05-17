#include "cpu_stack.h"
#include "../cpu_helper.h"

namespace CPUStack {
    CPU::CounterAction PUSH_BC(CPU& cpu, CPU::Instruction instruction) {
        CPUHelper::PUSH(cpu.state.GetBC(), cpu, instruction.memory);
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction POP_BC(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.SetBC(CPUHelper::POP(cpu, instruction.memory));
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction PUSH_DE(CPU& cpu, CPU::Instruction instruction) {
        CPUHelper::PUSH(cpu.state.GetHL(), cpu, instruction.memory);
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction POP_DE(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.SetHL(CPUHelper::POP(cpu, instruction.memory));
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction PUSH_HL(CPU& cpu, CPU::Instruction instruction) {
        CPUHelper::PUSH(cpu.state.GetHL(), cpu, instruction.memory);
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction POP_HL(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.SetHL(CPUHelper::POP(cpu, instruction.memory));
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction PUSH_AF(CPU& cpu, CPU::Instruction instruction) {
        CPUHelper::PUSH(cpu.state.GetAF(), cpu, instruction.memory);
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction POP_AF(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.SetAF(CPUHelper::POP(cpu, instruction.memory));
        return CPU::CounterAction::Advance;
    }
}