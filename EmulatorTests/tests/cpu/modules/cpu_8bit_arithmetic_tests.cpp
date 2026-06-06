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

            TestInstruction ti(0x80);
            CPU8BitArithmetic::ADD_A_R(cpu, ti.instruction);

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

            TestInstruction ti(0x81);
            CPU8BitArithmetic::ADD_A_R(cpu, ti.instruction);

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

            TestInstruction ti(0x82);
            CPU8BitArithmetic::ADD_A_R(cpu, ti.instruction);

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

            TestInstruction ti(0x83);
            CPU8BitArithmetic::ADD_A_R(cpu, ti.instruction);

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

            TestInstruction ti(0x84);
            CPU8BitArithmetic::ADD_A_R(cpu, ti.instruction);

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

            TestInstruction ti(0x85);
            CPU8BitArithmetic::ADD_A_R(cpu, ti.instruction);

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

            TestInstruction ti(0x86);
            ti.memory[0x1000] = tc.reg;

            CPU8BitArithmetic::ADD_A_R(cpu, ti.instruction);

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

            TestInstruction ti(0x87);
            CPU8BitArithmetic::ADD_A_R(cpu, ti.instruction);

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
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x88);
            CPU8BitArithmetic::ADC_A_R(cpu, ti.instruction);

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
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x89);
            CPU8BitArithmetic::ADC_A_R(cpu, ti.instruction);

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
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x9A);
            CPU8BitArithmetic::ADC_A_R(cpu, ti.instruction);

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
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x9B);
            CPU8BitArithmetic::ADC_A_R(cpu, ti.instruction);

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
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x9C);
            CPU8BitArithmetic::ADC_A_R(cpu, ti.instruction);

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
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x9D);
            CPU8BitArithmetic::ADC_A_R(cpu, ti.instruction);

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
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x9E);
            ti.memory[0x1000] = tc.reg;

            CPU8BitArithmetic::ADC_A_R(cpu, ti.instruction);

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
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x9F);
            CPU8BitArithmetic::ADC_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }
}

TEST_CASE("All SUB_A_R functions") {
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
        {"Sub case 1", 0x0A, 0x01, 0x09, false, true, false, false},
        {"Sub case 2", 0x10, 0x0F, 0x01, false, true, true, false},
        {"Both 0", 0x00, 0x00, 0x00, true, true, false, false},
        {"Carry", 0x02, 0xFF, 0x03, false, true, true, true},
        {"Half-Carry", 0x10, 0x01, 0x0F, false, true, true, false},
        {"Borrow to zero", 0x05, 0x05, 0x00, true, true, false, false},
        {"Borrow to non-zero", 0x05, 0x04, 0x01, false, true, false, false},
    };

    SUBCASE("SUB_A_B") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.B = tc.reg;

            TestInstruction ti(0x90);
            CPU8BitArithmetic::SUB_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SUB_A_C") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.C = tc.reg;

            TestInstruction ti(0x91);
            CPU8BitArithmetic::SUB_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SUB_A_D") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.D = tc.reg;

            TestInstruction ti(0x92);
            CPU8BitArithmetic::SUB_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SUB_A_E") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.E = tc.reg;

            TestInstruction ti(0x93);
            CPU8BitArithmetic::SUB_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SUB_A_H") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.H = tc.reg;

            TestInstruction ti(0x94);
            CPU8BitArithmetic::SUB_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SUB_A_L") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.L = tc.reg;

            TestInstruction ti(0x95);
            CPU8BitArithmetic::SUB_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SUB_A_HL") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.SetHL(0x1000);

            TestInstruction ti(0x96);
            ti.memory[0x1000] = tc.reg;

            CPU8BitArithmetic::SUB_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SUB_A_A") {
        std::vector<TestCase> localCases = {
            {"Sub case 1", 0x01, 0x01, 0x00, true, true, false, false},
            {"Sub case 2", 0x10, 0x10, 0x00, true, true, false, false},
        };

        for (const auto& tc : localCases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;

            TestInstruction ti(0x97);
            CPU8BitArithmetic::SUB_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }
}

TEST_CASE("All SBC_A_R functions") {
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
        {"Sub case 1", 0x0A, 0x01, 0x00, 0x09, false, true, false, false},
        {"Sub case 1", 0x0A, 0x01, 0x01, 0x08, false, true, false, false},
        {"Sub case 3", 0x11, 0x0F, 0x00, 0x02, false, true, true, false},
        {"Sub case 4", 0x11, 0x0F, 0x01, 0x01, false, true, true, false},
        {"Both 0", 0x00, 0x00, 0x00, 0x00, true, true, false, false},
        {"Both 0 with CY", 0x01, 0x00, 0x01, 0x00, true, true, false, false},
        {"Carry", 0x02, 0xFF, 0x00, 0x03, false, true, true, true},
        {"Carry with CY", 0x02, 0xFF, 0x01, 0x02, false, true, true, true},
        {"Half-Carry", 0x10, 0x01, 0x00, 0x0F, false, true, true, false},
        {"Half-Carry with CY", 0x10, 0x01, 0x01, 0x0E, false, true, true, false},
        {"Borrow to zero", 0x05, 0x05, 0x00, 0x00, true, true, false, false},
        {"Borrow to zero with CY", 0x06, 0x05, 0x01, 0x00, true, true, false, false},
        {"Borrow to non-zero", 0x05, 0x04, 0x00, 0x01, false, true, false, false},
        {"Borrow to non-zero with CY", 0x06, 0x04, 0x01, 0x01, false, true, false, false},
    };

    SUBCASE("SBC_A_B") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.B = tc.reg;
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x98);
            CPU8BitArithmetic::SBC_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SBC_A_C") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.C = tc.reg;
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x99);
            CPU8BitArithmetic::SBC_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SBC_A_D") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.D = tc.reg;
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x9A);
            CPU8BitArithmetic::SBC_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SBC_A_E") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.E = tc.reg;
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x9B);
            CPU8BitArithmetic::SBC_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SBC_A_H") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.H = tc.reg;
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x9C);
            CPU8BitArithmetic::SBC_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SBC_A_L") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.L = tc.reg;
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x9D);
            CPU8BitArithmetic::SBC_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SBC_A_HL") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.SetHL(0x1000);
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x9E);
            ti.memory[0x1000] = tc.reg;

            CPU8BitArithmetic::SBC_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("SBC_A_A") {
        std::vector<TestCase> localCases = {
            {"Sub case 1", 0x01, 0x01, 0x00, 0x00, true, true, false, false},
            {"Sub case 2", 0x01, 0x01, 0x01, 0xFF, false, true, true, true},
            {"Sub case 3", 0x10, 0x10, 0x00, 0x00, true, true, false, false},
            {"Sub case 4", 0x10, 0x10, 0x01, 0xFF, false, true, true, true},
        };

        for (const auto& tc : localCases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.FC = tc.CY;

            TestInstruction ti(0x97);
            CPU8BitArithmetic::SBC_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }
}

TEST_CASE("All AND_A_R functions") {
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
        {"And case 1", 0x01, 0x01, 0x01, false, false, true, false},
        {"And case 2", 0x10, 0x10, 0x10, false, false, true, false},
        {"And case 3", 0x01, 0x10, 0x00, true, false, true, false},
        {"And case 4", 0x10, 0x80, 0x00, true, false, true, false},
        {"Both 0", 0x00, 0x00, 0x00, true, false, true, false},
        {"Both 255", 0xFF, 0xFF, 0xFF, false, false, true, false},
    };

    SUBCASE("AND_A_B") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.B = tc.reg;

            TestInstruction ti(0xA0);
            CPU8BitArithmetic::AND_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("AND_A_C") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.C = tc.reg;

            TestInstruction ti(0xA1);
            CPU8BitArithmetic::AND_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("AND_A_D") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.D = tc.reg;

            TestInstruction ti(0xA2);
            CPU8BitArithmetic::AND_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("AND_A_E") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.E = tc.reg;

            TestInstruction ti(0xA3);
            CPU8BitArithmetic::AND_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("AND_A_H") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.H = tc.reg;

            TestInstruction ti(0xA4);
            CPU8BitArithmetic::AND_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("AND_A_L") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.L = tc.reg;

            TestInstruction ti(0xA5);
            CPU8BitArithmetic::AND_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("AND_A_HL") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.SetHL(0x1000);

            TestInstruction ti(0xA6);
            ti.memory[0x1000] = tc.reg;

            CPU8BitArithmetic::AND_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("AND_A_A") {
        std::vector<TestCase> localCases = {
            { "And case 1", 0x01, 0x01, 0x01, false, false, true, false },
            { "And case 2", 0x10, 0x10, 0x10, false, false, true, false },
            { "And case 3", 0x80, 0x80, 0x80, false, false, true, false },
            { "And case 4", 0x16, 0x16, 0x16, false, false, true, false },
            { "Both 0", 0x00, 0x00, 0x00, true, false, true, false },
            { "Both 255", 0xFF, 0xFF, 0xFF, false, false, true, false },
        };

        for (const auto& tc : localCases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;

            TestInstruction ti(0x87);
            CPU8BitArithmetic::AND_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }
}

TEST_CASE("All XOR_A_R functions") {
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
        {"Xor case 1", 0x01, 0x01, 0x00, true, false, false, false},
        {"Xor case 2", 0x10, 0x10, 0x00, true, false, false, false},
        {"Xor case 3", 0x01, 0x10, 0x11, false, false, false, false},
        {"Xor case 4", 0x17, 0x80, 0x97, false, false, false, false},
        {"Both 0", 0x00, 0x00, 0x00, true, false, false, false},
        {"Both 255", 0xFF, 0xFF, 0x00, true, false, false, false},
    };

    SUBCASE("XOR_A_B") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.B = tc.reg;

            TestInstruction ti(0xA8);
            CPU8BitArithmetic::XOR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("XOR_A_C") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.C = tc.reg;

            TestInstruction ti(0xA9);
            CPU8BitArithmetic::XOR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("XOR_A_D") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.D = tc.reg;

            TestInstruction ti(0xAA);
            CPU8BitArithmetic::XOR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("XOR_A_E") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.E = tc.reg;

            TestInstruction ti(0xAB);
            CPU8BitArithmetic::XOR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("XOR_A_H") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.H = tc.reg;

            TestInstruction ti(0xAC);
            CPU8BitArithmetic::XOR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("XOR_A_L") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.L = tc.reg;

            TestInstruction ti(0xAD);
            CPU8BitArithmetic::XOR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("XOR_A_HL") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.SetHL(0x1000);

            TestInstruction ti(0xAE);
            ti.memory[0x1000] = tc.reg;

            CPU8BitArithmetic::XOR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("XOR_A_A") {
        std::vector<TestCase> localCases = {
            { "Xor case 1", 0x01, 0x01, 0x00, true, false, false, false },
            { "Xor case 2", 0x10, 0x10, 0x00, true, false, false, false },
            { "Xor case 3", 0x80, 0x80, 0x00, true, false, false, false },
            { "Xor case 4", 0x16, 0x16, 0x00, true, false, false, false },
            { "Both 0", 0x00, 0x00, 0x00, true, false, false, false },
            { "Both 255", 0xFF, 0xFF, 0x00, true, false, false, false },
        };

        for (const auto& tc : localCases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;

            TestInstruction ti(0xAF);
            CPU8BitArithmetic::XOR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }
}

TEST_CASE("All OR_A_R functions") {
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
        {"Or case 1", 0x01, 0x01, 0x01, false, false, false, false},
        {"Or case 2", 0x10, 0x10, 0x10, false, false, false, false},
        {"Or case 3", 0x01, 0x10, 0x11, false, false, false, false},
        {"Or case 4", 0x17, 0x80, 0x97, false, false, false, false},
        {"Both 0", 0x00, 0x00, 0x00, true, false, false, false},
        {"Both 255", 0xFF, 0xFF, 0xFF, false, false, false, false},
    };

    SUBCASE("OR_A_B") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.B = tc.reg;

            TestInstruction ti(0xB0);
            CPU8BitArithmetic::OR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("OR_A_C") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.C = tc.reg;

            TestInstruction ti(0xB1);
            CPU8BitArithmetic::OR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("OR_A_D") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.D = tc.reg;

            TestInstruction ti(0xB2);
            CPU8BitArithmetic::OR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("OR_A_E") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.E = tc.reg;

            TestInstruction ti(0xB3);
            CPU8BitArithmetic::OR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("OR_A_H") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.H = tc.reg;

            TestInstruction ti(0xB4);
            CPU8BitArithmetic::OR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("OR_A_L") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.L = tc.reg;

            TestInstruction ti(0xB5);
            CPU8BitArithmetic::OR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("OR_A_HL") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.SetHL(0x1000);

            TestInstruction ti(0xB6);
            ti.memory[0x1000] = tc.reg;

            CPU8BitArithmetic::OR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("OR_A_A") {
        std::vector<TestCase> localCases = {
            {"Or case 1", 0x01, 0x01, 0x01, false, false, false, false},
            {"Or case 2", 0x10, 0x10, 0x10, false, false, false, false},
            {"Or case 3", 0x80, 0x80, 0x80, false, false, false, false},
            {"Or case 4", 0x16, 0x16, 0x16, false, false, false, false},
            {"Both 0", 0x00, 0x00, 0x00, true, false, false, false},
            {"Both 255", 0xFF, 0xFF, 0xFF, false, false, false, false},
        };

        for (const auto& tc : localCases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;

            TestInstruction ti(0xB7);
            CPU8BitArithmetic::OR_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }
}

TEST_CASE("All CP_A_R functions") {
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
        {"Cp case 1", 0x0A, 0x01, 0x0A, false, true, false, false},
        {"Cp case 2", 0x10, 0x0F, 0x10, false, true, true, false},
        {"Both 0", 0x00, 0x00, 0x00, true, true, false, false},
        {"Carry", 0x02, 0xFF, 0x02, false, true, true, true},
        {"Half-Carry", 0x10, 0x01, 0x10, false, true, true, false},
        {"Borrow to zero", 0x05, 0x05, 0x05, true, true, false, false},
        {"Borrow to non-zero", 0x05, 0x04, 0x05, false, true, false, false},
    };

    SUBCASE("CP_A_B") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.B = tc.reg;

            TestInstruction ti(0xB8);
            CPU8BitArithmetic::CP_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("CP_A_C") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.C = tc.reg;

            TestInstruction ti(0xB9);
            CPU8BitArithmetic::CP_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("CP_A_D") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.D = tc.reg;

            TestInstruction ti(0xBA);
            CPU8BitArithmetic::CP_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("CP_A_E") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.E = tc.reg;

            TestInstruction ti(0xBB);
            CPU8BitArithmetic::CP_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("CP_A_H") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.H = tc.reg;

            TestInstruction ti(0xBC);
            CPU8BitArithmetic::CP_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("CP_A_L") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.L = tc.reg;

            TestInstruction ti(0xBD);
            CPU8BitArithmetic::CP_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("CP_A_HL") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;
            cpu.state.SetHL(0x1000);

            TestInstruction ti(0xBE);
            ti.memory[0x1000] = tc.reg;

            CPU8BitArithmetic::CP_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("CP_A_A") {
        std::vector<TestCase> localCases = {
            {"Cp case 1", 0x01, 0x01, 0x01, true, true, false, false},
            {"Cp case 2", 0x10, 0x10, 0x10, true, true, false, false},
        };

        for (const auto& tc : localCases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.A;

            TestInstruction ti(0xBF);
            CPU8BitArithmetic::CP_A_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }
}

TEST_CASE("All INC_R functions") {
    struct TestCase {
        string name;
        uint8_t reg;
        uint8_t expectedResult;
        bool flagZ;
        bool flagN;
        bool flagH;
        bool flagC;
    };

    std::vector<TestCase> cases = {
        {"Inc case 1", 0x00, 0x01, false, false, false, false},
        {"Inc case 2", 0x10, 0x11, false, false, false, false},
        {"Half-Carry", 0x0F, 0x10, false, false, true, false},
        {"Overflow", 0xFF, 0x00, true, false, true, false},
    };

    SUBCASE("INC_B") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.B = tc.reg;

            TestInstruction ti(0x04);
            CPU8BitArithmetic::INC_R(cpu, ti.instruction);

            CHECK(cpu.state.B == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("INC_C") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.C = tc.reg;

            TestInstruction ti(0x0C);
            CPU8BitArithmetic::INC_R(cpu, ti.instruction);

            CHECK(cpu.state.C == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("INC_D") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.D = tc.reg;

            TestInstruction ti(0x14);
            CPU8BitArithmetic::INC_R(cpu, ti.instruction);

            CHECK(cpu.state.D == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("INC_E") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.E = tc.reg;

            TestInstruction ti(0x1C);
            CPU8BitArithmetic::INC_R(cpu, ti.instruction);

            CHECK(cpu.state.E == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("INC_H") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.H = tc.reg;

            TestInstruction ti(0x24);
            CPU8BitArithmetic::INC_R(cpu, ti.instruction);

            CHECK(cpu.state.H == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("INC_L") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.L = tc.reg;

            TestInstruction ti(0x2C);
            CPU8BitArithmetic::INC_R(cpu, ti.instruction);

            CHECK(cpu.state.L == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("INC_HL") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.SetHL(0x1000);

            TestInstruction ti(0x34);
            ti.memory[0x1000] = tc.reg;

            CPU8BitArithmetic::INC_R(cpu, ti.instruction);

            CHECK(ti.memory[0x1000] == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("INC_A") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.reg;

            TestInstruction ti(0x3C);
            CPU8BitArithmetic::INC_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }
}

TEST_CASE("All DEC_R functions") {
    struct TestCase {
        string name;
        uint8_t reg;
        uint8_t expectedResult;
        bool flagZ;
        bool flagN;
        bool flagH;
        bool flagC;
    };

    std::vector<TestCase> cases = {
        {"Dec case 1", 0x01, 0x00, true, true, false, false},
        {"Dec case 2", 0x11, 0x10, false, true, false, false},
        {"Half-Carry", 0x10, 0x0F, false, true, true, false},
        {"Borrow", 0x00, 0xFF, false, true, true, false},
    };

    SUBCASE("DEC_B") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.B = tc.reg;

            TestInstruction ti(0x05);
            CPU8BitArithmetic::DEC_R(cpu, ti.instruction);

            CHECK(cpu.state.B == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("DEC_C") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.C = tc.reg;

            TestInstruction ti(0x0D);
            CPU8BitArithmetic::DEC_R(cpu, ti.instruction);

            CHECK(cpu.state.C == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("DEC_D") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.D = tc.reg;

            TestInstruction ti(0x15);
            CPU8BitArithmetic::DEC_R(cpu, ti.instruction);

            CHECK(cpu.state.D == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("DEC_E") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.E = tc.reg;

            TestInstruction ti(0x1D);
            CPU8BitArithmetic::DEC_R(cpu, ti.instruction);

            CHECK(cpu.state.E == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("DEC_H") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.H = tc.reg;

            TestInstruction ti(0x25);
            CPU8BitArithmetic::DEC_R(cpu, ti.instruction);

            CHECK(cpu.state.H == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("DEC_L") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.L = tc.reg;

            TestInstruction ti(0x2D);
            CPU8BitArithmetic::DEC_R(cpu, ti.instruction);

            CHECK(cpu.state.L == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("DEC_HL") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.SetHL(0x1000);

            TestInstruction ti(0x35);
            ti.memory[0x1000] = tc.reg;

            CPU8BitArithmetic::DEC_R(cpu, ti.instruction);

            CHECK(ti.memory[0x1000] == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }

    SUBCASE("DEC_A") {
        for (const auto& tc : cases) {
            SUBCASE(tc.name.c_str());

            CPU cpu;
            cpu.state.A = tc.reg;

            TestInstruction ti(0x3D);
            CPU8BitArithmetic::DEC_R(cpu, ti.instruction);

            CHECK(cpu.state.A == tc.expectedResult);
            CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
        }
    }
}

TEST_CASE("CPL") {
    struct TestCase {
        string name;
        uint8_t A;
        uint8_t expectedResult;
        bool flagZ;
        bool flagN;
        bool flagH;
        bool flagC;
    };

    std::vector<TestCase> cases = {
        {"Cpl case 1", 0x01, 0xFE, false, true, true, false},
        {"Cpl case 2", 0x11, 0xEE, false, true, true, false},
        {"Cpl case 3", 0x80, 0x7F, false, true, true, false},
        {"Cpl case 4", 0xFF, 0x00, false, true, true, false},
    };

    for (const auto& tc : cases) {
        SUBCASE(tc.name.c_str());

        CPU cpu;
        cpu.state.A = tc.A;

        TestInstruction ti(0x2F);
        CPU8BitArithmetic::CPL(cpu, ti.instruction);

        CHECK(cpu.state.A == tc.expectedResult);
        CheckFlags(cpu, tc.flagZ, tc.flagN, tc.flagH, tc.flagC);
    }
}
