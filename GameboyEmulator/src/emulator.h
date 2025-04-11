#pragma once
#include <cstdint>
#include <vector>

class Emulator {
private:
	// 8 MB of max memory
	const uint32_t maxMemory = 8 * 1024 * 1024;
	// Starting address of all ROMs
	const uint8_t startAddress = 0x100;
	// Starting header address of all ROMs
	const uint16_t startHeaderAddress = 0x104;

	// Static header all valid ROMs should have
	static const std::vector<uint8_t> header;

	std::vector<uint8_t> memory;
	uint16_t pc = startAddress;
public:
	Emulator();
	bool LoadRom(const char* filePath);
	bool ValidateHeader();
	void Cycle();
};