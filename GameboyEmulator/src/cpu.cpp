#include "cpu.h"

#include <stdio.h>
#include <memory>

const std::vector<uint8_t> CPU::instructionBytes{
    // 0x00–0x0F
    1, 3, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    // 0x10–0x1F
    2, 3, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    // 0x20–0x2F
    2, 2, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    // 0x30–0x3F
    2, 3, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    // 0x40–0x4F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x50–0x5F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x60–0x6F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x70–0x7F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x80–0x8F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x90–0x9F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0xA0–0xAF
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0xB0–0xBF
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0xC0–0xCF
    1, 1, 3, 3, 3, 1, 3, 1, 1, 1, 3, 1, 1, 1, 2, 1,
    // 0xD0–0xDF
    1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 2, 1,
    // 0xE0–0xEF
    2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 3, 1,
    // 0xF0–0xFF
    2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 3, 1
};

CPU::CPU() : opcodeTable(InitializeOpcodeTable()) {}

std::vector<CPU::OpcodeFunc> CPU::InitializeOpcodeTable() {
    std::vector<OpcodeFunc> table(0x100, &CPU::UnimplementedOpcode);

    // Load R, R functions
    for (uint8_t opcode = 0x40; opcode <= 0x7F; opcode++) {
        opcodeTable[opcode] = &CPU::LD_R_R;
    }

    // ADD A, R functions
    for (uint8_t opcode = 0x80; opcode <= 0x87; opcode++) {
        opcodeTable[opcode] = &CPU::ADD_A_R;
    }

    // ADC A, R functions
    for (uint8_t opcode = 0x88; opcode <= 0x8F; opcode++) {
        opcodeTable[opcode] = &CPU::ADC_A_R;
    }

    // SUB A, R functions
    for (uint8_t opcode = 0x90; opcode <= 0x97; opcode++) {
        opcodeTable[opcode] = &CPU::SUB_A_R;
    }

    // SBC A, R functions
    for (uint8_t opcode = 0x98; opcode <= 0x9F; opcode++) {
        opcodeTable[opcode] = &CPU::SBC_A_R;
    }

    // AND A, R functions
    for (uint8_t opcode = 0xA0; opcode <= 0xA7; opcode++) {
        opcodeTable[opcode] = &CPU::AND_A_R;
    }

    // XOR A, R functions
    for (uint8_t opcode = 0xA8; opcode <= 0xAF; opcode++) {
        opcodeTable[opcode] = &CPU::XOR_A_R;
    }

    // OR A, R functions
    for (uint8_t opcode = 0xB0; opcode <= 0xB7; opcode++) {
        opcodeTable[opcode] = &CPU::OR_A_R;
    }

    // CP A, R functions
    for (uint8_t opcode = 0xB8; opcode <= 0xBF; opcode++) {
        opcodeTable[opcode] = &CPU::CP_A_R;
    }

    // 0x00–0x0F
    table[0x00] = &CPU::NOP;
    table[0x06] = &CPU::LD_B_N8;
    table[0x0E] = &CPU::LD_C_N8;

    // 0x10–0x1F
    table[0x16] = &CPU::LD_D_N8;
    table[0x18] = &CPU::JR_N16;
    table[0x1E] = &CPU::LD_E_N8;

    // 0x20–0x2F
    table[0x20] = &CPU::JR_NZ_N16;
    table[0x26] = &CPU::LD_H_N8;
    table[0x28] = &CPU::JR_Z_N16;
    table[0x2E] = &CPU::LD_L_N8;

    // 0xC0–0xCF
    table[0x30] = &CPU::JR_NC_N16;
    table[0x38] = &CPU::JR_C_N16;
    table[0xC3] = &CPU::JP_N16;

    return table;
}

// Load instructions
CPU::CounterAction CPU::LD_B_N8(Instruction instruction) {
    B = instruction.L;
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_C_N8(Instruction instruction) {
    C = instruction.L;
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_D_N8(Instruction instruction) {
    D = instruction.L;
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_E_N8(Instruction instruction) {
    E = instruction.L;
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_H_N8(Instruction instruction) {
    H = instruction.L;
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_L_N8(Instruction instruction) {
    L = instruction.L;
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_R_R(Instruction instruction) {
    // Mask opcode for dst and src register
    uint8_t dstIndex = (instruction.opcode >> 3) & 0b111;
    uint8_t srcIndex = instruction.opcode & 0b111;

    // Get register by index
    uint8_t* dst = registerLookup[dstIndex];
    uint8_t* src = registerLookup[srcIndex];

    uint16_t hl = (H << 8) | L;

    if (dstIndex == 6 && src) {
        instruction.memory[hl] = *src;
    }
    else if (srcIndex == 6 && dst) {
        *dst = instruction.memory[hl];
    }
    else if (dst && src) {
        *dst = *src;
    }

    return CPU::CounterAction::Advance;
}

// Jump instructions
CPU::CounterAction CPU::JumpRelative(Instruction instruction) {
    if (instruction.H == 0x00) {
        return CPU::CounterAction::Advance;
    }

    instruction.pc += (int8_t)instruction.H;

    return CPU::CounterAction::Jump;
}

CPU::CounterAction CPU::JP_N16(Instruction instruction) {
    instruction.pc = (instruction.H << 8) | instruction.L;
    return CPU::CounterAction::Jump;
}

CPU::CounterAction CPU::JR_N16(Instruction instruction) {
    return JumpRelative(instruction);
}

CPU::CounterAction CPU::JR_NZ_N16(Instruction instruction) {
    return FZ == 0 ? JumpRelative(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::JR_Z_N16(Instruction instruction) {
    return FZ == 1 ? JumpRelative(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::JR_NC_N16(Instruction instruction) {
    return FC == 0 ? JumpRelative(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::JR_C_N16(Instruction instruction) {
    return FC == 1 ? JumpRelative(instruction) : CPU::CounterAction::Advance;
}

// 8bit instructions 
uint8_t CPU::GetRegisterValue(Instruction instruction) {
    // Mask opcode with binary masking
    uint8_t srcIndex = instruction.opcode & 0b111;

    // Check the lookup table through the earlier index
    uint8_t* src = registerLookup[srcIndex];

    // Combines H and L registers to a 16-bit address
    uint16_t hl = (H << 8) | L;
    uint8_t value;

    if (srcIndex == 6) { // If 6 means nullptr = HL
        return instruction.memory[hl];
    }
    else {
        return *src;
    }
}

CPU::CounterAction CPU::ADD_A_R(Instruction instruction) {
    uint8_t value = GetRegisterValue(instruction);

    uint8_t oldA = A; // Keep oldA value for flags
    uint16_t result = oldA + value;
    A = result & 0xFF; // Mask back to 8bit size

    FZ = (A == 0);
    FN = 0;
    FH = ((oldA & 0xF) + (value & 0xF)) > 0xF; // Check if overflows to bit 4 or higher (half carry)
    FC = result > 0xFF; // Check if overflows to bit 8

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::ADC_A_R(Instruction instruction) {
    uint8_t value = GetRegisterValue(instruction);

    uint8_t oldA = A; // Keep oldA value for flags
    uint16_t result = oldA + value + C;
    A = result & 0xFF; // Mask back to 8bit size

    FZ = (A == 0);
    FN = 0;
    FH = ((oldA & 0xF) - (value & 0xF)) < 0xF; // Check if overflows to bit 4 or higher (half carry)
    FC = result > 0xFF; // Check if overflows to bit 8

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::SUB_A_R(Instruction instruction) {
    uint8_t value = GetRegisterValue(instruction);

    uint8_t oldA = A; // Keep oldA value for flags
    uint16_t result = oldA - value;
    A = result & 0xFF; // Mask back to 8bit size

    FZ = (A == 0);
    FN = 1;
    FH = ((oldA & 0xF) - (value & 0xF)) < 0xF; // Check if underflow from bit 4 lower (half borrow)
    FC = oldA < value; // Check if underflow from bit 8

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::SBC_A_R(Instruction instruction) {
    uint8_t value = GetRegisterValue(instruction);

    uint8_t oldA = A; // Keep oldA value for flags
    uint16_t result = oldA - value - C;
    A = result & 0xFF; // Mask back to 8bit size

    FZ = (A == 0);
    FN = 1;
    FH = ((oldA & 0xF) - (value & 0xF)) < 0xF; // Check if underflow from bit 4 lower (half borrow)
    FC = oldA < value; // Check if underflow from bit 8

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::AND_A_R(Instruction instruction) {
    uint8_t value = GetRegisterValue(instruction);

    uint8_t oldA = A; // Keep oldA value for flags
    uint16_t result = value & oldA;
    A = result & 0xFF; // Mask back to 8bit size

    FZ = (A == 0);
    FN = 0;
    FH = 1;
    FC = 0;

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::XOR_A_R(Instruction instruction) {
    uint8_t value = GetRegisterValue(instruction);

    uint8_t oldA = A; // Keep oldA value for flags
    uint16_t result = value ^ oldA;
    A = result & 0xFF; // Mask back to 8bit size

    FZ = (A == 0);
    FN = 0;
    FH = 0;
    FC = 0;

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::OR_A_R(Instruction instruction) {
    uint8_t value = GetRegisterValue(instruction);

    uint8_t oldA = A; // Keep oldA value for flags
    uint16_t result = value | oldA;
    A = result & 0xFF; // Mask back to 8bit size

    FZ = (A == 0);
    FN = 0;
    FH = 0;
    FC = 0;

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::CP_A_R(Instruction instruction) {
    uint8_t value = GetRegisterValue(instruction);

    uint8_t oldA = A; // Keep oldA value for flags
    uint16_t result = oldA - value;

    FZ = ((result & 0xFF) == 0); // Mask back to 8bit size
    FN = 1;
    FH = ((oldA & 0xF) - (value & 0xF)) < 0xF; // Check if underflow from bit 4 lower (half borrow)
    FC = oldA < value; // Check if underflow from bit 8

    return CPU::CounterAction::Advance;
}

// Other instructions
CPU::CounterAction CPU::NOP(Instruction) {
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::UnimplementedOpcode(Instruction instruction) {
    printf("Unimplemented opcode\n");
    exit(1);
}

uint8_t CPU::GetFlags() {
    return (FZ << 7) | (FN << 6) | (FH << 5) | (FC << 4);
}

uint8_t CPU::GetBytesByOpcode(uint8_t opcode) {
    return instructionBytes[opcode];
}

void CPU::ExecuteOpcode(std::vector<uint8_t>& memory, uint16_t& pc) {
    Instruction instruction(memory, pc);
    instruction.opcode = memory[pc];

    printf("PC: 0x%04X | Opcode: %02X\n", pc, instruction.opcode);

    uint8_t instructionBtyes = GetBytesByOpcode(instruction.opcode);
    if (instructionBtyes >= 2) {
        instruction.L = memory[pc + 1];
    }
    if (instructionBtyes == 3) {;
        instruction.H = memory[pc + 2];
    }

    CPU::CounterAction action = (this->*opcodeTable[instruction.opcode])(instruction);

    switch (action)
    {
        case CPU::CounterAction::Advance:
        default:
            pc += instructionBtyes;
            break;
        case CPU::CounterAction::Jump:
            break;
    }

    if (debugCycleCurrent++ >= debugCycleMax) {
        printf("A: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X\n", A, B, C, D, E, H, L);
        debugCycleCurrent = 0;
    }
}