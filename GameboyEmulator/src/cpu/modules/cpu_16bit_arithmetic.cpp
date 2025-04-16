#include "cpu_16bit_arithmetic.h"

namespace CPU16BitArithmetic {
    CPU::CounterAction CPU16BitArithmetic::INC_BC(CPU& cpu, CPU::Instruction) {
        uint16_t BC = cpu.state.GetBC();
        cpu.state.SetBC(++BC);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPU16BitArithmetic::INC_DE(CPU& cpu, CPU::Instruction) {
        uint16_t DE = cpu.state.GetDE();
        cpu.state.SetDE(++DE);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPU16BitArithmetic::INC_HL(CPU& cpu, CPU::Instruction) {
        uint16_t HL = cpu.state.GetHL();
        cpu.state.SetHL(++HL);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPU16BitArithmetic::INC_SP(CPU& cpu, CPU::Instruction) {
        cpu.state.SP++;
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPU16BitArithmetic::DEC_BC(CPU& cpu, CPU::Instruction) {
        uint16_t BC = cpu.state.GetBC();
        cpu.state.SetBC(--BC);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPU16BitArithmetic::DEC_DE(CPU& cpu, CPU::Instruction) {
        uint16_t DE = cpu.state.GetDE();
        cpu.state.SetDE(--DE);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPU16BitArithmetic::DEC_HL(CPU& cpu, CPU::Instruction) {
        uint16_t HL = cpu.state.GetHL();
        cpu.state.SetHL(--HL);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPU16BitArithmetic::DEC_SP(CPU& cpu, CPU::Instruction) {
        cpu.state.SP--;
        return CPU::CounterAction::Advance;
    }
}