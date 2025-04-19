#pragma once
#include "cpu_state.h"

#include <cstdint>
#include <vector>
#include <map>

class CPU {
public:
	static CPU& GetInstance() {
		static CPU instance;
		return instance;
	}

	enum class CounterAction {
		Advance,
		AdvanceSkipIME,
		Jump,
		Wait,
		BitShift
	};

	struct Instruction {
		uint8_t opcode;
		uint8_t L;
		uint8_t H;
		std::vector<uint8_t>& memory;
		uint16_t& pc;

		Instruction(std::vector<uint8_t>& memory, uint16_t& pc) : memory(memory), pc(pc) {}
	};

	CPUState state;

	CPU();
	void ExecuteOpcode(std::vector<uint8_t>& memory, uint16_t& pc);
	void UpdateTimers(std::vector<uint8_t>& memory, uint8_t cycles);
private:
	// Debug
	uint8_t debugCycleMax = 5;
	uint8_t debugCycleCurrent = 5;

	using OpcodeFunc = CPU::CounterAction (*)(CPU&, Instruction);
	std::vector<CPU::OpcodeFunc> InitializeOpcodeTable();

	const uint8_t instructionsAmount = 0x100;
	// instrunctions vector linked to function, non static to handle unimplemented ones
	std::vector<OpcodeFunc> opcodeTable;
	// Static vector of bytes needed for instruction, they never change
	static const std::vector<uint8_t> instructionBytes;

	uint8_t GetBytesByOpcode(uint8_t opcode);

	static const std::map<uint8_t, uint8_t> vectorJumps;
	void InterruptCPU(std::vector<uint8_t>& memory);
};