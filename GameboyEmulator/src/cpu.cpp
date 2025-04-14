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

const std::map<uint8_t, uint8_t> CPU::vectorJumps {
    { 0x01, 0x40 }, { 0x02, 0x48 }, { 0x04, 0x50 }, { 0x08, 0x58 }, { 0x10, 0x60 },
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

    // INC R and DEC R functions
    for (uint8_t index = 0; index < 8; index++) {
        uint8_t offset = EightFunctionOffset * index;
        opcodeTable[EightINCStart + offset] = &CPU::INC_R;
        opcodeTable[EightDECStart + offset] = &CPU::DEC_R;
    }

    // RST functions
    for (uint8_t index = 0; index < 8; index++) {
        uint8_t offset = RSTOffset * index;
        opcodeTable[RSTStart + offset] = &CPU::RST;
    }

    // 0x00–0x0F
    table[0x00] = &CPU::NOP;
    table[0x01] = &CPU::LD_BC_N16;
    table[0x02] = &CPU::LD_BC_A;
    table[0x03] = &CPU::INC_BC;
    table[0x06] = &CPU::LD_B_N8;
    table[0x0A] = &CPU::LD_A_BC;
    table[0x0B] = &CPU::DEC_BC;
    table[0x0E] = &CPU::LD_C_N8;

    // 0x10–0x1F
    table[0x11] = &CPU::LD_DE_N16;
    table[0x12] = &CPU::LD_DE_A;
    table[0x13] = &CPU::INC_DE;
    table[0x16] = &CPU::LD_D_N8;
    table[0x18] = &CPU::JR_N16;
    table[0x1A] = &CPU::LD_A_DE;
    table[0x1B] = &CPU::DEC_DE;
    table[0x1E] = &CPU::LD_E_N8;

    // 0x20–0x2F
    table[0x20] = &CPU::JR_NZ_N16;
    table[0x21] = &CPU::LD_HL_N16;
    table[0x22] = &CPU::LD_INC_HL_A;
    table[0x23] = &CPU::INC_HL;
    table[0x26] = &CPU::LD_H_N8;
    table[0x28] = &CPU::JR_Z_N16;
    table[0x2A] = &CPU::LD_A_INC_HL;
    table[0x2B] = &CPU::DEC_HL;
    table[0x2E] = &CPU::LD_L_N8;

    // 0x30–0x3F
    table[0x30] = &CPU::JR_NC_N16;
    table[0x31] = &CPU::LD_SP_N16;
    table[0x32] = &CPU::LD_DEC_HL_A;
    table[0x33] = &CPU::INC_SP;
    table[0x38] = &CPU::JR_C_N16;
    table[0x3A] = &CPU::LD_A_DEC_HL;
    table[0x3B] = &CPU::DEC_SP;

    // 0x70–0x7F
    table[0x76] = &CPU::HALT;

    // 0xC0–0xCF
    table[0xC0] = &CPU::RET_NZ;
    table[0xC1] = &CPU::POP_BC;
    table[0xC3] = &CPU::JP_N16;
    table[0xC4] = &CPU::CALL_NZ_N16;
    table[0xC5] = &CPU::PUSH_BC;
    table[0xC8] = &CPU::RET_Z;
    table[0xC9] = &CPU::RET;
    table[0xCC] = &CPU::CALL_Z_N16;
    table[0xCD] = &CPU::CALL_N16;

    // 0xD0–0xDF
    table[0xD0] = &CPU::RET_NC;
    table[0xD1] = &CPU::POP_DE;
    table[0xD4] = &CPU::CALL_NC_N16;
    table[0xD5] = &CPU::PUSH_DE;
    table[0xD8] = &CPU::RET_C;
    table[0xD9] = &CPU::RETI;
    table[0xDC] = &CPU::CALL_C_N16;

    // 0xE0–0xEF
    table[0xE1] = &CPU::POP_HL;
    table[0xE5] = &CPU::PUSH_HL;

    // 0xF0–0xFF
    table[0xF1] = &CPU::POP_AF;
    table[0xF3] = &CPU::DI;
    table[0xF5] = &CPU::PUSH_AF;
    table[0xFB] = &CPU::EI;

    return table;
}

uint8_t CPU::GetInterruptFlags(std::vector<uint8_t>& memory) {
    return memory[IEAddress] & memory[IFAddress];
}

void CPU::SetIF(uint8_t value, std::vector<uint8_t>& memory) {
    memory[IFAddress] &= ~value;
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

    if (dstIndex == 6 && src) {
        instruction.memory[GetHL()] = *src;
    }
    else if (srcIndex == 6 && dst) {
        *dst = instruction.memory[GetHL()];
    }
    else if (dst && src) {
        *dst = *src;
    }

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_BC_A(Instruction instruction) {
    SetBC(instruction.memory[A]);
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_DE_A(Instruction instruction) {
    SetDE(instruction.memory[A]);
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_A_BC(Instruction instruction) {
    uint16_t BC = GetBC();
    A = instruction.memory[BC];
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_A_DE(Instruction instruction) {
    uint16_t DE = GetDE();
    A = instruction.memory[DE];
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_INC_HL_A(Instruction instruction) {
    uint16_t HL = GetHL();
    instruction.memory[HL++] = A;
    SetHL(++HL);
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_DEC_HL_A(Instruction instruction) {
    uint16_t HL = GetHL();
    instruction.memory[HL--] = A;
    SetHL(--HL);
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_A_INC_HL(Instruction instruction) {
    uint16_t HL = GetHL();
    A = instruction.memory[HL];
    SetHL(++HL);
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_A_DEC_HL(Instruction instruction) {
    uint16_t HL = GetHL();
    A = instruction.memory[HL];
    SetHL(--HL);
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_BC_N16(Instruction instruction) {
    SetBC((instruction.H << 8) | instruction.L);
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_DE_N16(Instruction instruction) {
    SetDE((instruction.H << 8) | instruction.L);
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_HL_N16(Instruction instruction) {
    uint16_t HL = GetHL();
    instruction.memory[HL] = (instruction.H << 8) | instruction.L;;
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::LD_SP_N16(Instruction instruction) {
    SP = (instruction.H << 8) | instruction.L;;
    return CPU::CounterAction::Advance;
}

// Jump instructions
CPU::CounterAction CPU::JP_N16(Instruction instruction) {
    instruction.pc = (instruction.H << 8) | instruction.L;
    return CPU::CounterAction::Jump;
}

CPU::CounterAction CPU::JR_N16(Instruction instruction) {
    if (instruction.H == 0x00) {
        return CPU::CounterAction::Advance;
    }

    instruction.pc += (int8_t)instruction.H;

    return CPU::CounterAction::Jump;
}

CPU::CounterAction CPU::JR_NZ_N16(Instruction instruction) {
    return FZ == 0 ? JR_N16(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::JR_Z_N16(Instruction instruction) {
    return FZ == 1 ? JR_N16(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::JR_NC_N16(Instruction instruction) {
    return FC == 0 ? JR_N16(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::JR_C_N16(Instruction instruction) {
    return FC == 1 ? JR_N16(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::RST(Instruction instruction) {
    uint16_t returnAddress = instruction.pc + 1;
    PUSH(returnAddress, instruction.memory);

    // TODO: Check if more clever way to do this later.
    switch (instruction.opcode) {
        case 0xC7:
            instruction.pc = 0x00;
            break;
        case 0xCF:
            instruction.pc = 0x08;
            break;
        case 0xD7:
            instruction.pc = 0x10;
            break;
        case 0xDF:
            instruction.pc = 0x18;
            break;
        case 0xE7:
            instruction.pc = 0x20;
            break;
        case 0xEF:
            instruction.pc = 0x28;
            break;
        case 0xF7:
            instruction.pc = 0x30;
            break;
        case 0xFF:
            instruction.pc = 0x38;
            break;
    }
    return CPU::CounterAction::Jump;
}

CPU::CounterAction CPU::DI(Instruction instruction) {
    IME = 0;
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::EI(Instruction instruction) {
    pendingIME = true;
    return CPU::CounterAction::AdvanceSkipIME;
}

// Subroutine instructions
CPU::CounterAction CPU::CALL_N16(Instruction instruction) {
    uint16_t returnAddress = instruction.pc + 3;
    PUSH(returnAddress, instruction.memory);
    return JP_N16(instruction);
}

CPU::CounterAction CPU::CALL_NZ_N16(Instruction instruction) {
    return FZ == 0 ? CALL_N16(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::CALL_Z_N16(Instruction instruction) {
    return FZ == 1 ? CALL_N16(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::CALL_NC_N16(Instruction instruction) {
    return FC == 0 ? CALL_N16(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::CALL_C_N16(Instruction instruction) {
    return FC == 1 ? CALL_N16(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::RET(Instruction instruction) {
    instruction.pc = POP(instruction.memory);
    return CPU::CounterAction::Wait;
}

CPU::CounterAction CPU::RET_NZ(Instruction instruction) {
    return FZ == 0 ? RET(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::RET_Z(Instruction instruction) {
    return FZ == 1 ? RET(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::RET_NC(Instruction instruction) {
    return FC == 0 ? RET(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::RET_C(Instruction instruction) {
    return FC == 1 ? RET(instruction) : CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::RETI(Instruction instruction) {
    instruction.pc = POP(instruction.memory);
    pendingIME = true;
    return CPU::CounterAction::AdvanceSkipIME;
}

// 8bit instructions 
uint8_t CPU::GetRegister(Instruction instruction) {
    // Mask opcode with binary masking
    return instruction.opcode & 0b111;
}

uint8_t CPU::GetRegisterValue(Instruction instruction) {
    uint8_t srcIndex = GetRegister(instruction);

    // Check the lookup table through the earlier index
    uint8_t* src = registerLookup[srcIndex];

    if (srcIndex == 6) { // If 6 means nullptr = HL
        return instruction.memory[GetHL()];
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
    FH = ((oldA & 0xF) + (value & 0xF)) > 0xF; // Check if overflows to bit 4 or higher (half carry)
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

CPU::CounterAction CPU::INC_R(Instruction instruction) {
    uint8_t dstIndex = GetRegister(instruction);

    if (dstIndex == 6) { // If 6 means nullptr = HL
        uint16_t HL = GetHL();
        uint8_t value = instruction.memory[HL];
        uint8_t oldValue = value;
        value++;
        instruction.memory[HL] = value;

        FZ = (value == 0);
        FN = 0;
        FH = ((oldValue & 0xF) + 1) > 0xF; // Check if overflows to bit 4 or higher (half carry)
    }
    else {
        uint8_t* dst = registerLookup[dstIndex];
        uint8_t oldR = *dst; // Keep old R value for flags
        *dst = oldR + 1;

        FZ = (*dst == 0);
        FN = 0;
        FH = ((oldR & 0xF) + 1) > 0xF; // Check if overflows to bit 4 or higher (half carry)
    }

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::DEC_R(Instruction instruction) {
    uint8_t dstIndex = GetRegister(instruction);

    if (dstIndex == 6) { // If 6 means nullptr = HL
        uint16_t HL = GetHL();
        uint8_t value = instruction.memory[HL];
        uint8_t oldValue = value;
        value--;
        instruction.memory[HL] = value;

        FZ = (value == 0);
        FN = 0;
        FH = ((oldValue & 0xF) - 1) < 0xF; // Check if underflow from bit 4 lower (half borrow)
    }
    else {
        uint8_t* dst = registerLookup[dstIndex];
        uint8_t oldR = *dst; // Keep old R value for flags
        *dst = oldR - 1;

        FZ = (*dst == 0);
        FN = 1;
        FH = ((oldR & 0xF) - 1) < 0xF; // Check if underflow from bit 4 lower (half borrow)
    }

    return CPU::CounterAction::Advance;
}

// 16bit instructions 
CPU::CounterAction CPU::INC_BC(Instruction instruction) {
    uint16_t BC = GetBC();
    SetBC(++BC);

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::INC_DE(Instruction instruction) {
    uint16_t DE = GetDE();
    SetDE(++DE);

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::INC_HL(Instruction instruction) {
    uint16_t HL = GetHL();
    SetHL(++HL);

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::INC_SP(Instruction instruction) {
    SP++;
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::DEC_BC(Instruction instruction) {
    uint16_t BC = GetBC();
    SetBC(--BC);

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::DEC_DE(Instruction instruction) {
    uint16_t DE = GetDE();
    SetDE(--DE);

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::DEC_HL(Instruction instruction) {
    uint16_t HL = GetHL();
    SetHL(--HL);

    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::DEC_SP(Instruction instruction) {
    SP--;
    return CPU::CounterAction::Advance;
}

// Stack instructions
void CPU::PUSH(uint16_t value, std::vector<uint8_t>& memory) {
    memory[--SP] = value >> 8;
    memory[--SP] = value & 0xFF;
}

uint16_t CPU::POP(std::vector<uint8_t>& memory) {
    uint8_t low = memory[SP++];
    uint8_t high = memory[SP++];
    return (high << 8) | low;
}

CPU::CounterAction CPU::PUSH_BC(Instruction instruction) {
    PUSH(GetBC(), instruction.memory);
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::POP_BC(Instruction instruction) {
    SetBC(POP(instruction.memory));
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::PUSH_DE(Instruction instruction) {
    PUSH(GetHL(), instruction.memory);
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::POP_DE(Instruction instruction) {
    SetHL(POP(instruction.memory));
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::PUSH_HL(Instruction instruction) {
    PUSH(GetHL(), instruction.memory);
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::POP_HL(Instruction instruction) {
    SetHL(POP(instruction.memory));
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::PUSH_AF(Instruction instruction) {
    PUSH(GetAF(), instruction.memory);
    return CPU::CounterAction::Advance;
}

CPU::CounterAction CPU::POP_AF(Instruction instruction) {
    SetAF(POP(instruction.memory));
    return CPU::CounterAction::Advance;
}

// Interrupt functions
CPU::CounterAction CPU::HALT(Instruction instruction) {
    if (IME == 1 || IME == 0 && GetInterruptFlags(instruction.memory) == 0) {
        halted = true;
        return CPU::CounterAction::Advance;
    }
    else {
        return CPU::CounterAction::Advance;
    }
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

void CPU::InterruptCPU(std::vector<uint8_t>& memory) {
    uint8_t interruptFlags = GetInterruptFlags(memory);
    if (IME == 0 || interruptFlags == 0) {
        return;
    }
    uint8_t lsb = interruptFlags & -interruptFlags;
    PUSH(vectorJumps.at(lsb), memory);
    SetIF(lsb, memory);
    IME = 0;
}

void CPU::ExecuteOpcode(std::vector<uint8_t>& memory, uint16_t& pc) {
    InterruptCPU(memory);

    if (halted) {
        if (GetInterruptFlags(memory) != 0) {
            halted = false;
        }
        else {
            return;
        }
    }

    Instruction instruction(memory, pc);
    instruction.opcode = memory[pc];

    printf("PC: 0x%04X | Opcode: %02X\n", pc, instruction.opcode);

    uint8_t instructionBytes = GetBytesByOpcode(instruction.opcode);
    if (instructionBytes >= 2) {
        instruction.L = memory[pc + 1];
    }
    if (instructionBytes == 3) {;
        instruction.H = memory[pc + 2];
    }

    CPU::CounterAction action = (this->*opcodeTable[instruction.opcode])(instruction);

    if (action != CPU::CounterAction::AdvanceSkipIME && pendingIME) {
        IME = 1;
        pendingIME = false;
    }

    switch (action)
    {
        case CPU::CounterAction::Advance:
        case CPU::CounterAction::AdvanceSkipIME:
        default:
            pc += instructionBtyes;
            break;
        case CPU::CounterAction::Jump:
        case CPU::CounterAction::Wait:
            break;
    }

    if (debugCycleCurrent++ >= debugCycleMax) {
        printf("A: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X\n", A, B, C, D, E, H, L);
        debugCycleCurrent = 0;
    }
}