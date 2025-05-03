
#include "doctest/doctest.h"
#include "../tests/TestUtils.h"

#include <cpu/cpu.h>
#include <cpu/modules/cpu_8bit_arithmetic.h>

TEST_CASE("All ADD_A_R functions") {
    struct TestCase {
        string name;
        uint8_t A;
        uint8_t reg;
        uint8_t expectedResult;
        bool flagZ;
        bool flagH;
        bool flagC;
    };

    std::vector<TestCase> cases = {
        {"Add case 1", 0x01, 0x01, 0x02, false, false, false},
        {"Add case 2", 0x10, 0x0F, 0x1F, false, false, false},
        {"Both 0", 0x00, 0x00, 0x00, true, false, false},
        {"Carry", 0xF0, 0x20, 0x10, false, false, true},
        {"Half-Carry", 0x0F, 0x01, 0x10, false, true, false},
        {"Double Carry", 0xFF, 0x01, 0x00, true, true, true},
        {"Overflow to zero", 0xFF, 0x01, 0x00, true, true, true},
        {"Overflow to non-zero", 0xFF, 0x02, 0x01, false, true, true},
    };

    SUBCASE("ADD_A_B") {
        for (const auto& tc : cases) {
            INFO("Test case: " << tc.name);

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.B = tc.reg;

            auto instruction = CreateInstruction(0x80);
            CPU8BitArithmetic::ADD_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CHECK(cpu.state.FZ == tc.flagZ);
            CHECK(cpu.state.FN == false);
            CHECK(cpu.state.FH == tc.flagH);
            CHECK(cpu.state.FC == tc.flagC);
        }
    }
}