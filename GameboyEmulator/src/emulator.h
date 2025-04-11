#pragma once
#include <cstdint>
#include <vector>

class Emulator {
private:
	// 8 MB of max memory
	static constexpr uint32_t maxMemory = 8 * 1024 * 1024;

	std::vector<uint8_t> memory;
	uint32_t pc = 0x100;
public:
	Emulator();
	bool LoadRom(const char* filePath);
	void Cycle();
};