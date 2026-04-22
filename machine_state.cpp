/*
From my undertstanding, MachineState is a container that holds 
evreything the processor need to remember while running a program.
like the values of registers, memory, and the program counter.

without it, all the data is scattered across loose global variables
like the starter code has. That works for single file programs, but as this 
project grows, everyone needs a single organized object they can pass 
around to access the current state of the machine.

Instead of passing 5 separate things into every function, you just 
pass one MachineState object. That's why operations.h has functions like:

int op_lw(MachineState& state, int base, int offset);

Rather than passing memory and registers separately.
*/

#define OPERATIONS_H
#include <unordered_map>
#include <string>
using namespace std;


#define NUM_REGS 32
#define MEM_SIZE 1024

//we can switch this to a class later if we want
struct MachineState{


    int REG[NUM_REGS];
    int MEM[MEM_SIZE];
    int PC; //tracks which instruction we're on

    //turns regis name into their index ex: "$t0" -> 8
    unordered_map<string,int> regMap;
    
    //turns label name into line number ex: "loop" -> 5
    //I think we gonna need this for jump instructions
    unordered_map<string,int> labelMap;

    //constructor (initializes PC and register mapping))
    MachineState(){
        PC = 0;

        //setting everything to 0
        for (int i = 0; i < NUM_REGS; i++) 
            REG[i] = 0;
        for (int i = 0; i < MEM_SIZE;      i++) 
            MEM[i] = 0;

            
        regMap = {
            {"$zero",0},

            {"$v0",2},{"$v1",3},

            {"$a0",4},{"$a1",5},{"$a2",6},{"$a3",7},

            {"$t0",8},{"$t1",9},{"$t2",10},{"$t3",11},
            {"$t4",12},{"$t5",13},{"$t6",14},{"$t7",15},

            {"$s0",16},{"$s1",17},{"$s2",18},{"$s3",19},
            {"$s4",20},{"$s5",21},{"$s6",22},{"$s7",23},

            {"$t8",24},{"$t9",25},

            {"$k0",26},{"$k1",27},

            {"$gp",28},{"$sp",29},{"$fp",30},{"$ra",31}
        };
    }

    //stores value into the register array at index 
    void writeReg(int index, int value){
        if(index == 0) return; // $zero is always 0
        REG[index] = value;
    }

    //takes register name (ex "$t0") and returns its index (ex 8)
    int getRegIndex(const string& regName){
        if(regMap.count(regName) > 0)
            return regMap[regName];
        else
            return -1;
    }
};
