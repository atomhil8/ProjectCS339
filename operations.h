#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "machine_state.h"

#include <string>
#include <unordered_map>

int op_add(int a, int b);
int op_addi(int a, int imm);
int op_sub(int a, int b);
int op_mul(int a, int b);
int op_and(int a, int b);
int op_or(int a, int b);
int op_sll(int value, int shamt);
int op_srl(int value, int shamt);

int op_lw(int base, int offset);
int op_sw(int base, int offset);

bool op_beq(int a, int b);

#endif
