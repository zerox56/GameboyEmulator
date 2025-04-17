#include "emulator.h"
#include "cpu/cpu.h"

#include <cstdint>
#include <fstream>

const std::vector<uint8_t> Emulator::header{
	0xCE,0xED,0x66,0x66,0xCC,0x0D,0x00,0x0B,0x03,0x73,0x00,0x83,0x00,0x0C,0x00,0x0D,
	0x00,0x08,0x11,0x1F,0x88,0x89,0x00,0x0E,0xDC,0xCC,0x6E,0xE6,0xDD,0xDD,0xD9,0x99,
	0xBB,0xBB,0x67,0x63,0x6E,0x0E,0xEC,0xCC,0xDD,0xDC,0x99,0x9F,0xBB,0xB9,0x33,0x3E
};

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
	if (romData.gcount() != size) {
		printf("ROM isn't fully loaded into memory.\n");
		return false;
	}

	return true;
}

bool Emulator::ValidateHeader() {
	return std::equal(
		memory.begin() + startHeaderAddress,
		memory.begin() + startHeaderAddress + 48,
		header.begin()
	);
}

void Emulator::Cycle() {
	if (pc + 1 >= maxMemory) {
		printf("PC out of bounds: %02X\n", pc);
		return;
	}
	static CPU cpu;

	cpu.ExecuteOpcode(memory, pc);
	cpu.UpdateTimers(memory, cycles);
}