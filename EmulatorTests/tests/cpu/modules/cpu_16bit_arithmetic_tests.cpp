#include "doctest/doctest.h"
#include "../tests/TestUtils.h"

#include <cpu/cpu.h>
#include <cpu/modules/cpu_16bit_arithmetic.h>

struct IncTestCase {
    string name;
    uint16_t reg;
    uint16_t expectedResult;
};

std::vector<IncTestCase> incCases = {
    {"Inc case 1", 0x0000, 0x0001},
    {"Inc case 2", 0x1FDF, 0x1FE0},
    {"Overflow", 0xFFFF, 0x0000},
};

std::vector<IncTestCase> decCases = {
    {"Dec case 1", 0x0001, 0x0000},
    {"Dec case 2", 0x1FE0, 0x1FDF},
    {"Borrow", 0x0000, 0xFFFF},
};

TEST_CASE("INC_BC") {
    for (const auto& tc : incCases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.SetBC(tc.reg);

        TestInstruction ti(0x03);
        CPU16BitArithmetic::INC_BC(cpu, ti.instruction);

        CHECK(cpu.state.GetBC() == tc.expectedResult);
    }
}

TEST_CASE("INC_DE") {
    for (const auto& tc : incCases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.SetDE(tc.reg);

        TestInstruction ti(0x13);
        CPU16BitArithmetic::INC_DE(cpu, ti.instruction);

        CHECK(cpu.state.GetDE() == tc.expectedResult);
    }
}

TEST_CASE("INC_HL") {
    for (const auto& tc : incCases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.SetHL(tc.reg);

        TestInstruction ti(0x23);
        CPU16BitArithmetic::INC_HL(cpu, ti.instruction);

        CHECK(cpu.state.GetHL() == tc.expectedResult);
    }
}

TEST_CASE("INC_SP") {
    for (const auto& tc : incCases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.SP = tc.reg;

        TestInstruction ti(0x33);
        CPU16BitArithmetic::INC_SP(cpu, ti.instruction);

        CHECK(cpu.state.SP == tc.expectedResult);
    }
}

TEST_CASE("DEC_BC") {
    for (const auto& tc : decCases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.SetBC(tc.reg);

        TestInstruction ti(0x0B);
        CPU16BitArithmetic::DEC_BC(cpu, ti.instruction);

        CHECK(cpu.state.GetBC() == tc.expectedResult);
    }
}

TEST_CASE("DEC_DE") {
    for (const auto& tc : decCases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.SetDE(tc.reg);

        TestInstruction ti(0x1B);
        CPU16BitArithmetic::DEC_DE(cpu, ti.instruction);

        CHECK(cpu.state.GetDE() == tc.expectedResult);
    }
}

TEST_CASE("DEC_HL") {
    for (const auto& tc : decCases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.SetHL(tc.reg);

        TestInstruction ti(0x2B);
        CPU16BitArithmetic::DEC_HL(cpu, ti.instruction);

        CHECK(cpu.state.GetHL() == tc.expectedResult);
    }
}

TEST_CASE("DEC_SP") {
    for (const auto& tc : decCases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.SP = tc.reg;

        TestInstruction ti(0x3B);
        CPU16BitArithmetic::DEC_SP(cpu, ti.instruction);

        CHECK(cpu.state.SP == tc.expectedResult);
    }
}
