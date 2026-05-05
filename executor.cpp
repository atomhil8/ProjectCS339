// Luke Werner A05265888 onn16
// 5/4/2026
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

void IFStage(const std::vector<std::string>& program, MachineState& state) {
    state.if_id = state.next_if_id;

    state.next_if_id.instruction = program[state.PC];
    state.next_if_id.newPC = state.PC + 1;
}

void IDStage(MachineState& state) {
    state.id_ex = state.next_id_ex;

    // Fetch
    std::stringstream ss(state.if_id.instruction);

    std::string op, a, b, c;
    ss >> op >> a >> b >> c;

    a = clean(a);
    b = clean(b);
    c = clean(c);

    // Clear old values
    state.next_id_ex = {};
    int rsVal = 0;
    int rtVal = 0;
    int imm = 0;
    int rs = -1;
    int rt = -1;
    int rd = -1;

    // Decode instructions
    if(op == "add" || op == "Add" || op == "ADD" ||
        op == "sub" || op == "Sub" || op == "SUB" ||
        op == "and" || op == "And" || op == "AND" ||
        op == "mul" || op == "Mul" || op == "MUL" ||
        op == "or" || op == "Or" || op == "OR") {

        rd = state.regMap.at(a);
        rs = state.regMap.at(b);
        rt = state.regMap.at(c);

        rsVal = state.REG[rs];
        rtVal = state.REG[rt];

        state.next_id_ex.RegDst = true;
        state.next_id_ex.RegWrite = true;
        state.next_id_ex.ALUSrc = false;
        state.next_id_ex.MemWrite = false;
        state.next_id_ex.MemRead = false;
        state.next_id_ex.MemtoReg = false;
        state.next_id_ex.PCSrc = false;
    }
    else if(op == "addi" || op == "Addi" || op == "ADDI") {

        rt = state.regMap.at(a);
        rs = state.regMap.at(b);
        imm = std::stoi(c);

        rsVal = state.REG[rs];

        state.next_id_ex.RegDst = false;
        state.next_id_ex.RegWrite = true;
        state.next_id_ex.ALUSrc = true;
        state.next_id_ex.MemWrite = false;
        state.next_id_ex.MemRead = false;
        state.next_id_ex.MemtoReg = false;
        state.next_id_ex.PCSrc = false;
    }
    else if(op == "sll" || op == "Sll" || op == "SLL") {

        rd = state.regMap.at(a);
        rt = state.regMap.at(b);
        // shamt
        imm = std::stoi(c);

        rtVal = state.REG[rt];

        state.next_id_ex.RegDst = true;
        state.next_id_ex.RegWrite = true;
        state.next_id_ex.ALUSrc = false;
        state.next_id_ex.MemWrite = false;
        state.next_id_ex.MemRead = false;
        state.next_id_ex.MemtoReg = false;
        state.next_id_ex.PCSrc = false;
    }
    else if(op == "srl" || op == "Srl" || op == "SRL") {

        rd = state.regMap.at(a);
        rt = state.regMap.at(b);
        // shamt
        imm = std::stoi(c);

        rtVal = state.REG[rt];

        state.next_id_ex.RegDst = true;
        state.next_id_ex.RegWrite = true;
        state.next_id_ex.ALUSrc = true;
        state.next_id_ex.MemWrite = false;
        state.next_id_ex.MemRead = false;
        state.next_id_ex.MemtoReg = false;
        state.next_id_ex.PCSrc = false;
    }
    else if(op == "lw" || op == "Lw" || op == "LW") {

        rt = state.regMap.at(a);

        // Format: offset(base)
        std::size_t p1 = b.find('(');
        std::size_t p2 = b.find(')');

        imm = std::stoi(b.substr(0, p1));
        rs = state.regMap.at(b.substr(p1 + 1, p2 - p1 - 1));

        rsVal = state.REG[rs];

        state.next_id_ex.RegDst = true;
        state.next_id_ex.RegWrite = true;
        state.next_id_ex.ALUSrc = true;
        state.next_id_ex.MemWrite = false;
        state.next_id_ex.MemRead = true;
        state.next_id_ex.MemtoReg = true;
        state.next_id_ex.PCSrc = false;
    }
    else if(op == "sw" || op == "Sw" || op == "SW") {

        rt = state.regMap.at(a);

        std::size_t p1 = b.find('(');
        std::size_t p2 = b.find(')');

        imm = std::stoi(b.substr(0, p1));
        rs = state.regMap.at(b.substr(p1 + 1, p2 - p1 - 1));

        rsVal = state.REG[rs];
        rtVal = state.REG[rt];

        state.next_id_ex.RegDst = true;
        state.next_id_ex.RegWrite = false;
        state.next_id_ex.ALUSrc = true;
        state.next_id_ex.MemWrite = true;
        state.next_id_ex.MemRead = false;
        state.next_id_ex.MemtoReg = false;
        state.next_id_ex.PCSrc = false;
    }
    else if(op == "beq" || op == "Beq" || op == "BEQ") {

        rs = state.regMap.at(a);
        rt = state.regMap.at(b);
        imm = state.labelMap.at(c);

        rsVal = state.REG[rs];
        rtVal = state.REG[rt];

        state.next_id_ex.RegDst = false;
        state.next_id_ex.RegWrite = false;
        state.next_id_ex.ALUSrc = false;
        state.next_id_ex.MemWrite = false;
        state.next_id_ex.MemRead = false;
        state.next_id_ex.MemtoReg = false;
        state.next_id_ex.PCSrc = true;
    }
    else if(op == "j" || op == "J") {

        imm = state.labelMap.at(a);

        state.next_id_ex.RegDst = false;
        state.next_id_ex.RegWrite = false;
        state.next_id_ex.ALUSrc = false;
        state.next_id_ex.MemWrite = false;
        state.next_id_ex.MemRead = false;
        state.next_id_ex.MemtoReg = false;
        state.next_id_ex.PCSrc = false;

        // state.id_ex.Jump = 1;
    }
    else if(op == "nop" || op == "Nop" || op == "NOP") {

        state.next_id_ex.RegDst = false;
        state.next_id_ex.RegWrite = false;
        state.next_id_ex.ALUSrc = false;
        state.next_id_ex.MemWrite = false;
        state.next_id_ex.MemRead = false;
        state.next_id_ex.MemtoReg = false;
        state.next_id_ex.PCSrc = false;
    }
    else {
        std::cerr << "Undocumented operator used. Passing NOP.";

        op = "NOP";

        state.next_id_ex.RegDst = false;
        state.next_id_ex.RegWrite = false;
        state.next_id_ex.ALUSrc = false;
        state.next_id_ex.MemWrite = false;
        state.next_id_ex.MemRead = false;
        state.next_id_ex.MemtoReg = false;
        state.next_id_ex.PCSrc = false;
    }

    state.next_id_ex.rsVal = rsVal;
    state.next_id_ex.rtVal = rtVal;
    state.next_id_ex.imm = imm;
    state.next_id_ex.rs = rs;
    state.next_id_ex.rt = rt;
    state.next_id_ex.rd = rd;
    state.next_id_ex.ALUOp = op;
}

void EXStage(MachineState& state) {
    state.ex_mem = state.next_ex_mem;

    int rsVal = state.id_ex.rsVal;
    int rtVal = state.id_ex.rtVal;
    int imm = state.id_ex.imm;
    int rs = state.id_ex.rs;
    int rt = state.id_ex.rt;
    int rd = state.id_ex.rd;

    std::string op = state.id_ex.ALUOp;

    if(op == "add" || op == "Add" || op == "ADD") {

        int sum = op_add(rsVal, rtVal);

        state.next_ex_mem.ALUResult = sum;
        state.next_ex_mem.writeReg = rd;
    }
    else if(op == "addi" || op == "Addi" || op == "ADDI") {

        int sum = op_addi(rsVal, imm);

        state.next_ex_mem.ALUResult = sum;
        state.next_ex_mem.writeReg = rt;
    }
    else if(op == "sub" || op == "Sub" || op == "SUB") {

        int diff = op_sub(rsVal, rtVal);

        state.next_ex_mem.ALUResult = diff;
        state.next_ex_mem.writeReg = rd;
    }
    else if(op == "mul" || op == "Mul" || op == "MUL") {

        int prod = op_mul(rsVal, rtVal);

        state.next_ex_mem.ALUResult = prod;
        state.next_ex_mem.writeReg = rd;
    }
    else if(op == "and" || op == "And" || op == "AND") {

        int result = op_and(rsVal, rtVal);

        state.next_ex_mem.ALUResult = result;
        state.next_ex_mem.writeReg = rd;
    }
    else if(op == "or" || op == "Or" || op == "OR") {

        int result = op_or(rsVal, rtVal);

        state.next_ex_mem.ALUResult = result;
        state.next_ex_mem.writeReg = rd;
    }
    else if(op == "sll" || op == "Sll" || op == "SLL") {

        int result = op_sll(rtVal, imm);

        state.next_ex_mem.ALUResult = result;
        state.next_ex_mem.writeReg = rd;
    }
    else if(op == "srl" || op == "Srl" || op == "SRL") {

        int result = op_srl(rtVal, imm);

        state.next_ex_mem.ALUResult = result;
        state.next_ex_mem.writeReg = rd;
    }
    else if(op == "lw" || op == "Lw" || op == "LW") {

        int address = op_lw(rs, imm);

        state.next_ex_mem.ALUResult = address;
        state.next_ex_mem.writeReg = rt;
    }
    else if(op == "sw" || op == "Sw" || op == "SW") {

        int address = op_sw(rs, imm);

        state.next_ex_mem.ALUResult = address;
        state.next_ex_mem.writeData = rtVal;
    }
    else if(op == "beq" || op == "Beq" || op == "BEQ") {
        
        bool result = op_beq(rsVal, rtVal);

        // Branch is taken, flush pipeline
        if(result) {
            state.next_if_id = {"NOP"};

            state.next_id_ex = {0, 0, 0, -1, -1, -1,
                false, false, false, false, false, false, false, "NOP"};

            state.PC = imm;
        }
    }
    else if(op == "j" || op == "J") {

        // Flush pipeline for jump
        state.next_if_id = {"NOP"};

        state.next_id_ex = {0, 0, 0, -1, -1, -1,
            false, false, false, false, false, false, false, "NOP"};

        state.PC = imm;
    }
    else if(op == "nop" || op == "Nop" || op == "NOP") {
        // Do nothing
    }

    state.next_ex_mem.RegWrite = state.id_ex.RegWrite;
    state.next_ex_mem.MemWrite = state.id_ex.MemWrite;
    state.next_ex_mem.MemRead = state.id_ex.MemRead;
    state.next_ex_mem.MemtoReg = state.id_ex.MemtoReg;
}

void MEMStage(MachineState& state) {
    state.mem_wb = state.next_mem_wb;

    if(state.ex_mem.MemWrite) {
        state.MEM[state.ex_mem.ALUResult] = state.ex_mem.writeData;
    }

    if(state.ex_mem.MemRead) {
        state.next_mem_wb.memData = state.MEM[state.ex_mem.ALUResult];
    }

    state.next_mem_wb.ALUResult = state.ex_mem.ALUResult;
    state.next_mem_wb.writeReg = state.ex_mem.writeReg;

    state.next_mem_wb.RegWrite = state.ex_mem.RegWrite;
    state.next_mem_wb.MemtoReg = state.ex_mem.MemtoReg;
}

void WBStage(MachineState& state) {
    if(state.mem_wb.RegWrite) {
        if(state.mem_wb.writeReg != 0) {
            state.REG[state.mem_wb.writeReg] = state.mem_wb.ALUResult;
        }
    }

    if(state.mem_wb.MemtoReg) {
        if(state.mem_wb.writeReg != 0) {
            state.REG[state.mem_wb.writeReg] = state.mem_wb.memData;
        }
    }
}
