#include "cpu.h"
#include "modules/cpu_8bit_arithmetic.h"
#include "modules/cpu_16bit_arithmetic.h"
#include "modules/cpu_bit_shift.h"
#include "modules/cpu_carry_flag.h"
#include "modules/cpu_interrupt.h"
#include "modules/cpu_jump.h"
#include "modules/cpu_load.h"
#include "modules/cpu_misc.h"
#include "modules/cpu_stack.h"

#include <stdio.h>
#include <memory>
#include "cpu_helper.h"

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
    std::vector<CPU::OpcodeFunc> table(256);

    // Load R, R functions
    for (uint8_t opcode = 0x40; opcode <= 0x7F; opcode++) {
        opcodeTable[opcode] = CPULoad::LD_R_R;
    }

    // ADD A, R functions
    for (uint8_t opcode = 0x80; opcode <= 0x87; opcode++) {
        opcodeTable[opcode] = CPU8BitArithmetic::ADD_A_R;
    }

    // ADC A, R functions
    for (uint8_t opcode = 0x88; opcode <= 0x8F; opcode++) {
        opcodeTable[opcode] = CPU8BitArithmetic::ADC_A_R;
    }

    // SUB A, R functions
    for (uint8_t opcode = 0x90; opcode <= 0x97; opcode++) {
        opcodeTable[opcode] = CPU8BitArithmetic::SUB_A_R;
    }

    // SBC A, R functions
    for (uint8_t opcode = 0x98; opcode <= 0x9F; opcode++) {
        opcodeTable[opcode] = CPU8BitArithmetic::SBC_A_R;
    }

    // AND A, R functions
    for (uint8_t opcode = 0xA0; opcode <= 0xA7; opcode++) {
        opcodeTable[opcode] = CPU8BitArithmetic::AND_A_R;
    }

    // XOR A, R functions
    for (uint8_t opcode = 0xA8; opcode <= 0xAF; opcode++) {
        opcodeTable[opcode] = CPU8BitArithmetic::XOR_A_R;
    }

    // OR A, R functions
    for (uint8_t opcode = 0xB0; opcode <= 0xB7; opcode++) {
        opcodeTable[opcode] = CPU8BitArithmetic::OR_A_R;
    }

    // CP A, R functions
    for (uint8_t opcode = 0xB8; opcode <= 0xBF; opcode++) {
        opcodeTable[opcode] = CPU8BitArithmetic::CP_A_R;
    }

    // INC R and DEC R functions
    for (uint8_t index = 0; index < 8; index++) {
        uint8_t offset = CPU8BitArithmetic::EightFunctionOffset * index;
        opcodeTable[CPU8BitArithmetic::EightINCStart + offset] = CPU8BitArithmetic::INC_R;
        opcodeTable[CPU8BitArithmetic::EightDECStart + offset] = CPU8BitArithmetic::DEC_R;
    }

    // RST functions
    for (uint8_t index = 0; index < 8; index++) {
        uint8_t offset = CPUJump::RSTOffset * index;
        opcodeTable[CPUJump::RSTStart + offset] = CPUJump::RST;
    }

    // 0x00–0x0F
    table[0x00] = CPUMisc::NOP;
    table[0x01] = CPULoad::LD_BC_N16;
    table[0x02] = CPULoad::LD_BC_A;
    table[0x03] = CPU16BitArithmetic::INC_BC;
    table[0x06] = CPULoad::LD_B_N8;
    table[0x0A] = CPULoad::LD_A_BC;
    table[0x0B] = CPU16BitArithmetic::DEC_BC;
    table[0x0E] = CPULoad::LD_C_N8;

    // 0x10–0x1F
    table[0x11] = CPULoad::LD_DE_N16;
    table[0x12] = CPULoad::LD_DE_A;
    table[0x13] = CPU16BitArithmetic::INC_DE;
    table[0x16] = CPULoad::LD_D_N8;
    table[0x18] = CPUJump::JR_N16;
    table[0x1A] = CPULoad::LD_A_DE;
    table[0x1B] = CPU16BitArithmetic::DEC_DE;
    table[0x1E] = CPULoad::LD_E_N8;

    // 0x20–0x2F
    table[0x20] = CPUJump::JR_NZ_N16;
    table[0x21] = CPULoad::LD_HL_N16;
    table[0x22] = CPULoad::LD_INC_HL_A;
    table[0x23] = CPU16BitArithmetic::INC_HL;
    table[0x26] = CPULoad::LD_H_N8;
    table[0x27] = CPUMisc::DAA;
    table[0x28] = CPUJump::JR_Z_N16;
    table[0x2A] = CPULoad::LD_A_INC_HL;
    table[0x2B] = CPU16BitArithmetic::DEC_HL;
    table[0x2E] = CPULoad::LD_L_N8;

    // 0x30–0x3F
    table[0x30] = CPUJump::JR_NC_N16;
    table[0x31] = CPULoad::LD_SP_N16;
    table[0x32] = CPULoad::LD_DEC_HL_A;
    table[0x33] = CPU16BitArithmetic::INC_SP;
    table[0x36] = CPULoad::LD_HL_N8;
    table[0x37] = CPUCarryFlag::SCF;
    table[0x38] = CPUJump::JR_C_N16;
    table[0x3A] = CPULoad::LD_A_DEC_HL;
    table[0x3B] = CPU16BitArithmetic::DEC_SP;
    table[0x3E] = CPULoad::LD_A_N8;
    table[0x3F] = CPUCarryFlag::CCF;

    // 0x70–0x7F
    table[0x76] = CPUInterrupt::HALT;

    // 0xC0–0xCF
    table[0xC0] = CPUJump::RET_NZ;
    table[0xC1] = CPUStack::POP_BC;
    table[0xC3] = CPUJump::JP_N16;
    table[0xC4] = CPUJump::CALL_NZ_N16;
    table[0xC5] = CPUStack::PUSH_BC;
    table[0xC8] = CPUJump::RET_Z;
    table[0xC9] = CPUJump::RET;
    table[0xCB] = CPUBitShift::BitShift;
    table[0xCC] = CPUJump::CALL_Z_N16;
    table[0xCD] = CPUJump::CALL_N16;

    // 0xD0–0xDF
    table[0xD0] = CPUJump::RET_NC;
    table[0xD1] = CPUStack::POP_DE;
    table[0xD4] = CPUJump::CALL_NC_N16;
    table[0xD5] = CPUStack::PUSH_DE;
    table[0xD8] = CPUJump::RET_C;
    table[0xD9] = CPUJump::RETI;
    table[0xDC] = CPUJump::CALL_C_N16;

    // 0xE0–0xEF
    table[0xD0] = CPULoad::LDH_N8_A;
    table[0xD2] = CPULoad::LDH_C_A;
    table[0xE1] = CPUStack::POP_HL;
    table[0xE5] = CPUStack::PUSH_HL;
    table[0xEA] = CPULoad::LD_N16_A;

    // 0xF0–0xFF
    table[0xF0] = CPULoad::LDH_A_N8;
    table[0xF1] = CPUStack::POP_AF;
    table[0xF2] = CPULoad::LDH_A_C;
    table[0xF3] = CPUInterrupt::DI;
    table[0xF5] = CPUStack::PUSH_AF;
    table[0xFA] = CPULoad::LD_A_N16;
    table[0xFB] = CPUInterrupt::EI;

    return table;
}

void CPU::ExecuteOpcode(std::vector<uint8_t>& memory, uint16_t& pc) {
    InterruptCPU(memory);

    if (state.halted) {
        if (state.GetInterruptFlags(memory) != 0) {
            state.halted = false;
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
    if (instructionBytes == 3) {
        ;
        instruction.H = memory[pc + 2];
    }

    CounterAction action = opcodeTable[instruction.opcode](*this, instruction);

    if (action != CPU::CounterAction::AdvanceSkipIME && state.pendingIME) {
        state.IME = 1;
        state.pendingIME = false;
    }
    if (action == CPU::CounterAction::BitShift) {
        Instruction bitShiftInstruction(memory, pc);
        bitShiftInstruction.opcode = memory[++pc];

        printf("BitShift: PC: 0x%04X | Opcode: %02X\n", pc, instruction.opcode);

        CPUBitShift::ExecuteBitShift(*this, instruction);
    }

    switch (action)
    {
        case CPU::CounterAction::Advance:
        case CPU::CounterAction::AdvanceSkipIME:
        case CPU::CounterAction::BitShift:
        default:
            pc += instructionBytes;
            break;
        case CPU::CounterAction::Jump:
        case CPU::CounterAction::Wait:
            break;
    }

    if (debugCycleCurrent++ >= debugCycleMax) {
        printf("A: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X\n", 
            state.A, state.B, state.C, state.D, state.E, state.H, state.L);
        debugCycleCurrent = 0;
    }
}

uint8_t CPU::GetBytesByOpcode(uint8_t opcode) {
    return instructionBytes[opcode];
}

void CPU::InterruptCPU(std::vector<uint8_t>& memory) {
    uint8_t interruptFlags = state.GetInterruptFlags(memory);
    if (state.IME == 0 || interruptFlags == 0) {
        return;
    }
    uint8_t lsb = interruptFlags & -interruptFlags;
    CPUHelper::PUSH(vectorJumps.at(lsb), *this, memory);
    state.SetIF(lsb, memory);
    state.IME = 0;
}