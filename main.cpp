#include "executor.h"
#include "machine_state.h"
#include "operations.h"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    // Command-line argument error check
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <fileName> <debugMode>" << std::endl;
        return 1;
    }

    const std::string fileName = argv[1];
    const std::string dArg = argv[2];
    bool debug = false;

    if(dArg == "1" || dArg == "true" || dArg == "True") {
        debug = true;
    }

    MachineState state;

    std::vector<std::string> program = parse(fileName, state);
    
    while (state.PC < program.size()) {
        IFStage(program, state);
    }

    /*
    std::cout << "\nFINAL REGISTERS:\n";
    for (int i = 0; i < 32; i++) {
        std::cout << "R" << i << ": " << REG[i] << "\n";
    }

    std::cout << "\nNON-ZERO MEMORY:\n";
    for (int i = 0; i < 1024; i++) {
        if (MEM[i] != 0)
            cout << "MEM[" << i << "] = " << MEM[i] << "\n";
    }
    */

    return 0;
}
