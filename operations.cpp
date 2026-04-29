#include "operations.h"

int op_add(int a, int b) {
    return a + b;
}

int op_addi(int a, int imm) {
    return a + imm;
}

int op_sub(int a, int b) {
    return a - b;
}

int op_mul(int a, int b) {
    return a * b;
}

int op_and(int a, int b) {
    return a & b;
}

int op_or(int a, int b) {
    return a | b;
}

int op_sll(int value, int shamt) {
    return value << shamt;
}

int op_srl(int value, int shamt) {
    return value >> shamt;
}

int op_lw(int base, int offset) {
    return base + offset;
}

int op_sw(int base, int offset) {
    return base + offset;
}

bool op_beq(int a, int b) {
    return a == b;
}
