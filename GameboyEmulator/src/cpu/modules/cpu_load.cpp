#include "cpu_load.h"

namespace CPULoad {
    CPU::CounterAction LD_B_N8(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.B = instruction.L;
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_C_N8(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.C = instruction.L;
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_D_N8(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.D = instruction.L;
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_E_N8(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.E = instruction.L;
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_H_N8(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.H = instruction.L;
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_L_N8(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.L = instruction.L;
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_R_R(CPU& cpu, CPU::Instruction instruction) {
        // Mask opcode for dst and src register
        uint8_t dstIndex = (instruction.opcode >> 3) & 0b111;
        uint8_t srcIndex = instruction.opcode & 0b111;

        // Get register by index
        uint8_t* dst = cpu.state.registerLookup[dstIndex];
        uint8_t* src = cpu.state.registerLookup[srcIndex];

        if (dstIndex == 6 && src) {
            instruction.memory[cpu.state.GetHL()] = *src;
        }
        else if (srcIndex == 6 && dst) {
            *dst = instruction.memory[cpu.state.GetHL()];
        }
        else if (dst && src) {
            *dst = *src;
        }

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_BC_A(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.SetBC(instruction.memory[cpu.state.A]);
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_DE_A(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.SetDE(instruction.memory[cpu.state.A]);
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_A_BC(CPU& cpu, CPU::Instruction instruction) {
        uint16_t BC = cpu.state.GetBC();
        cpu.state.A = instruction.memory[BC];
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_A_DE(CPU& cpu, CPU::Instruction instruction) {
        uint16_t DE = cpu.state.GetDE();
        cpu.state.A = instruction.memory[DE];
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_INC_HL_A(CPU& cpu, CPU::Instruction instruction) {
        uint16_t HL = cpu.state.GetHL();
        instruction.memory[HL++] = cpu.state.A;
        cpu.state.SetHL(++HL);
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_DEC_HL_A(CPU& cpu, CPU::Instruction instruction) {
        uint16_t HL = cpu.state.GetHL();
        instruction.memory[HL--] = cpu.state.A;
        cpu.state.SetHL(--HL);
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_A_INC_HL(CPU& cpu, CPU::Instruction instruction) {
        uint16_t HL = cpu.state.GetHL();
        cpu.state.A = instruction.memory[HL];
        cpu.state.SetHL(++HL);
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_A_DEC_HL(CPU& cpu, CPU::Instruction instruction) {
        uint16_t HL = cpu.state.GetHL();
        cpu.state.A = instruction.memory[HL];
        cpu.state.SetHL(--HL);
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_BC_N16(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.SetBC((instruction.H << 8) | instruction.L);
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_DE_N16(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.SetDE((instruction.H << 8) | instruction.L);
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_HL_N16(CPU& cpu, CPU::Instruction instruction) {
        uint16_t HL = cpu.state.GetHL();
        instruction.memory[HL] = (instruction.H << 8) | instruction.L;;
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_SP_N16(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.SP = (instruction.H << 8) | instruction.L;;
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_N16_A(CPU& cpu, CPU::Instruction instruction) {
        uint16_t N16 = (instruction.H << 8) | instruction.L;;
        instruction.memory[N16] = cpu.state.A;
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LD_A_N16(CPU& cpu, CPU::Instruction instruction) {
        uint16_t N16 = (instruction.H << 8) | instruction.L;;
        cpu.state.A = instruction.memory[N16];
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LDH_N8_A(CPU& cpu, CPU::Instruction instruction) {
        instruction.memory[instruction.L] = cpu.state.A;
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LDH_A_N8(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.A = instruction.memory[instruction.L];
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LDH_C_A(CPU& cpu, CPU::Instruction instruction) {
        instruction.memory[0xFF00 + cpu.state.C] = cpu.state.A;
        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction LDH_A_C(CPU& cpu, CPU::Instruction instruction) {
        cpu.state.A = instruction.memory[0xFF00 + cpu.state.C];
        return CPU::CounterAction::Advance;
    }
}