#include "cpu_misc.h"

namespace CPUMisc {
    CPU::CounterAction NOP(CPU&, CPU::Instruction) {
        return CPU::CounterAction::Advance;
    }
}