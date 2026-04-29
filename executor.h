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
std::vector<std::string> parse(const std::string& fileName, MachineState& state);

// Fetch stage of pipeline
void IFStage(const std::vector<std::string>& program, MachineState& state);

// Decode stage of pipeline
void IDStage(MachineState& state);

// Execute stage of pipeline
void EXStage(MachineState& state);

// Memory read/write stage of pipeline
void MEMStage(MachineState& state);

// Register write back stage of pipeline
void WBStage(MachineState& state);

#endif
