#pragma once
#include <string>
using std::string;

#include "doctest/doctest.h"
#include <cpu/cpu.h>

struct TestInstruction {
    std::vector<uint8_t> memory;
    uint16_t pc;
    CPU::Instruction instruction;

    TestInstruction(uint8_t opcode, uint16_t startPc = 0x100)
        : memory(0x10000, 0), pc(startPc), instruction(memory, pc) {
        memory[pc] = opcode;
        instruction.opcode = opcode;
    }

    TestInstruction(const TestInstruction&) = delete;
    TestInstruction& operator=(const TestInstruction&) = delete;
};

inline void CheckFlags(const CPU& cpu, bool Z, bool N, bool H, bool C) {
    CHECK(cpu.state.FZ == Z);
    CHECK(cpu.state.FN == N);
    CHECK(cpu.state.FH == H);
    CHECK(cpu.state.FC == C);
}
