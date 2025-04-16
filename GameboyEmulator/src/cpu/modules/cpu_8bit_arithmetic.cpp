#include "cpu_8bit_arithmetic.h"

namespace CPU8BitArithmetic {
    uint8_t GetRegister(CPU& cpu, CPU::Instruction instruction) {
        // Mask opcode with binary masking
        return instruction.opcode & 0b111;
    }

    uint8_t GetRegisterValue(CPU& cpu, CPU::Instruction instruction) {
        uint8_t srcIndex = GetRegister(cpu, instruction);

        // Check the lookup table through the earlier index
        uint8_t* src = cpu.state.registerLookup[srcIndex];

        if (srcIndex == 6) { // If 6 means nullptr = HL
            return instruction.memory[cpu.state.GetHL()];
        }
        else {
            return *src;
        }
    }

    CPU::CounterAction ADD_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A; // Keep oldA value for flags
        uint16_t result = oldA + value;
        cpu.state.A = result & 0xFF; // Mask back to 8bit size

        cpu.state.FZ = (cpu.state.A == 0);
        cpu.state.FN = 0;
        cpu.state.FH = ((oldA & 0xF) + (value & 0xF)) > 0xF; // Check if overflows to bit 4 or higher (half carry)
        cpu.state.FC = result > 0xFF; // Check if overflows to bit 8

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction ADC_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A; // Keep oldA value for flags
        uint16_t result = oldA + value + cpu.state.C;
        cpu.state.A = result & 0xFF; // Mask back to 8bit size

        cpu.state.FZ = (cpu.state.A == 0);
        cpu.state.FN = 0;
        cpu.state.FH = ((oldA & 0xF) + (value & 0xF)) > 0xF; // Check if overflows to bit 4 or higher (half carry)
        cpu.state.FC = result > 0xFF; // Check if overflows to bit 8

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction SUB_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A; // Keep oldA value for flags
        uint16_t result = oldA - value;
        cpu.state.A = result & 0xFF; // Mask back to 8bit size

        cpu.state.FZ = (cpu.state.A == 0);
        cpu.state.FN = 1;
        cpu.state.FH = ((oldA & 0xF) - (value & 0xF)) < 0xF; // Check if underflow from bit 4 lower (half borrow)
        cpu.state.FC = oldA < value; // Check if underflow from bit 8

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction SBC_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A; // Keep oldA value for flags
        uint16_t result = oldA - value - cpu.state.C;
        cpu.state.A = result & 0xFF; // Mask back to 8bit size

        cpu.state.FZ = (cpu.state.A == 0);
        cpu.state.FN = 1;
        cpu.state.FH = ((oldA & 0xF) - (value & 0xF)) < 0xF; // Check if underflow from bit 4 lower (half borrow)
        cpu.state.FC = oldA < value; // Check if underflow from bit 8

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction AND_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A; // Keep oldA value for flags
        uint16_t result = value & oldA;
        cpu.state.A = result & 0xFF; // Mask back to 8bit size

        cpu.state.FZ = (cpu.state.A == 0);
        cpu.state.FN = 0;
        cpu.state.FH = 1;
        cpu.state.FC = 0;

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction XOR_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A; // Keep oldA value for flags
        uint16_t result = value ^ oldA;
        cpu.state.A = result & 0xFF; // Mask back to 8bit size

        cpu.state.FZ = (cpu.state.A == 0);
        cpu.state.FN = 0;
        cpu.state.FH = 0;
        cpu.state.FC = 0;

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction OR_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A; // Keep oldA value for flags
        uint16_t result = value | oldA;
        cpu.state.A = result & 0xFF; // Mask back to 8bit size

        cpu.state.FZ = (cpu.state.A == 0);
        cpu.state.FN = 0;
        cpu.state.FH = 0;
        cpu.state.FC = 0;

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction CP_A_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t value = GetRegisterValue(cpu, instruction);

        uint8_t oldA = cpu.state.A; // Keep oldA value for flags
        uint16_t result = oldA - value;

        cpu.state.FZ = ((result & 0xFF) == 0); // Mask back to 8bit size
        cpu.state.FN = 1;
        cpu.state.FH = ((oldA & 0xF) - (value & 0xF)) < 0xF; // Check if underflow from bit 4 lower (half borrow)
        cpu.state.FC = oldA < value; // Check if underflow from bit 8

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction INC_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t dstIndex = GetRegister(cpu, instruction);

        if (dstIndex == 6) { // If 6 means nullptr = HL
            uint16_t HL = cpu.state.GetHL();
            uint8_t value = instruction.memory[HL];
            uint8_t oldValue = value;
            value++;
            instruction.memory[HL] = value;

            cpu.state.FZ = (value == 0);
            cpu.state.FN = 0;
            cpu.state.FH = ((oldValue & 0xF) + 1) > 0xF; // Check if overflows to bit 4 or higher (half carry)
        }
        else {
            uint8_t* dst = cpu.state.registerLookup[dstIndex];
            uint8_t oldR = *dst; // Keep old R value for flags
            *dst = oldR + 1;

            cpu.state.FZ = (*dst == 0);
            cpu.state.FN = 0;
            cpu.state.FH = ((oldR & 0xF) + 1) > 0xF; // Check if overflows to bit 4 or higher (half carry)
        }

        return CPU::CounterAction::Advance;
    }

    CPU::CounterAction DEC_R(CPU& cpu, CPU::Instruction instruction) {
        uint8_t dstIndex = GetRegister(cpu, instruction);

        if (dstIndex == 6) { // If 6 means nullptr = HL
            uint16_t HL = cpu.state.GetHL();
            uint8_t value = instruction.memory[HL];
            uint8_t oldValue = value;
            value--;
            instruction.memory[HL] = value;

            cpu.state.FZ = (value == 0);
            cpu.state.FN = 0;
            cpu.state.FH = ((oldValue & 0xF) - 1) < 0xF; // Check if underflow from bit 4 lower (half borrow)
        }
        else {
            uint8_t* dst = cpu.state.registerLookup[dstIndex];
            uint8_t oldR = *dst; // Keep old R value for flags
            *dst = oldR - 1;

            cpu.state.FZ = (*dst == 0);
            cpu.state.FN = 1;
            cpu.state.FH = ((oldR & 0xF) - 1) < 0xF; // Check if underflow from bit 4 lower (half borrow)
        }

        return CPU::CounterAction::Advance;
    }
}