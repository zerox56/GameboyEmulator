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

    // 0x00–0x0F
    table[0x00] = &CPU::NOP;
    table[0x06] = &CPU::LD_B_N8;
    table[0x0E] = &CPU::LD_C_N8;

    // 0x10–0x1F
    table[0x16] = &CPU::LD_D_N8;
    table[0x1E] = &CPU::LD_E_N8;

    // 0x20–0x2F
    table[0x26] = &CPU::LD_H_N8;
    table[0x2E] = &CPU::LD_L_N8;

    // 0xC0–0xCF
    table[0xC3] = &CPU::JP_A16;

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

    if (dst && src) {
        *dst = *src;
    }
    return CPU::CounterAction::Advance;
}

// Jump instructions
CPU::CounterAction CPU::JP_A16(Instruction instruction) {
    instruction.pc = (instruction.H << 8) | instruction.L;
    return CPU::CounterAction::Jump;
}

// Other instructions
CPU::CounterAction CPU::NOP(Instruction) {
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::UnimplementedOpcode(Instruction instruction) {
    printf("Unimplemented opcode\n");
    exit(1);
}

uint8_t CPU::GetBytesByOpcode(uint8_t opcode) {
    return instructionBytes[opcode];
}

void CPU::ExecuteOpcode(std::vector<uint8_t>& memory, uint16_t& pc) {
    Instruction instruction(pc);
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