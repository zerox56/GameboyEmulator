#pragma once
#include <cstdint>
#include <vector>

class PPU {
public:
	PPU();
	void Update(std::vector<uint8_t>& memory, uint8_t cycles);
private:
	enum class PPUMode {
		HBlank = 0,
		VBlank = 1,
		OAMSearch = 2,
		LCDTransfer = 3,
	};
	std::vector<uint8_t> display[160 * 144];

	uint8_t cycleCounter = 0;
	PPUMode currentMode = PPUMode::HBlank;

	// Scanlines
	uint8_t scanLines = 153;
	uint8_t LYAddress = 0xFF44;
	uint8_t LYCAddress = 0xFF45;

	// Interrupt Flag
	uint8_t IFAddress = 0xFF0F;

	uint8_t STATAddress = 0xFF41;

	void UpdateMode(std::vector<uint8_t>& memory);
	void UpdateSTAT(std::vector<uint8_t>& memory);
};