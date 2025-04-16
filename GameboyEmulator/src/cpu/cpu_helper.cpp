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
}