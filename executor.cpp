// Luke Werner A05265888 onn16
// 4/27/2026
// executor.cpp
// Parses input and executes instructions

#include "executor.h"

std::string clean(std::string s) {
    if (!s.empty() && s.back() == ',') s.pop_back();
    return s;
}

std::vector<std::string> parse(const std::string& fileName, MachineState& state) {
    std::ifstream file(fileName);

    if(!file.is_open()) {
        std::cout << "Cannot open file." << std::endl;
        return {"UNABLE TO OPEN FILE"};
    }

    std::vector<std::string> program;
    std::string line;
    
    // Temporary instruction counter for label documentation
    int IC = 0;

    while(getline(file, line)) {
        if(!line.empty()){
            // Found label (via colon)
            if(line.find(':') != std::string::npos){
                std::string label = line.substr(0, line.find(':'));

                state.labelMap.insert(std::make_pair(label, IC + 1));

                // [WIP] Attempts to extract instruction following label
                // Since this doesn't work, program assumes labels are on separate lines for now

                // line = line.substr(line.find(':') + 1, line.size() - line.find(':'));
            }
            // Otherwise it's an instruction
            else {
                program.push_back(line);
                IC ++;
            }
        }
    }

    file.close();

    return program;
}

void IFStage(const std::vector<std::string> program, MachineState& state) {

}

void execute(const std::string& instruction, MachineState& state, const bool& debug) {
    std::stringstream ss(instruction);

    std::string op, a, b, c;
    ss >> op >> a >> b >> c;

    a = clean(a);
    b = clean(b);
    c = clean(c);

    std::cout << "Executing: " << instruction << std::endl;

    if(op == "ADD" || op == "Add" || op == "add") {
        // Assuming ctrl uses RegDst, RegWrite, ALUSrc, MemWrite, MemRead, MemtoReg, PCSrc
        state.ctrl = {1, 1, 0, 0, 0, 0, 0};

        int writeID = state.regMap.at(a);
        int arg1 = state.REG[state.regMap.at(b)];
        int arg2 = state.REG[state.regMap.at(c)];

        int sum = op_add(arg1, arg2);

        state.writeReg(writeID, sum);
    }
    else if(op == "ADDI" || op == "Addi" || op == "addi") {
        state.ctrl = {0, 1, 1, 0, 0, 0, 0};

        int writeID = state.regMap.at(a);
        int arg1 = state.REG[state.regMap.at(b)];
        int arg2 = stoi(c);

        int sum = op_addi(arg1, arg2);

        state.writeReg(writeID, sum);
    }
    else if(op == "SUB" || op == "Sub" || op == "sub") {
        state.ctrl = {1, 1, 0, 0, 0, 0, 0};

        int writeID = state.regMap.at(a);
        int arg1 = state.REG[state.regMap.at(b)];
        int arg2 = state.REG[state.regMap.at(c)];

        int diff = op_sub(arg1, arg2);

        state.writeReg(writeID, diff);
    }
    else if(op == "MUL" || op == "Mul" || op == "mul") {
        state.ctrl = {1, 1, 0, 0, 0, 0, 0};

        int writeID = state.regMap.at(a);
        int arg1 = state.REG[state.regMap.at(b)];
        int arg2 = state.REG[state.regMap.at(c)];

        int prod = op_mul(arg1, arg2);

        state.writeReg(writeID, prod);
    }

    else if(op == "AND" || op == "And" || op == "and") {
        state.ctrl = {1, 1, 0, 0, 0, 0, 0};

        int writeID = state.regMap.at(a);
        int arg1 = state.REG[state.regMap.at(b)];
        int arg2 = state.REG[state.regMap.at(c)];

        int result = op_and(arg1, arg2);

        state.writeReg(writeID, result);
    }
    else if(op == "OR" || op == "Or" || op == "or") {
        state.ctrl = {1, 1, 0, 0, 0, 0, 0};

        int writeID = state.regMap.at(a);
        int arg1 = state.REG[state.regMap.at(b)];
        int arg2 = state.REG[state.regMap.at(c)];

        int result = op_or(arg1, arg2);

        state.writeReg(writeID, result);
    }
    else if(op == "SLL" || op == "Sll" || op == "sll") {
        state.ctrl = {1, 1, 0, 0, 0, 0, 0};

        int writeID = state.regMap.at(a);
        int arg1 = state.REG[state.regMap.at(b)];
        int arg2 = stoi(c);

        int result = op_sll(arg1, arg2);

        state.writeReg(writeID, result);
    }
    else if(op == "SRL" || op == "Srl" || op == "srl") {
        state.ctrl = {1, 1, 0, 0, 0, 0, 0};

        int writeID = state.regMap.at(a);
        int arg1 = state.REG[state.regMap.at(b)];
        int arg2 = stoi(c);

        int result = op_srl(arg1, arg2);

        state.writeReg(writeID, result);
    }

    else if(op == "LW" || op == "Lw" || op == "lw") {
        state.ctrl = {0, 1, 1, 0, 1, 1, 0};

        int writeID = state.regMap.at(a);
        
        std::size_t p1 = b.find('(');
        std::size_t p2 = b.find(')');

        int offset = stoi(b.substr(0, p1));
        // op_lw takes the base argument in terms of the address
        int base = state.regMap.at(b.substr(p1 + 1, p2 - p1 - 1));

        int result = op_lw(state, base, offset);

        state.writeReg(writeID, result);
    }
    else if(op == "SW" || op == "Sw" || op == "sw") {
        state.ctrl = {0, 0, 1, 1, 0, 0, 0};

        int storeVal = state.REG[state.regMap.at(a)];
        
        std::size_t p1 = b.find('(');
        std::size_t p2 = b.find(')');

        int offset = stoi(b.substr(0, p1));
        // op_sw takes the base argument in terms of the address
        int base = state.regMap.at(b.substr(p1 + 1, p2 - p1 - 1));

        op_sw(state, base, offset, storeVal);
    }

    ///// ALU for branch instructions does not align with machine state info, fix operations.cpp /////
    else if(op == "BEQ" || op == "Beq" || op == "beq") {
        state.ctrl = {0, 0, 0, 0, 0, 0, 1};
    }
    else if(op == "J" || op == "j") {
        state.ctrl = {0, 0, 0, 0, 0, 0, 0};
    }

    else if(op == "NOP" || op == "Nop" || op == "nop") {
        state.ctrl = {0, 0, 0, 0, 0, 0, 0};
    }

    else {
        std::cerr << "Undocumented operator used. Skipping instruction.";
    }

    /*
    else if (op == "beq") {
        int rs = getR(a), rt = getR(b);
        int offset = stoi(c);

        ctrl.Branch = 1;

        if (REG[rs] == REG[rt]) {
            PC += offset;
            continue;
        }
    }
    else if (op == "j") {
        int target = stoi(a);
        state.PC = target;
        continue;
    }
    */

    if(debug) {
        std::cout << "Registers:" << std::endl;

        // Counter makes new line every four registers to make it look pretty
        int counter = 1;

        for(int i = 0; i < 32; i++) {
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
        std::cout << "Control Signals:" << std::endl;
        std::cout << "RegDst: " << state.ctrl[0] << std::endl;
        std::cout << "RegWrite: " << state.ctrl[1] << std::endl;
        std::cout << "ALUSrc: " << state.ctrl[2] << std::endl;
        std::cout << "MemWrite: " << state.ctrl[3] << std::endl;
        std::cout << "MemRead: " << state.ctrl[4] << std::endl;
        std::cout << "MemtoReg: " << state.ctrl[5] << std::endl;
        std::cout << "PCSrc: " << state.ctrl[6] << std::endl;
    }
}
