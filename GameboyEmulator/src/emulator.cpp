#include "emulator.h"

#include <cstdint>
#include <fstream>

Emulator::Emulator() {
	memory.resize(maxMemory);
}

bool Emulator::LoadRom(const char* filePath) {
	std::ifstream romData(filePath, std::ios::binary | std::ios::ate);
	if (!romData) {
		printf("Problem reading ROM file.\n");
		return false;
	}
	std::streamsize size = romData.tellg();
	romData.seekg(0, std::ios::beg);

	if (size <= 0 || size > (maxMemory - 0x100)) {
		printf("ROM is too large or empty.\n");
		return false;
	}

	romData.read(reinterpret_cast<char*>(&memory[pc]), size);
	return true;
}

void Emulator::Cycle() {
	if (pc + 1 >= maxMemory) {
		printf("PC out of bounds: %04X\n", pc);
		return;
	}

	printf("PC: %04X\n", pc);

	pc += 2;
}