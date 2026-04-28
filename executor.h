#ifndef EXECUTOR_H
#define EXECUTOR_H

//#include "instruction.h"
#include "machine_state.cpp"
#include "operations.h"
#include <string>
#include <vector>

// Rids string of comma
std::string clean(std::string s);

// Breaks down entire program into computer-friendly vector, also documents labels
std::vector<std::string> parse(const std::string& fileName, MachineState& state);

// Executes one instruction line
void execute(const std::string& instruction, MachineState& state, const bool& debug);

#endif