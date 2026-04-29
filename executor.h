#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "machine_state.h"
#include "operations.h"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <unordered_map>

// Rids string of comma
std::string clean(std::string s);

// Breaks down entire program into computer-friendly vector, also documents labels
std::vector<std::string> parse(const std::string& fileName, ::MachineState& state);

void IFStage(const std::vector<std::string> program, ::MachineState& state);

// Executes one instruction line
void execute(const std::string& instruction, ::MachineState& state, const bool& debug);

#endif
