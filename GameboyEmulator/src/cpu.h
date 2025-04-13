#pragma once
#include <cstdint>
#include <vector>

class CPU { 
private:
	enum class CounterAction {
		Advance,
		Jump
	};

	struct Instruction {
		uint8_t opcode;
		uint8_t L;
		uint8_t H;
		uint16_t& pc;

		Instruction(uint16_t& pc) : pc(pc) {}
	};

	using OpcodeFunc = CPU::CounterAction (CPU::*)(Instruction);

	const uint8_t instructionsAmount = 0x100;
	// instrunctions vector linked to function, non static to handle unimplemented ones
	std::vector<OpcodeFunc> opcodeTable;
	// Static vector of bytes needed for instruction, they never change
	static const std::vector<uint8_t> instructionBytes;

	std::vector<CPU::OpcodeFunc> InitializeOpcodeTable();

	// Opcode load handlers
	CPU::CounterAction NOP(Instruction);
	CPU::CounterAction JP_A16(Instruction);

	// Opcode fallback (Can be removed after everything implemented)
	CPU::CounterAction UnimplementedOpcode(Instruction);

	uint8_t GetBytesByOpcode(uint8_t opcode);
public:
	CPU();
	void ExecuteOpcode(std::vector<uint8_t>& memory, uint16_t& pc);
};