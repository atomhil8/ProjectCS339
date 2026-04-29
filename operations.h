#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "machine_state.h"

#include <unordered_map>
#include <string>
#include <stdexcept>

int op_add(int a, int b);
int op_addi(int a, int imm);
int op_sub(int a, int b);
int op_mul(int a, int b);
int op_and(int a, int b);
int op_or(int a, int b);
int op_sll(int value, int shamt);
int op_srl(int value, int shamt);

int op_lw(MachineState& state, int base, int offset);
void op_sw(MachineState& state, int base, int offset, int value);

bool op_beq(int a, int b);
int op_j(const std::unordered_map<std::string, int>& label_table, const std::string& label);

void enforce_zero_register(MachineState& state);

#endif
