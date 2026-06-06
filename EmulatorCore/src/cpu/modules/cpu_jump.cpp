#include "cpu_jump.h"
#include "../cpu_helper.h"

namespace CPUJump {
    // Jump instructions
    CPU::CounterAction CPUJump::JP_N16(CPU& cpu, CPU::Instruction instruction) {
        instruction.pc = (instruction.H << 8) | instruction.L;
        return CPU::CounterAction::Jump;
    }

    CPU::CounterAction CPUJump::JR_N8(CPU& cpu, CPU::Instruction instruction) {
        if (instruction.H == 0x00) {
            return CPU::CounterAction::Advance;
        }

        instruction.pc += (int8_t)instruction.H;

        return CPU::CounterAction::Jump;
    }

    CPU::CounterAction CPUJump::JR_NZ_N8(CPU& cpu, CPU::Instruction instruction) {
        return cpu.state.FZ == 0 ? JR_N8(cpu, instruction) : CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUJump::JR_Z_N8(CPU& cpu, CPU::Instruction instruction) {
        return cpu.state.FZ == 1 ? JR_N8(cpu, instruction) : CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUJump::JR_NC_N8(CPU& cpu, CPU::Instruction instruction) {
        return cpu.state.FC == 0 ? JR_N8(cpu, instruction) : CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUJump::JR_C_N8(CPU& cpu, CPU::Instruction instruction) {
        return cpu.state.FC == 1 ? JR_N8(cpu, instruction) : CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUJump::RST(CPU& cpu, CPU::Instruction instruction) {
        uint16_t returnAddress = instruction.pc + 1;
        CPUHelper::PUSH(returnAddress, cpu, instruction.memory);

        // TODO: Check if more clever way to do this later.
        switch (instruction.opcode) {
        case 0xC7:
            instruction.pc = 0x0000;
            break;
        case 0xCF:
            instruction.pc = 0x0008;
            break;
        case 0xD7:
            instruction.pc = 0x0010;
            break;
        case 0xDF:
            instruction.pc = 0x0018;
            break;
        case 0xE7:
            instruction.pc = 0x0020;
            break;
        case 0xEF:
            instruction.pc = 0x0028;
            break;
        case 0xF7:
            instruction.pc = 0x0030;
            break;
        case 0xFF:
            instruction.pc = 0x0038;
            break;
        default:
            // Should never be here
            break;
        }
        return CPU::CounterAction::Jump;
    }

    // Subroutine instructions
    CPU::CounterAction CPUJump::CALL_N16(CPU& cpu, CPU::Instruction instruction) {
        uint16_t returnAddress = instruction.pc + 3;
        CPUHelper::PUSH(returnAddress, cpu, instruction.memory);
        return JP_N16(cpu, instruction);
    }

    CPU::CounterAction CPUJump::CALL_NZ_N16(CPU& cpu, CPU::Instruction instruction) {
        return cpu.state.FZ == 0 ? CALL_N16(cpu, instruction) : CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUJump::CALL_Z_N16(CPU& cpu, CPU::Instruction instruction) {
        return cpu.state.FZ == 1 ? CALL_N16(cpu, instruction) : CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUJump::CALL_NC_N16(CPU& cpu, CPU::Instruction instruction) {
        return cpu.state.FC == 0 ? CALL_N16(cpu, instruction) : CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUJump::CALL_C_N16(CPU& cpu, CPU::Instruction instruction) {
        return cpu.state.FC == 1 ? CALL_N16(cpu, instruction) : CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUJump::RET(CPU& cpu, CPU::Instruction instruction) {
        instruction.pc = CPUHelper::POP(cpu, instruction.memory);
        return CPU::CounterAction::Wait;
    }

    CPU::CounterAction CPUJump::RET_NZ(CPU& cpu, CPU::Instruction instruction) {
        return cpu.state.FZ == 0 ? RET(cpu, instruction) : CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUJump::RET_Z(CPU& cpu, CPU::Instruction instruction) {
        return cpu.state.FZ == 1 ? RET(cpu, instruction) : CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUJump::RET_NC(CPU& cpu, CPU::Instruction instruction) {
        return cpu.state.FC == 0 ? RET(cpu, instruction) : CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUJump::RET_C(CPU& cpu, CPU::Instruction instruction) {
        return cpu.state.FC == 1 ? RET(cpu, instruction) : CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPUJump::RETI(CPU& cpu, CPU::Instruction instruction) {
        instruction.pc = CPUHelper::POP(cpu, instruction.memory);
        cpu.state.pendingIME = true;
        return CPU::CounterAction::AdvanceSkipIME;
    }
}