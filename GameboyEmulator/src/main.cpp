#include "emulator.h"

#include <stdio.h>
#include <memory>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("No ROM file provided.\n");
        return 1;
    }

    Emulator emulator;
    emulator.LoadRom(argv[1]);

    for(uint8_t i = 0; i < 32; i++) {
        emulator.Cycle();
    }

    system("pause");

    return 0;
}