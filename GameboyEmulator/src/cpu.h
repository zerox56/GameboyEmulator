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
		std::vector<uint8_t>& memory;
		uint16_t& pc;

		Instruction(std::vector<uint8_t>& memory, uint16_t& pc) : memory(memory), pc(pc) {}
	};

	// Debug
	uint8_t debugCycleMax = 5;
	uint8_t debugCycleCurrent = 5;

	// Registers
	uint8_t A, B, C, D, E, H, L;
	uint16_t BC, DE, HL;
	// Registers loopup table (nullptr = HL)
	uint8_t* registerLookup[8] = { &B, &C, &D, &E, &H, &L, nullptr, &A };

	// Flags
	uint8_t FZ, FN, FH, FC;

	using OpcodeFunc = CPU::CounterAction (CPU::*)(Instruction);

	const uint8_t instructionsAmount = 0x100;
	// instrunctions vector linked to function, non static to handle unimplemented ones
	std::vector<OpcodeFunc> opcodeTable;
	// Static vector of bytes needed for instruction, they never change
	static const std::vector<uint8_t> instructionBytes;

	std::vector<CPU::OpcodeFunc> InitializeOpcodeTable();

	// Load instructions
	CPU::CounterAction LD_B_N8(Instruction);
	CPU::CounterAction LD_C_N8(Instruction);
	CPU::CounterAction LD_D_N8(Instruction);
	CPU::CounterAction LD_E_N8(Instruction);
	CPU::CounterAction LD_H_N8(Instruction);
	CPU::CounterAction LD_L_N8(Instruction);
	CPU::CounterAction LD_R_R(Instruction);

	// Jump instructions
	CPU::CounterAction JP_A16(Instruction);

	// 8bit instructions
	CPU::CounterAction ADD_A_R(Instruction);
	CPU::CounterAction ADC_A_R(Instruction);
	CPU::CounterAction SUB_A_R(Instruction);
	CPU::CounterAction SBC_A_R(Instruction);
	CPU::CounterAction AND_A_R(Instruction);
	CPU::CounterAction XOR_A_R(Instruction);
	CPU::CounterAction OR_A_R(Instruction);
	CPU::CounterAction CP_A_R(Instruction);

	// Other instructions
	CPU::CounterAction NOP(Instruction);

	// Opcode fallback (Can be removed after everything implemented)
	CPU::CounterAction UnimplementedOpcode(Instruction);

	uint8_t GetBytesByOpcode(uint8_t opcode);

	uint8_t GetFlags();
public:
	CPU();
	void ExecuteOpcode(std::vector<uint8_t>& memory, uint16_t& pc);
};