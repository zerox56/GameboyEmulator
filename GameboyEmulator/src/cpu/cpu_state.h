#pragma once
#include <cstdint>
#include <vector>

struct CPUState {
	// 8bit registers
	uint8_t A, B, C, D, E, H, L;
	// 8bit registers loopup table (nullptr = HL)
	std::vector<uint8_t*> registerLookup = { &B, &C, &D, &E, &H, &L, nullptr, &A };

	// Stack pointer
	uint16_t SP = 0xFFFE;

	// Pending variable to turn on IME after next instruction
	bool pendingIME = false; 
	// Halts Executions
	bool halted = false; 
	// Interrupt Master Enable
	bool IME; 

	// Flags
	uint8_t FZ, FN, FH, FC;
	// Interrupt Enable
	uint8_t IEAddress = 0xFFFF; 
	// Interrupt Flag
	uint8_t IFAddress = 0xFF0F; 

	// Flags functions
	uint8_t GetFlags() const {
		return (FZ << 7) | (FN << 6) | (FH << 5) | (FC << 4);
	}

	void SetFlags(uint8_t F) {
		FZ = (F >> 7) & 1;
		FN = (F >> 6) & 1;
		FH = (F >> 5) & 1;
		FC = (F >> 4) & 1;
	}

	// 16bit registers pair functions
	uint16_t GetBC() const {
		return (B << 8) | C;
	}

	void SetBC(uint16_t BC) {
		B = BC >> 8; C = BC & 0xFF;
	}

	uint16_t GetDE() const {
		return (D << 8) | E;
	}

	void SetDE(uint16_t DE) {
		D = DE >> 8; E = DE & 0xFF;
	}

	uint16_t GetHL() const {
		return (H << 8) | L;
	}

	void SetHL(uint16_t HL) {
		H = HL >> 8; L = HL & 0xFF;
	}

	uint16_t GetAF() const {
		return (A << 8) | GetFlags();
	}

	void SetAF(uint16_t AF) {
		A = AF >> 8;
		SetFlags(AF & 0xF0);
	}

	uint8_t GetInterruptFlags(std::vector<uint8_t>& memory) const {
		return memory[IEAddress] & memory[IFAddress];
	}

	void SetIF(uint8_t value, std::vector<uint8_t>& memory) {
		memory[IFAddress] &= ~value;
	}
};