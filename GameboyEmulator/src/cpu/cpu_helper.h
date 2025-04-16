#pragma once
#include "cpu.h"

#include <cstdint>
#include <vector>

namespace CPUHelper {
	// Stack helpers
	void PUSH(uint16_t value, CPU& cpu, std::vector<uint8_t>& memory);
	uint16_t POP(CPU& cpu, std::vector<uint8_t>& memory);
};