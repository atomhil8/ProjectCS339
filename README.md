# CS3339 Course Project: MIPS Simulator
Written by: Luke Werner, Vincent Siv, Adam Hill, and Fidel Rocha Loza
### Input File Formatting:
- No comments
- Labels must be on a different line than instructions
- Registers use the usual `$t0`-eque name system
- Our simulator only supports ADD, ADDI, SUB, MUL, AND, OR, SLL, SRL, LW, SW, BEQ, J, and NOP instructions, as well as labels
- Our simulator does not account for hazards, so be sure to place NOPs in places where hazards may otherwise appear

### Build Instructions:
1. Make sure you have g++ set up and that it compiles C++ at version 11 or higher
2. Download all .cpp and .h files
3. Run the command `g++ main.cpp executor.cpp machine_state.cpp operations.cpp -o <fileName>`

This should give you an executable file with the file name you chose.

### Run Instructions:
1. Run the command `.\<fileName> <inputFileName> <debugMode>`

> Note that inputFileName is the full name of the assembly file that you wish to run and debugMode is set to true when the argument "true" is passed.

This should run the file and output the register file and the values stored in memory after the execution is complete.
