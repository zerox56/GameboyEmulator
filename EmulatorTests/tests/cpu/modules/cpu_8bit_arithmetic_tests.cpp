
#include "doctest/doctest.h"
#include "../tests/TestUtils.h"

#include <cpu/cpu.h>
#include <cpu/modules/cpu_8bit_arithmetic.h>

void CheckFlags(const CPU& cpu, bool Z, bool N, bool H, bool C) {
    CHECK(cpu.state.FZ == Z);
    CHECK(cpu.state.FN == N);
    CHECK(cpu.state.FH == H);
    CHECK(cpu.state.FC == C);
}

TEST_CASE("All ADD_A_R functions") {
    struct TestCase {
        string name;
        uint8_t A;
        uint8_t reg;
        uint8_t expectedResult;
        bool flagZ;
        bool flagN;
        bool flagH;
        bool flagC;
    };

    std::vector<TestCase> cases = {
        {"Add case 1", 0x01, 0x01, 0x02, false, false, false, false},
        {"Add case 2", 0x10, 0x0F, 0x1F, false, false, false, false},
        {"Both 0", 0x00, 0x00, 0x00, true, false, false, false},
        {"Carry", 0xFF, 0x01, 0x00, true, false, true, true},
        {"Half-Carry", 0x0F, 0x01, 0x10, false, false, true, false},
        {"Overflow to zero", 0xFF, 0x01, 0x00, true, false, true, true},
        {"Overflow to non-zero", 0xFF, 0x02, 0x01, false, false, true, true},
    };

    SUBCASE("ADD_A_B") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.B = tc.reg;

            auto instruction = CreateInstruction(0x80);
            CPU8BitArithmetic::ADD_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADD_A_C") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.C = tc.reg;

            auto instruction = CreateInstruction(0x81);
            CPU8BitArithmetic::ADD_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADD_A_D") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.D = tc.reg;

            auto instruction = CreateInstruction(0x82);
            CPU8BitArithmetic::ADD_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADD_A_E") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.E = tc.reg;

            auto instruction = CreateInstruction(0x83);
            CPU8BitArithmetic::ADD_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADD_A_H") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.H = tc.reg;

            auto instruction = CreateInstruction(0x84);
            CPU8BitArithmetic::ADD_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADD_A_L") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.L = tc.reg;

            auto instruction = CreateInstruction(0x85);
            CPU8BitArithmetic::ADD_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADD_A_HL") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.SetHL(0x1000);

            std::vector<uint8_t> dummyMemory(0x10000, 0);
            dummyMemory[0x1000] = tc.reg;

            auto instruction = CreateInstruction(0x86, dummyMemory);
            CPU8BitArithmetic::ADD_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADD_A_A") {
        std::vector<TestCase> localCases = {
            {"Add case 1", 0x01, 0x01, 0x02, false, false, false, false},
            {"Add case 2", 0x10, 0x10, 0x20, false, false, false, false},
            {"Both 0", 0x00, 0x00, 0x00, true, false, false, false},
            {"Carry", 0x80, 0x80, 0x00, true, false, false, true},
            {"Half-Carry", 0x08, 0x08, 0x10, false, false, true, false},
            {"Overflow to zero", 0x80, 0x80, 0x00, true, false, false, true},
            {"Overflow to non-zero", 0x81, 0x81, 0x02, false, false, false, true},
        };

        for (const auto& tc : localCases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;

            auto instruction = CreateInstruction(0x87);
            CPU8BitArithmetic::ADD_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }
}

TEST_CASE("All ADC_A_R functions") {
    struct TestCase {
        string name;
        uint8_t A;
        uint8_t reg;
        uint8_t CY;
        uint8_t expectedResult;
        bool flagZ;
        bool flagN;
        bool flagH;
        bool flagC;
    };

    std::vector<TestCase> cases = {
        {"Add case 1", 0x01, 0x01, 0x00, 0x02, false, false, false, false},
        {"Add case 2", 0x01, 0x01, 0x01, 0x03, false, false, false, false},
        {"Add case 3", 0x10, 0x0F, 0x00, 0x1F, false, false, false, false},
        {"Add case 4", 0x10, 0x0E, 0x01, 0x1F, false, false, false, false},
        {"Both 0", 0x00, 0x00, 0x00, 0x00, true, false, false, false},
        {"Both 0 with CY", 0x00, 0x00, 0x01, 0x01, false, false, false, false},
        {"Carry", 0xFF, 0x01, 0x00, 0x00, true, false, true, true},
        {"Carry with CY", 0xFF, 0x01, 0x01, 0x01, false, false, true, true},
        {"Half-Carry", 0x0F, 0x01, 0x00, 0x10, false, false, true, false},
        {"Half-Carry with CY", 0x0F, 0x01, 0x01, 0x11, false, false, true, false},
        {"Overflow to zero", 0xFF, 0x01, 0x00, 0x00, true, false, true, true},
        {"Overflow to zero with CY", 0xFE, 0x01, 0x01, 0x00, true, false, true, true},
        {"Overflow to non-zero", 0xFF, 0x02, 0x00, 0x01, false, false, true, true},
        {"Overflow to non-zero with CY", 0xFF, 0x02, 0x01, 0x02, false, false, true, true},
    };

    SUBCASE("ADC_A_B") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.B = tc.reg;
            cpu.state.FlagC = tc.CY;

            auto instruction = CreateInstruction(0x88);
            CPU8BitArithmetic::ADC_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADC_A_C") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.C = tc.reg;
            cpu.state.FlagC = tc.CY;

            auto instruction = CreateInstruction(0x89);
            CPU8BitArithmetic::ADC_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADC_A_D") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.D = tc.reg;
            cpu.state.FlagC = tc.CY;

            auto instruction = CreateInstruction(0x9A);
            CPU8BitArithmetic::ADC_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADC_A_E") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.E = tc.reg;
            cpu.state.FlagC = tc.CY;

            auto instruction = CreateInstruction(0x9B);
            CPU8BitArithmetic::ADC_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADC_A_H") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.H = tc.reg;
            cpu.state.FlagC = tc.CY;

            auto instruction = CreateInstruction(0x9C);
            CPU8BitArithmetic::ADC_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADC_A_L") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.L = tc.reg;
            cpu.state.FlagC = tc.CY;

            auto instruction = CreateInstruction(0x9D);
            CPU8BitArithmetic::ADC_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADD_A_HL") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.SetHL(0x1000);
            cpu.state.FlagC = tc.CY;

            std::vector<uint8_t> dummyMemory(0x10000, 0);
            dummyMemory[0x1000] = tc.reg;

            auto instruction = CreateInstruction(0x9E, dummyMemory);
            CPU8BitArithmetic::ADC_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("ADC_A_A") {
        std::vector<TestCase> localCases = {
            {"Add case 1", 0x01, 0x01, 0x00, 0x02, false, false, false, false},
            {"Add case 2", 0x01, 0x01, 0x01, 0x03, false, false, false, false},
            {"Add case 3", 0x10, 0x10, 0x00, 0x20, false, false, false, false},
            {"Add case 4", 0x10, 0x10, 0x01, 0x21, false, false, false, false},
            {"Both 0", 0x00, 0x00, 0x00, 0x00, true, false, false, false},
            {"Both 0 with CY", 0x00, 0x00, 0x01, 0x01, false, false, false, false},
            {"Carry", 0x85, 0x85, 0x00, 0x0A, false, false, false, true},
            {"Carry with CY", 0x85, 0x85, 0x01, 0x0B, false, false, false, true},
            {"Half-Carry", 0x08, 0x08, 0x00, 0x10, false, false, true, false},
            {"Half-Carry with CY", 0x08, 0x08, 0x01, 0x11, false, false, true, false},
            {"Overflow to zero", 0x80, 0x80, 0x00, 0x00, true, false, false, true},
            {"Overflow to non-zero", 0x81, 0x81, 0x00, 0x02, false, false, false, true},
            {"Overflow to non-zero with CY", 0x81, 0x81, 0x01, 0x03, false, false, false, true},
        };

        for (const auto& tc : localCases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.FlagC = tc.CY;

            auto instruction = CreateInstruction(0x9F);
            CPU8BitArithmetic::ADC_A_R(cpu, instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }
}