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

    /*
    for(int i = 0; i < program.size(); i++) {
        std::cout << program[i] << std::endl;
    }
    */
    
    while(state.PC < program.size()) {
        //std::cout << state.PC << std::endl;
        
        IFStage(program, state);
        //std::cout << "ran if" << std::endl;
        IDStage(state);
        //std::cout << "ran id" << std::endl;
        EXStage(state);
        //std::cout << "ran ex" << std::endl;
        MEMStage(state);
        //std::cout << "ran mem" << std::endl;
        WBStage(state);
        //std::cout << "ran wb" << std::endl;

        if(debug) {
            std::cout << std::endl;
            std::cout << "Registers:" << std::endl;

            // Counter makes new line every four registers to make it look pretty
            int counter = 1;

            for(int i = 0; i < NUM_REGS; i++) {
                std::string reg;

                for(const auto& pair : state.regMap) {
                    if(pair.second == i) {
                        reg = pair.first;

                        break;
                    }
                }

                std::cout << std::setw(5) << "[" << reg << "]: " << state.REG[i];

                if(counter % 4 == 0) {
                    std::cout << std::endl;
                }

                counter++;
            }

            std::string IFinst = "empty";
            std::string IDinst = "empty";
            std::string EXinst = "empty";
            std::string MEMinst = "empty";
            std::string WBinst = "empty";

            if(state.PC > 3) {
                IFinst = program[state.PC];
                IDinst = program[state.PC - 1];
                EXinst = program[state.PC - 2];
                MEMinst = program[state.PC - 3];
                WBinst = program[state.PC - 4];
            }
            else if(state.PC > 2) {
                IFinst = program[state.PC];
                IDinst = program[state.PC - 1];
                EXinst = program[state.PC - 2];
                MEMinst = program[state.PC - 3];
            }
            else if(state.PC > 1) {
                IFinst = program[state.PC];
                IDinst = program[state.PC - 1];
                EXinst = program[state.PC - 2];
            }
            else if(state.PC > 0) {
                IFinst = program[state.PC];
                IDinst = program[state.PC - 1];
            }
            else if(state.PC == 0) {
                IFinst = program[state.PC];
            }

            std::cout << std::endl;
            std::cout << "State Registers:" << std::endl;
            std::cout << "[IF]: " << IFinst << std::endl;
            std::cout << "[ID]: " << IDinst << std::endl;
            std::cout << "[EX]: " << EXinst << std::endl;
            std::cout << "[MEM]: " << MEMinst << std::endl;
            std::cout << "[WB]: " << WBinst << std::endl;

            std::cout << std::endl;
            std::cout << "Control Signals:" << std::endl;
            std::cout << "RegDst: " << state.id_ex.RegDst << std::endl;
            std::cout << "RegWrite: " << state.id_ex.RegWrite << std::endl;
            std::cout << "ALUSrc: " << state.id_ex.ALUSrc << std::endl;
            std::cout << "MemWrite: " << state.id_ex.MemWrite << std::endl;
            std::cout << "MemRead: " << state.id_ex.MemRead << std::endl;
            std::cout << "MemtoReg: " << state.id_ex.MemtoReg << std::endl;
            std::cout << "PCSrc: " << state.id_ex.PCSrc << std::endl;
        }

        state.PC++;
    }

    std::cout << std::endl;
    std::cout << "Final Registers:" << std::endl;

    // Counter makes new line every four registers to make it look pretty
    int counter = 1;

    for(int i = 0; i < NUM_REGS; i++) {
        std::string reg;

        for(const auto& pair : state.regMap) {
            if(pair.second == i) {
                reg = pair.first;

                break;
            }
        }

        std::cout << std::setw(5) << "[" << reg << "]: " << state.REG[i];

        if(counter % 4 == 0) {
            std::cout << std::endl;
        }

        counter++;
    }

    std::cout << std::endl;
    std::cout << "Non-Zero Memory:" << std::endl;
    for(int i = 0; i < MEM_SIZE; i++) {
        if(state.MEM[i] != 0) {
            std::cout << "MEM[" << i << "] = " << state.MEM[i] << std::endl;
        }
    }

    return 0;
}
