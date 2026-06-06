#include "doctest/doctest.h"
#include "../tests/TestUtils.h"

#include <cpu/cpu.h>
#include <cpu/modules/cpu_jump.h>

TEST_CASE("JP_N16") {
    struct TestCase {
        string name;
        uint8_t L;
        uint8_t H;
        uint16_t newPc;
    };

    std::vector<TestCase> cases = {
        {"Jump case 1", 0x00, 0x00, 0x0000},
        {"Jump case 3", 0x01, 0x01, 0x0101},
        {"Jump case 4", 0xFF, 0xFF, 0xFFFF},
        {"Jump case 5", 0x00, 0x80, 0x8000},
        {"Jump case 6", 0x50, 0x01, 0x0150},
    };

    for (const auto& tc : cases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        TestInstruction ti(0xC3);
        ti.instruction.H = tc.H;
        ti.instruction.L = tc.L;
        ti.pc = 0x1000;

        CPUJump::JP_N16(cpu, ti.instruction);

        CHECK(ti.pc == tc.newPc);
    }
}

TEST_CASE("JR_N16") {
    struct TestCase {
        string name;
        uint8_t H;
        uint16_t newPc;
    };

    std::vector<TestCase> cases = {
        {"Jump case 1", 0x00, 0x1000},
        {"Jump case 2", 0x01, 0x1001},
        {"Jump case 3", 0x7F, 0x107F},
        {"Jump case 4", 0x80, 0x0F80},
        {"Jump case 5", 0xFF, 0x0FFF},
        {"Jump case 6", 0xFE, 0x0FFE},
    };

    for (const auto& tc : cases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        TestInstruction ti(0x18);
        ti.instruction.H = tc.H;
        ti.pc = 0x1000;

        CPUJump::JR_N8(cpu, ti.instruction);

        CHECK(ti.pc == tc.newPc);
    }
}

TEST_CASE("JR_NZ_N8") {
    struct TestCase {
        string name;
        uint8_t H;
        uint16_t newPc;
        bool flagZ;
    };

    std::vector<TestCase> cases = {
        {"Jump case 1", 0x00, 0x1000, true},
        {"Jump case 2", 0x01, 0x1001, false},
        {"Jump case 3", 0x7F, 0x1000, true},
        {"Jump case 4", 0x80, 0x0F80, false},
        {"Jump case 5", 0xFF, 0x1000, true},
        {"Jump case 6", 0xFE, 0x0FFE, false},
    };

    for (const auto& tc : cases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.FZ = tc.flagZ;

        TestInstruction ti(0x20);
        ti.instruction.H = tc.H;
        ti.pc = 0x1000;

        CPUJump::JR_NZ_N8(cpu, ti.instruction);

        CHECK(ti.pc == tc.newPc);
    }
}

TEST_CASE("JR_Z_N8") {
    struct TestCase {
        string name;
        uint8_t H;
        uint16_t newPc;
        bool flagZ;
    };

    std::vector<TestCase> cases = {
        {"Jump case 1", 0x00, 0x1000, true},
        {"Jump case 2", 0x01, 0x1000, false},
        {"Jump case 3", 0x7F, 0x107F, true},
        {"Jump case 4", 0x80, 0x1000, false},
        {"Jump case 5", 0xFF, 0x0FFF, true},
        {"Jump case 6", 0xFE, 0x1000, false},
    };

    for (const auto& tc : cases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.FZ = tc.flagZ;

        TestInstruction ti(0x28);
        ti.instruction.H = tc.H;
        ti.pc = 0x1000;

        CPUJump::JR_Z_N8(cpu, ti.instruction);

        CHECK(ti.pc == tc.newPc);
    }
}

TEST_CASE("JR_NC_N8") {
    struct TestCase {
        string name;
        uint8_t H;
        uint16_t newPc;
        bool flagC;
    };

    std::vector<TestCase> cases = {
        {"Jump case 1", 0x00, 0x1000, true},
        {"Jump case 2", 0x01, 0x1001, false},
        {"Jump case 3", 0x7F, 0x1000, true},
        {"Jump case 4", 0x80, 0x0F80, false},
        {"Jump case 5", 0xFF, 0x1000, true},
        {"Jump case 6", 0xFE, 0x0FFE, false},
    };

    for (const auto& tc : cases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.FC = tc.flagC;

        TestInstruction ti(0x30);
        ti.instruction.H = tc.H;
        ti.pc = 0x1000;

        CPUJump::JR_NC_N8(cpu, ti.instruction);

        CHECK(ti.pc == tc.newPc);
    }
}

TEST_CASE("JR_C_N8") {
    struct TestCase {
        string name;
        uint8_t H;
        uint16_t newPc;
        bool flagC;
    };

    std::vector<TestCase> cases = {
        {"Jump case 1", 0x00, 0x1000, true},
        {"Jump case 2", 0x01, 0x1000, false},
        {"Jump case 3", 0x7F, 0x107F, true},
        {"Jump case 4", 0x80, 0x1000, false},
        {"Jump case 5", 0xFF, 0x0FFF, true},
        {"Jump case 6", 0xFE, 0x1000, false},
    };

    for (const auto& tc : cases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.FC = tc.flagC;

        TestInstruction ti(0x38);
        ti.instruction.H = tc.H;
        ti.pc = 0x1000;

        CPUJump::JR_C_N8(cpu, ti.instruction);

        CHECK(ti.pc == tc.newPc);
    }
}

TEST_CASE("RST") {
    struct TestCase {
        string name;
        uint8_t opcode;
        uint16_t pc;
        uint16_t newPc;
    };

    std::vector<TestCase> cases = {
        {"RST 0", 0xC7, 0x1000, 0x0000},
        {"RST 1", 0xCF, 0x1100, 0x0008},
        {"RST 2", 0xD7, 0x1010, 0x0010},
        {"RST 3", 0xDF, 0x1001, 0x0018},
        {"RST 4", 0xE7, 0x1110, 0x0020},
        {"RST 5", 0xEF, 0x1011, 0x0028},
        {"RST 6", 0xF7, 0x0010, 0x0030},
        {"RST 7", 0xFF, 0x0100, 0x0038},
    };

    for (const auto& tc : cases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        TestInstruction ti(tc.opcode, tc.pc);

        CPUJump::RST(cpu, ti.instruction);

        CHECK(ti.pc == tc.newPc);
        CHECK(cpu.state.SP == 0xFFFC);

        uint8_t low = ti.memory[cpu.state.SP++];
        uint8_t high = ti.memory[cpu.state.SP++];
        uint16_t popped = (high << 8) | low;
        CHECK(popped == tc.pc + 1);
    }
}
