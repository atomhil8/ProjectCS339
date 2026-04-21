#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "instruction.h"
#include "machine_state.h"
#include <unordered_map>
#include <string>

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
int op_j(const unordered_map<string, int>& label_table, const string& label);

void enforce_zero_register(MachineState& state);

#endif