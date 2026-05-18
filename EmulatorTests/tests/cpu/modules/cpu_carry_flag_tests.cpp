#include "doctest/doctest.h"
#include "../tests/TestUtils.h"

#include <cpu/cpu.h>
#include <cpu/modules/cpu_carry_flag.h>

TEST_CASE("CCF") {
    struct TestCase {
        string name;
        uint8_t CY;
        bool flagZ;
        bool flagN;
        bool flagH;
        bool flagC;
    };

    std::vector<TestCase> cases = {
        {"CCF flip to true", false, false, false, false, true},
        {"CCF flip to false", true, false, false, false, false},
    };

    for (const auto& tc : cases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.FC = tc.CY;

        auto instruction = CreateInstruction(0x3F);
        CPUCarryFlag::CCF(cpu, instruction);

        CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
    }
}

TEST_CASE("SCF") {
    struct TestCase {
        string name;
        uint8_t CY;
        bool flagZ;
        bool flagN;
        bool flagH;
        bool flagC;
    };

    std::vector<TestCase> cases = {
        {"SCF false to true", false, false, false, false, true},
        {"SCF true to true", true, false, false, false, true},
    };

    for (const auto& tc : cases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.FC = tc.CY;

        auto instruction = CreateInstruction(0x37);
        CPUCarryFlag::SCF(cpu, instruction);

        CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
    }
}