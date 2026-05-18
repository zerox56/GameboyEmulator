#pragma once
#include <string>
using std::string;

#include "doctest/doctest.h"
#include <cpu/cpu.h>

inline CPU::Instruction CreateInstruction(uint8_t opcode, std::vector<uint8_t>& dummyMemory) {
    static uint16_t dummyPC = 0x100;
    dummyMemory[dummyPC] = opcode;

    CPU::Instruction instruction(dummyMemory, dummyPC);
    instruction.opcode = opcode;
    return instruction;
}

inline CPU::Instruction CreateInstruction(uint8_t opcode) {
    std::vector<uint8_t> dummyMemory(0x10000, 0);

    return CreateInstruction(opcode, dummyMemory);
}

inline void CheckFlags(const CPU& cpu, bool Z, bool N, bool H, bool C) {
    CHECK(cpu.state.FZ == Z);
    CHECK(cpu.state.FN == N);
    CHECK(cpu.state.FH == H);
    CHECK(cpu.state.FC == C);
}