#pragma once
#include <cstdint>
#include <vector>

class PPU {
public:
	PPU();
	void Update(std::vector<uint8_t>& memory, uint8_t cycles);
	std::vector<uint8_t>& GetDisplay();
private:
	enum class PPUMode {
		HBlank = 0,
		VBlank = 1,
		OAMSearch = 2,
		LCDTransfer = 3,
	};
	uint8_t screenWidth = 160;
	uint8_t screenHeigth = 144;

	std::vector<uint8_t> display;

	uint8_t cycleCounter = 0;
	PPUMode currentMode = PPUMode::HBlank;

	// LCD
	uint16_t LCDCAddress = 0xFF40;
	uint16_t LYAddress = 0xFF44;
	uint16_t LYCAddress = 0xFF45;
	uint16_t tileMap0Start = 0x9800;
	uint16_t tileMap1Start = 0x9C00;
	uint16_t unsignedTilesStart = 0x8000;
	uint16_t signedTilesStart = 0x8800;
	uint8_t tileSize = 16;

	// Scanlines
	uint8_t scanLines = 153;

	// BG related
	// Scroll offset
	uint16_t SCYAddress = 0xFF42;
	uint16_t SCXAddress = 0xFF43;
	// Background palette
	uint16_t BGPAddress = 0xFF47;

	// Window related
	uint16_t WXAddress = 0xFF4B;
	uint16_t WYAddress = 0xFF4A;

	// Interrupt Flag
	uint16_t IFAddress = 0xFF0F;
	uint16_t STATAddress = 0xFF41;

	// Sprites
	uint16_t OAMStartAddress = 0xFE00;
	// Palettes
	uint16_t OBP0StartAddress = 0xFF48;
	uint16_t OBP1StartAddress = 0xFF49;
	struct Sprite {
		uint8_t y;
		uint8_t x;
		uint16_t tileId;
		uint8_t attributes;
	};
	uint8_t spriteSize = 4;

	void UpdateMode(std::vector<uint8_t>& memory);
	void UpdateSTAT(std::vector<uint8_t>& memory);

	uint8_t GetTileDataAddress(std::vector<uint8_t>& memory, uint8_t column, uint8_t row, uint8_t pixelInTile,
		uint16_t tileMapBaseAddress, bool isTileUnsigned);
	uint8_t GetColorValue(std::vector<uint8_t>& memory, uint8_t tileDataAddress, uint8_t xBit);
	uint8_t GetSpriteColorValue(std::vector<uint8_t>& memory, uint8_t tileDataAddress, uint8_t xBit, bool useOBP1);
	void FetchBackground(std::vector<uint8_t>& memory);
	void FetchWindow(std::vector<uint8_t>& memory);

	void DrawSprites(std::vector<uint8_t>& memory);
};