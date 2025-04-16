#include "cpu_8bit_arithmetic.h"
#include "../cpu_helper.h"

namespace CPU8BitArithmetic {
    CPU::CounterAction ADD_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = CPUHelper::GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A;
        uint16_t result = oldA + value;
        cpu.state.A = result & 0xFF;

        CPUHelper::UpdateFlags(cpu, result, oldA, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
            CPUHelper::FlagsType::OverflowBit3, CPUHelper::FlagsType::OverflowBit7);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction ADC_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = CPUHelper::GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A;
        uint16_t result = oldA + value + cpu.state.C;
        cpu.state.A = result & 0xFF;

        CPUHelper::UpdateFlags(cpu, result, oldA, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
            CPUHelper::FlagsType::OverflowBit3, CPUHelper::FlagsType::OverflowBit7);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction SUB_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = CPUHelper::GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A;
        uint16_t result = oldA - value;
        cpu.state.A = result & 0xFF;

        CPUHelper::UpdateFlags(cpu, result, oldA, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::True,
            CPUHelper::FlagsType::BorrowBit4, CPUHelper::FlagsType::BorrowBit7);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction SBC_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = CPUHelper::GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A;
        uint16_t result = oldA - value - cpu.state.C;
        cpu.state.A = result & 0xFF;

        CPUHelper::UpdateFlags(cpu, result, oldA, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::True,
            CPUHelper::FlagsType::BorrowBit4, CPUHelper::FlagsType::BorrowBit7);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction AND_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = CPUHelper::GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A;
        uint16_t result = value & oldA;
        cpu.state.A = result & 0xFF;

        CPUHelper::UpdateFlags(cpu, result, oldA, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
            CPUHelper::FlagsType::True, CPUHelper::FlagsType::False);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction XOR_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = CPUHelper::GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A;
        uint16_t result = value ^ oldA;
        cpu.state.A = result & 0xFF;

        CPUHelper::UpdateFlags(cpu, result, oldA, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
            CPUHelper::FlagsType::False, CPUHelper::FlagsType::False);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction OR_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = CPUHelper::GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A;
        uint16_t result = value | oldA;
        cpu.state.A = result & 0xFF;

        CPUHelper::UpdateFlags(cpu, result, oldA, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
            CPUHelper::FlagsType::False, CPUHelper::FlagsType::False);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CP_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = CPUHelper::GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A; 
        uint16_t result = oldA - value;

        CPUHelper::UpdateFlags(cpu, (result & 0xFF), oldA, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::True,
            CPUHelper::FlagsType::BorrowBit4, CPUHelper::FlagsType::BorrowBit7);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction INC_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = CPUHelper::GetRegisterValue(cpu, instruction);
        uint8_t result = value + 1;

        CPUHelper::UpdateFlags(cpu, result, value, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
            CPUHelper::FlagsType::OverflowBit3, CPUHelper::FlagsType::None);

        CPUHelper::SetRegisterValue(cpu, instruction, result);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction DEC_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = CPUHelper::GetRegisterValue(cpu, instruction);
        uint8_t result = value - 1;

        CPUHelper::UpdateFlags(cpu, result, value, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::True,
            CPUHelper::FlagsType::BorrowBit4, CPUHelper::FlagsType::None);

        CPUHelper::SetRegisterValue(cpu, instruction, result);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPL(CPU& cpu, CPU::Instruction) {
        cpu.state.A = ~cpu.state.A;

        CPUHelper::UpdateFlags(cpu, 0, 0, CPUHelper::FlagsType::None, CPUHelper::FlagsType::True,
            CPUHelper::FlagsType::True, CPUHelper::FlagsType::None);

        return CPU::CounterAction::Advance;
    }
}