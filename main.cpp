#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

/* =========================
   REGISTER FILE
========================= */
int REG[32] = {0};

/* $zero is always 0 */
void writeReg(int idx, int val) {
    if (idx == 0) return;
    REG[idx] = val;
}

/* Register mapping */
unordered_map<string, int> R = {
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

int getR(string s) {
    return R.count(s) ? R[s] : -1;
}

/* =========================
   MEMORY
========================= */
int MEM[1024] = {0};

/* =========================
   CONTROL SIGNALS
========================= */
struct Control {
    int RegWrite;
    int MemRead;
    int MemWrite;
    int Branch;
    string ALUOp;
};

void printControl(Control c) {
    cout << "Control Signals:\n";
    cout << "RegWrite: " << c.RegWrite << "\n";
    cout << "MemRead:  " << c.MemRead << "\n";
    cout << "MemWrite: " << c.MemWrite << "\n";
    cout << "Branch:   " << c.Branch << "\n";
    cout << "ALUOp:    " << c.ALUOp << "\n";
}

/* =========================
   UTIL: CLEAN STRING
========================= */
string clean(string s) {
    if (!s.empty() && s.back() == ',') s.pop_back();
    return s;
}

/* =========================
   MAIN SIMULATOR
========================= */
int main() {
    ifstream file("input.txt");

    if (!file.is_open()) {
        cout << "Cannot open file\n";
        return 1;
    }

    vector<string> program;
    string line;

    while (getline(file, line)) {
        if (!line.empty())
            program.push_back(line);
    }

    file.close();

    int PC = 0;
    bool debug = true;

    while (PC < program.size()) {

        string instr = program[PC];
        stringstream ss(instr);

        string op, a, b, c;
        ss >> op >> a >> b >> c;

        a = clean(a);
        b = clean(b);
        c = clean(c);

        Control ctrl = {0,0,0,0,"NONE"};

        cout << "\nExecuting: " << instr << "\n";

        /* =========================
           ADD
        ========================= */
        if (op == "add") {
            int rd = getR(a), rs = getR(b), rt = getR(c);

            writeReg(rd, REG[rs] + REG[rt]);

            ctrl.RegWrite = 1;
            ctrl.ALUOp = "ADD";
        }

        /* =========================
           SUB
        ========================= */
        else if (op == "sub") {
            int rd = getR(a), rs = getR(b), rt = getR(c);

            writeReg(rd, REG[rs] - REG[rt]);

            ctrl.RegWrite = 1;
            ctrl.ALUOp = "SUB";
        }

        /* =========================
           LW
        ========================= */
        else if (op == "lw") {
            int rt = getR(a);

            // format: lw $t0, 4($t1)
            int offset;
            string base;

            size_t p1 = b.find('(');
            size_t p2 = b.find(')');

            offset = stoi(b.substr(0, p1));
            base = b.substr(p1 + 1, p2 - p1 - 1);

            int rs = getR(base);

            writeReg(rt, MEM[REG[rs] + offset]);

            ctrl.MemRead = 1;
            ctrl.RegWrite = 1;
            ctrl.ALUOp = "ADD";
        }

        /* =========================
           SW
        ========================= */
        else if (op == "sw") {
            int rt = getR(a);

            int offset;
            string base;

            size_t p1 = b.find('(');
            size_t p2 = b.find(')');

            offset = stoi(b.substr(0, p1));
            base = b.substr(p1 + 1, p2 - p1 - 1);

            int rs = getR(base);

            MEM[REG[rs] + offset] = REG[rt];

            ctrl.MemWrite = 1;
            ctrl.ALUOp = "ADD";
        }

        /* =========================
           BEQ
        ========================= */
        else if (op == "beq") {
            int rs = getR(a), rt = getR(b);
            int offset = stoi(c);

            ctrl.Branch = 1;

            if (REG[rs] == REG[rt]) {
                PC += offset;
                continue;
            }
        }

        /* =========================
           JUMP
        ========================= */
        else if (op == "j") {
            int target = stoi(a);
            PC = target;
            continue;
        }

        /* =========================
           DEBUG OUTPUT
        ========================= */
        if (debug) {
            printControl(ctrl);

            cout << "Registers (first 8): ";
            for (int i = 0; i < 8; i++)
                cout << REG[i] << " ";
            cout << "\n---------------------\n";
        }

        PC++;
    }

    /* =========================
       FINAL STATE
    ========================= */
    cout << "\nFINAL REGISTERS:\n";
    for (int i = 0; i < 32; i++) {
        cout << "R" << i << ": " << REG[i] << "\n";
    }

    cout << "\nNON-ZERO MEMORY:\n";
    for (int i = 0; i < 1024; i++) {
        if (MEM[i] != 0)
            cout << "MEM[" << i << "] = " << MEM[i] << "\n";
    }

    return 0;
}
