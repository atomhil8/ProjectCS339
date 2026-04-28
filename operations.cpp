#include "operations.h"

#include <stdexcept>

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
    return static_cast<unsigned int>(value) >> shamt;
}

int op_lw(MachineState& state, int base, int offset) {
    int address = base + offset;

    if (state.memory.find(address) == state.memory.end()) {
        return 0;
    }

    return state.memory[address];
}

void op_sw(MachineState& state, int base, int offset, int value) {
    int address = base + offset;
    state.memory[address] = value;
}

bool op_beq(int a, int b) {
    return a == b;
}

int op_j(const std::unordered_map<std::string, int>& label_table,
         const std::string& label) {
    std::unordered_map<std::string, int>::const_iterator it =
        label_table.find(label);

    if (it == label_table.end()) {
        throw std::runtime_error("Jump label not found: " + label);
    }

    return it->second;
}

void enforce_zero_register(MachineState& state) {
    state.registers[0] = 0;
}
