#include "cpu_8bit_arithmetic.h"
#include "../cpu_helper.h"

namespace CPU8BitArithmetic {
    CPU::CounterAction ADD_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

        uint8_t a = cpu.state.A;
        uint8_t result = a + b;
        cpu.state.A = result;

        CPUHelper::UpdateFlags(cpu, result, a, b, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
            CPUHelper::FlagsType::OverflowBit3, CPUHelper::FlagsType::OverflowBit7);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction ADC_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

        uint8_t a = cpu.state.A;
        uint8_t result = a + b + cpu.state.C;
        cpu.state.A = result;

        CPUHelper::UpdateFlags(cpu, result, a, b, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
            CPUHelper::FlagsType::OverflowBit3, CPUHelper::FlagsType::OverflowBit7);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction SUB_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

        uint8_t a = cpu.state.A;
        uint8_t result = a - b;
        cpu.state.A = result;

        CPUHelper::UpdateFlags(cpu, result, a, b, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::True,
            CPUHelper::FlagsType::BorrowBit4, CPUHelper::FlagsType::BorrowBit7);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction SBC_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

        uint8_t a = cpu.state.A;
        uint8_t result = a - b - cpu.state.C;
        cpu.state.A = result;

        CPUHelper::UpdateFlags(cpu, result, a, b, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::True,
            CPUHelper::FlagsType::BorrowBit4, CPUHelper::FlagsType::BorrowBit7);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction AND_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

        uint8_t a = cpu.state.A;
        uint8_t result = b & a;
        cpu.state.A = result;

        CPUHelper::UpdateFlags(cpu, result, a, b, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
            CPUHelper::FlagsType::True, CPUHelper::FlagsType::False);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction XOR_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

        uint8_t a = cpu.state.A;
        uint8_t result = b ^ a;
        cpu.state.A = result;

        CPUHelper::UpdateFlags(cpu, result, a, b, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
            CPUHelper::FlagsType::False, CPUHelper::FlagsType::False);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction OR_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

        uint8_t a = cpu.state.A;
        uint8_t result = b | a;
        cpu.state.A = result;

        CPUHelper::UpdateFlags(cpu, result, a, b, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
            CPUHelper::FlagsType::False, CPUHelper::FlagsType::False);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CP_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, true);

        uint8_t a = cpu.state.A; 
        uint8_t result = a - b;

        CPUHelper::UpdateFlags(cpu, result, a, b, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::True,
            CPUHelper::FlagsType::BorrowBit4, CPUHelper::FlagsType::BorrowBit7);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction INC_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, false);
        uint8_t result = b + 1;

        CPUHelper::UpdateFlags(cpu, result, 0, b, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::False,
            CPUHelper::FlagsType::OverflowBit3, CPUHelper::FlagsType::None);

        CPUHelper::SetRegisterValue(cpu, instruction, result);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction DEC_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t b = CPUHelper::GetRegisterValue(cpu, instruction, false);
        uint8_t result = b - 1;

        CPUHelper::UpdateFlags(cpu, result, 0, b, CPUHelper::FlagsType::ValueZero, CPUHelper::FlagsType::True,
            CPUHelper::FlagsType::BorrowBit4, CPUHelper::FlagsType::None);

        CPUHelper::SetRegisterValue(cpu, instruction, result);

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CPL(CPU& cpu, CPU::Instruction) {
        cpu.state.A = ~cpu.state.A;

        CPUHelper::UpdateFlags(cpu, 0, 0, 0, CPUHelper::FlagsType::None, CPUHelper::FlagsType::True,
            CPUHelper::FlagsType::True, CPUHelper::FlagsType::None);

        return CPU::CounterAction::Advance;
    }
}