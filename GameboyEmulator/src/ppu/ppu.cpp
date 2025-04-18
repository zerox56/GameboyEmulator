#include "ppu.h"

PPU::PPU() {}

void PPU::Update(std::vector<uint8_t>& memory, uint8_t cycles) {
	cycleCounter += cycles;

	UpdateMode(memory);
	UpdateSTAT(memory);	
}

void PPU::UpdateMode(std::vector<uint8_t>& memory) {
	switch (currentMode) {
		case PPU::PPUMode::OAMSearch: {
			if (cycleCounter >= 80) {
				currentMode = PPUMode::LCDTransfer;
				cycleCounter = 0;
			}
			break;
		}
		case PPUMode::LCDTransfer: {
			if (cycleCounter >= 172) {
				currentMode = PPUMode::HBlank;
				cycleCounter = 0;
			}
			break;
		}
		case PPUMode::HBlank: {
			if (cycleCounter >= 456) {
				uint8_t LY = memory[LYAddress];
				LY++;
				memory[LYAddress] = LY;
				currentMode = LY == 144 ? PPUMode::VBlank : PPUMode::OAMSearch;
				if (LY == 144) {
					memory[IFAddress] |= 1;
				}
				cycleCounter = 0;
			}
			break;
		}
		case PPUMode::VBlank: {
			if (cycleCounter >= 456) {
				uint8_t LY = memory[LYAddress];
				if (++LY > 153) {
					LY = 0;
					currentMode = PPUMode::OAMSearch;
				}
				memory[LYAddress] = LY;
				cycleCounter = 0;
			}
			break;
		}
	}
}

void PPU::UpdateSTAT(std::vector<uint8_t>& memory) {
	uint8_t STAT = memory[STATAddress];
	if (currentMode == PPUMode::HBlank || currentMode == PPUMode::OAMSearch) {
		STAT = (STAT & 0b11111100) | static_cast<uint8_t>(currentMode);  // Keep bits 2–7
	}
	if (memory[LYAddress] == memory[LYCAddress]) {
		STAT |= 0x04; // Set LYC=LY flag
		if (STAT & 0x40) {// LYC interrupt enable check
			memory[IFAddress] |= 0x02;
		}
	}
	else {
		STAT &= ~0x04; // Clear LYC=LY flag
	}

	if (currentMode == PPUMode::HBlank && (STAT & 0x08)) {
		memory[IFAddress] |= 0x02;
	}
	else if (currentMode == PPUMode::VBlank && (STAT & 0x10)) {
		memory[IFAddress] |= 0x02;
	}
	else if (currentMode == PPUMode::OAMSearch && (STAT & 0x20)) {
		memory[IFAddress] |= 0x02;
	}

	memory[STATAddress] = STAT;
}