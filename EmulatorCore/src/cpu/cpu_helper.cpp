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
        return (instruction.opcode >> 3) & 0b111;
    }

    uint8_t GetALURegisterIndex(CPU::Instruction instruction) {
        // Mask opcode with binary masking
        return instruction.opcode & 0b111;
    }

    uint8_t GetRegisterValue(CPU& cpu, CPU::Instruction instruction, bool aluInstruction) {
        uint8_t regIndex = 0;
        if (aluInstruction) {
            regIndex = GetALURegisterIndex(instruction);
        }
        else {
            regIndex = GetRegisterIndex(instruction);
        }

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
    void UpdateFlag(uint8_t result, uint8_t a, uint8_t b, bool carryIn, FlagsType flagType, bool& flag) {
        switch (flagType)
        {
            case FlagsType::False:
                flag = false;
                break;
            case FlagsType::True:
                flag = true;
                break;
            case FlagsType::ValueZero:
                flag = result == 0;
                break;
            case FlagsType::ValueNotZero:
                flag = result != 0;
                break;
            case FlagsType::BasedOnValue:
                flag = result;
                break;
            case FlagsType::OverflowBit3:
                // Check if overflows to bit 4 or higher (half carry)
                flag = ((a & 0xF) + (b & 0xF)) > 0xF;
                break;
            case FlagsType::OverflowBit3WithFlag:
                // Check if overflows to bit 4 or higher (half carry) with flag
                flag = ((a & 0xF) + (b & 0xF) + carryIn) > 0xF;
                break;
            case FlagsType::OverflowBit7:
                // Check if overflows to bit 8
                flag = (uint16_t)a + (uint16_t)b > 0xFF;
                break;
            case FlagsType::OverflowBit7WithFlag:
                // Check if overflows to bit 8 with flag
                flag = (uint16_t)a + (uint16_t)b + (uint16_t)carryIn > 0xFF;
                break;
            case FlagsType::BorrowBit4:
                // Check if borrow from bit 4 or lower (half borrow)
                flag = (a & 0xF) < (b & 0xF);
                break;
            case FlagsType::BorrowBit7:
                // Check if borrow from oldValue < value
                flag = a < b;
                break;
            case FlagsType::Invert:
                flag = !flag;
                break;
            case FlagsType::None:
            default:
                break;
        }
    }

    void UpdateFlags(CPU& cpu, uint8_t result, uint8_t a, uint8_t b, bool carryIn, FlagsType Z, FlagsType N, FlagsType H, FlagsType C) {
        printf("INFO - result: 0x%02X | a: 0x%02X | b: 0x%02X\n",
            result, a, b);

        printf("Old Flags - Z: 0x%02X | N: 0x%02X | H: 0x%02X | C: 0x%02X \n", 
            cpu.state.FZ, cpu.state.FN, cpu.state.FH, cpu.state.FC);

        UpdateFlag(result, a, b, carryIn, Z, cpu.state.FZ);
        UpdateFlag(result, a, b, carryIn, N, cpu.state.FN);
        UpdateFlag(result, a, b, carryIn, H, cpu.state.FH);
        UpdateFlag(result, a, b, carryIn, C, cpu.state.FC);

        printf("New Flags - Z: 0x%02X | N: 0x%02X | H: 0x%02X | C: 0x%02X \n",
            cpu.state.FZ, cpu.state.FN, cpu.state.FH, cpu.state.FC);
    }
}