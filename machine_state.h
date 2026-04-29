#ifndef MACHINE_STATE_H
#define MACHINE_STATE_H

#include <string>
#include <unordered_map>

#define NUM_REGS 32
#define MEM_SIZE 1024

typedef struct MachineState {
    int REG[NUM_REGS];
    int MEM[MEM_SIZE];
    int PC; // Tracks which instruction we're on

    // Maps register names to their index, Ex: "$t0" -> 8
    std::unordered_map<std::string,int> regMap;
    
    // Maps label name to line number, Ex: "loop" -> 5
    std::unordered_map<std::string,int> labelMap;

    // State Registers
    struct IF_ID {
        int instruction;
        int pc_plus_4;
    };

    struct ID_EX {
        int rs_val, rt_val;
        int immediate;
        int rs, rt, rd;
        // Control signals
        bool RegWrite, MemRead, MemWrite, MemtoReg, ALUSrc;
    };

    struct EX_MEM {
        int alu_result;
        int write_data;
        int write_reg;
        bool RegWrite, MemRead, MemWrite, MemtoReg;
    };

    struct MEM_WB {
        int mem_data;
        int alu_result;
        int write_reg;
        bool RegWrite, MemtoReg;
    };

    IF_ID if_id, next_if_id;
    ID_EX id_ex, next_id_ex;
    EX_MEM ex_mem, next_ex_mem;
    MEM_WB mem_wb, next_mem_wb;

    //constructor (initializes PC and register mapping))
    MachineState();

    //stores value into the register array at index 
    void writeReg(int index, int value);
};

#endif
