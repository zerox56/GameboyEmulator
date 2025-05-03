#pragma once
#include <string>
using std::string;

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