#pragma once
#include <string>
using std::string;

#include <cpu/cpu.h>

inline CPU::Instruction CreateInstruction(uint8_t opcode) {
    static std::vector<uint8_t> dummyMemory(0x10000, 0);
    static uint16_t dummyPC = 0x100;
    dummyMemory[dummyPC] = opcode;

    CPU::Instruction instruction(dummyMemory, dummyPC);
    instruction.opcode = opcode;
    return instruction;
}