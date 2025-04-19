#pragma once
#include "cpu/cpu.h"
#include "ppu/ppu.h"
#include "sdl/sdl_renderer.h"

#include <cstdint>
#include <vector>

class Emulator {
public:
	static Emulator& GetInstance() {
		static Emulator instance;
		return instance;
	}

	Emulator();
	bool LoadRom(const char* filePath);
	bool ValidateHeader();
	void Cycle();
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

	uint8_t cycles = 1;

	static CPU& cpu;
	static PPU& ppu;
	static SDLRenderer& sdlRenderer;
};