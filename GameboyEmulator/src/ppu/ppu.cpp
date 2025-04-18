#include "ppu.h"

PPU::PPU() {
	display.resize(screenWidth * screenHeigth);
}

void PPU::Update(std::vector<uint8_t>& memory, uint8_t cycles) {
	cycleCounter += cycles;

	UpdateMode(memory);
	UpdateSTAT(memory);	
	FetchBackground(memory);
	FetchWindow(memory);
	DrawSprites(memory);
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

uint8_t PPU::GetTileDataAddress(std::vector<uint8_t>& memory, uint8_t column, uint8_t row, uint8_t pixelInTile,
	uint16_t tileMapBaseAddress, bool isTileUnsigned) {
	uint8_t tileMapIndex = row * 32 + column;
	uint8_t	tileId = memory[tileMapBaseAddress + tileMapIndex];
	uint16_t tileBaseAddress = isTileUnsigned ? unsignedTilesStart : signedTilesStart;

	if (isTileUnsigned) {
		tileId *= 16;
	}
	else {
		tileId = (int8_t)tileId * 16;
	}

	uint8_t pixelRowIndex = pixelInTile % 8;
	return tileBaseAddress + tileId + (pixelRowIndex * 2);
}

uint8_t PPU::GetColorValue(std::vector<uint8_t>& memory, uint8_t tileDataAddress, uint8_t xBit) {
	uint8_t lowBitPlane = memory[tileDataAddress];
	uint8_t highBitPlane = memory[tileDataAddress + 1];

	uint8_t low = (lowBitPlane >> xBit) & 1;
	uint8_t high = (highBitPlane >> xBit) & 1;

	uint8_t colorId = (high << 1) | low;
	return (memory[BGPAddress] >> (colorId * 2)) & 3;;
}

uint8_t PPU::GetSpriteColorValue(std::vector<uint8_t>& memory, uint8_t tileDataAddress, uint8_t xBit, bool useOBP1) {
	uint8_t lowBitPlane = memory[tileDataAddress];
	uint8_t highBitPlane = memory[tileDataAddress + 1];

	uint8_t low = (lowBitPlane >> xBit) & 1;
	uint8_t high = (highBitPlane >> xBit) & 1;

	uint8_t colorId = (high << 1) | low;

	if (colorId == 0) return 0;

	uint16_t paletteAddress = useOBP1 ? OBP1StartAddress : OBP0StartAddress;
	return (memory[paletteAddress] >> (colorId * 2)) & 3;
}

void PPU::FetchBackground(std::vector<uint8_t>& memory) {
	uint8_t SCX = memory[SCXAddress];
	uint8_t SCY = memory[SCYAddress];
	uint8_t LY = memory[LYAddress];
	uint8_t LCDC = memory[LCDCAddress];
	uint8_t pixelInTile = SCY + LY;

	bool useTileMap1 = (LCDC >> 3) & 1;
	uint16_t tileMapBaseAddress = useTileMap1 ? tileMap1Start : tileMap0Start;
	bool isTileUnsigned = (LCDC >> 4) & 1;

	for (uint8_t x = 0; x < 160; x++) {
		uint8_t column = (SCX + x) / 8;
		uint8_t row = (SCY + LY) / 8;

		uint8_t tileDataAddress = GetTileDataAddress(memory, column, row, pixelInTile, tileMapBaseAddress, isTileUnsigned);

		uint8_t xBit = (7 - (SCX + x) % 8);
		uint8_t colorValue = GetColorValue(memory, tileDataAddress, xBit);
		display[LY * screenWidth + x] = colorValue;
	}
}

void PPU::FetchWindow(std::vector<uint8_t>& memory) {
	uint8_t LCDC = memory[LCDCAddress];
	bool renderWindow = (LCDC >> 5) & 1;
	if (!renderWindow) return;

	uint8_t WX = memory[WXAddress];
	uint8_t WY = memory[WYAddress];
	uint8_t LY = memory[LYAddress];

	if (LY < WY) return;

	bool useTileMap1 = (LCDC >> 6) & 1;
	uint16_t tileMapBaseAddress = useTileMap1 ? tileMap1Start : tileMap0Start;
	bool isTileUnsigned = (LCDC >> 4) & 1;
	uint8_t line = LY - WY;

	for (uint8_t x = WX - 7; x < 160; x++) {
		uint8_t column = (x - WX - 7) / 8;
		uint8_t row = line / 8;
		uint8_t pixelInTile = (x - (WX - 7)) % 8;

		uint8_t tileDataAddress = GetTileDataAddress(memory, column, row, pixelInTile, tileMapBaseAddress, isTileUnsigned);

		uint8_t lowBitPlane = memory[tileDataAddress];
		uint8_t highBitPlane = memory[tileDataAddress + 1];

		uint8_t xBit = 7 - pixelInTile;
		uint8_t colorValue = GetColorValue(memory, tileDataAddress, xBit);
		display[LY * screenWidth + x] = colorValue;
	}
}

void PPU::DrawSprites(std::vector<uint8_t>& memory) {
	uint8_t LCDC = memory[LCDCAddress];
	uint8_t LY = memory[LYAddress];
	bool objDoubleHeight = (LCDC >> 2) & 1;
	std::vector<Sprite> visibleSprites;

	for (uint8_t i = 0; i < 40; i++) {
		uint16_t oamAddress = OAMStartAddress + (i * spriteSize);
		Sprite sprite;
		sprite.y = memory[oamAddress] - 16;
		sprite.x = memory[oamAddress + 1] - 8;
		sprite.tileId = memory[oamAddress + 2];
		sprite.attributes = memory[oamAddress + 3];

		if (objDoubleHeight) {
			sprite.tileId &= 0xFE;
		}

		if (LY >= sprite.y && LY < sprite.y + 16) {
			visibleSprites.push_back(sprite);
		}

		if (visibleSprites.size() == 10) break;
	}


	for (uint8_t spriteIndex = 0; spriteIndex < visibleSprites.size(); spriteIndex++) {
		Sprite sprite = visibleSprites[spriteIndex];
		uint8_t yInTile = LY - sprite.y;
		bool useOBP1 = sprite.attributes & 0x10;

		bool isTileUnsigned = (LCDC >> 4) & 1;
		uint16_t tileBaseAddress = isTileUnsigned ? unsignedTilesStart : signedTilesStart;

		uint16_t tileAddress = tileBaseAddress + sprite.tileId * 16;
		uint16_t tileDataAddress = tileAddress + yInTile * 2;

		for (uint8_t x = 0; x < 8; x++) {
			uint8_t xPixel = sprite.x + x;
			if (xPixel >= 160) continue;

			uint8_t xBit = 7 - x;
			uint8_t colorValue = GetSpriteColorValue(memory, tileDataAddress, xBit, useOBP1);

			if (colorValue == 0) continue;

			bool backgroundPriority = sprite.attributes & 0x80;
			if (backgroundPriority && display[LY * 160 + xPixel] != 0) continue;

			display[LY * 160 + xPixel] = colorValue;
		}
	}
}