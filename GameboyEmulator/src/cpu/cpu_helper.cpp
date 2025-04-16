#include "cpu_helper.h"

namespace CPUHelper {
    // Stack helpers
    void PUSH(uint16_t value, CPU& cpu, std::vector<uint8_t>& memory) {
        memory[--cpu.state.SP] = value >> 8;
        memory[--cpu.state.SP] = value & 0xFF;
    }

    uint16_t POP(CPU& cpu, std::vector<uint8_t>& memory) {
        uint8_t low = memory[cpu.state.SP++];
        uint8_t high = memory[cpu.state.SP++];
        return (high << 8) | low;
    }

    // Register helpers
    uint8_t GetRegisterIndex(CPU::Instruction instruction) {
        // Mask opcode with binary masking
        return instruction.opcode & 0b111;
    }

    uint8_t GetRegisterValue(CPU& cpu, CPU::Instruction instruction) {
        uint8_t regIndex = GetRegisterIndex(instruction);
        if (regIndex == cpu.state.HLIndex) {
            return instruction.memory[cpu.state.GetHL()];
        }
        else {
            return *cpu.state.registerLookup[regIndex];
        }
    }

    void SetRegisterValue(CPU& cpu, CPU::Instruction instruction, uint8_t value) {
        uint8_t regIndex = GetRegisterIndex(instruction);
        if (regIndex == cpu.state.HLIndex) {
            instruction.memory[cpu.state.GetHL()] = value;
        }
        else {
            *cpu.state.registerLookup[regIndex] = value;
        }
    }

    // Flag helpers
    void UpdateFlag(uint8_t value, uint8_t oldValue, FlagsType flagType, bool& flag) {
        switch (flagType)
        {
            case FlagsType::False:
                flag = false;
                break;
            case FlagsType::True:
                flag = true;
                break;
            case FlagsType::ValueZero:
                flag = value == 0;
                break;
            case FlagsType::ValueNotZero:
                flag = value != 0;
                break;
            case FlagsType::BasedOnValue:
                flag = value;
                break;
            case FlagsType::OverflowBit3:
                // Check if overflows to bit 4 or higher (half carry)
                flag = ((oldValue & 0xF) + (value & 0xF)) > 0xF;
                break;
            case FlagsType::OverflowBit7:
                // Check if overflows to bit 8
                flag = value > 0xFF;
                break;
            case FlagsType::BorrowBit4:
                // Check if borrow from bit 4 or lower (half borrow)
                flag = ((oldValue & 0xF) - (value & 0xF)) < 0xF;
                break;
            case FlagsType::BorrowBit7:
                // Check if borrow from oldValue < value
                flag = oldValue < value;
                break;
            case FlagsType::Invert:
                flag = !flag;
                break;
            case FlagsType::None:
            default:
                break;
        }
    }

    void UpdateFlags(CPU& cpu, uint8_t value, uint8_t oldValue, FlagsType Z, FlagsType N, FlagsType H, FlagsType C) {
        UpdateFlag(value, oldValue, Z, cpu.state.FZ);
        UpdateFlag(value, oldValue, N, cpu.state.FN);
        UpdateFlag(value, oldValue, H, cpu.state.FH);
        UpdateFlag(value, oldValue, C, cpu.state.FC);
    }
}