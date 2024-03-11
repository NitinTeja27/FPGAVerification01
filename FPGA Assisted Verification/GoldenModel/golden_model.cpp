#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

vector<int> temp(16,0);
vector<vector<int> > registers(16, temp);

void init_regs() {
    for(int i=0;i<registers.size();i++){
        for(int j=0;j<registers[i].size();j++){
            registers[i][j] = 0;
        }
    }
}

char nibbleToHexChar(int nibble) {
    return (nibble < 10) ? '0' + nibble : 'a' + (nibble - 10);  // Use 'a' for values 10-15
}

string binaryToHexChar(const string& binary) {
    int decimalValue = bitset<4>(binary).to_ulong();  // Change to 4 bits for each chunk
    return string(1, nibbleToHexChar(decimalValue));
}

vector<string> binaryToHexVector(const vector<vector<int> >& binaryVector) {
    vector<string> hexVector;

    for (size_t i = 0; i < binaryVector.size(); ++i) {
        string hexRow = "";
        for (size_t j = 0; j < binaryVector[i].size(); j += 4) {
            string binaryChunk = "";
            for (size_t k = 0; k < 4 && (j + k) < binaryVector[i].size(); ++k) {
                binaryChunk += to_string(binaryVector[i][j + k]);
            }
            hexRow += binaryToHexChar(binaryChunk);
        }
        hexVector.push_back(hexRow);
    }


    return hexVector;
}

void clean() {
    for(int i=0;i<registers[0].size();i++){
        registers[0][i] = 0;
    }
}

void print_regs(){
    for (int i=0;i<registers.size();i++){
        for (int j=0;j<registers[i].size();j++){
            cout<<registers[i][j];
        }cout<<endl;
    }
    cout<<endl;
}

void print_instr(vector<int> &instr){
    for (auto x: instr){
        cout<<x<<' ';   
    }cout<<endl;
}

int bin2int(vector<int> bin){
    int result = 0;
    for (int bit : bin) {
        result = (result << 1) | bit;
    }
    return result;
}

// opcode: 0
void MOV(int dest, int r1, int r2){
    vector<int>& regDest = registers[dest];
    const vector<int> reg1 = registers[r1];
    const vector<int> reg2 = registers[r2];

    vector<int> ans = reg1;
    for (size_t i = 0; i < regDest.size(); i++) {
        regDest[i] = reg1[i];
    }
}

// opcode: 1
void ADD(int dest, int r1, int r2){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];
    const vector<int>& reg2 = registers[r2];

    int carry = 0;
    for (size_t i = regDest.size(); i > 0; i--) {
        int sum = reg1[i - 1] + reg2[i - 1] + carry;
        regDest[i - 1] = sum % 2;
        carry = sum / 2;
    }
}

// opcode: 2
void SUB(int dest, int r1, int r2){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];
    const vector<int>& reg2 = registers[r2];

    int borrow = 0;
    for (size_t i = regDest.size(); i > 0; i--) {
        int diff = reg1[i - 1] - reg2[i - 1] - borrow;    
        if (diff < 0) {
            regDest[i - 1] = diff + 2; 
            borrow = 1;
        } else {
            regDest[i - 1] = diff;
            borrow = 0;
        }
    }
}

// opcode: 3
void AND(int dest, int r1, int r2){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];
    const vector<int>& reg2 = registers[r2];

    for (size_t i = 0; i < regDest.size(); i++) {
        regDest[i] = reg1[i] & reg2[i];
    }
}

// opcode: 4
void OR(int dest, int r1, int r2){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];
    const vector<int>& reg2 = registers[r2];

    for (size_t i = 0; i < regDest.size(); i++) {
        regDest[i] = reg1[i] | reg2[i];
    }
}

// opcode: 5
void XOR(int dest, int r1, int r2){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];
    const vector<int>& reg2 = registers[r2];

    for (size_t i = 0; i < regDest.size(); i++) {
        regDest[i] = reg1[i] ^ reg2[i];
    }
}

// opcode: 6
void SLL(int dest, int r1, int r2){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];
    const vector<int>& reg2 = registers[r2];

    int shiftAmount = bin2int(reg2);
    for (size_t i = 0; i < regDest.size(); i++) {
        regDest[i] = (i + shiftAmount < reg1.size()) ? reg1[i + shiftAmount] : 0;
    }
}

// opcode: 7
void SRL(int dest, int r1, int r2){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];
    const vector<int>& reg2 = registers[r2];

    int shiftAmount = bin2int(reg2);
    for (size_t i = 0; i < regDest.size(); i++) {
        regDest[i] = (i >= shiftAmount && i - shiftAmount < reg1.size()) ? reg1[i - shiftAmount] : 0;
    }
}

// opcode: 8
void SRA(int dest, int r1, int r2){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];
    const vector<int>& reg2 = registers[r2];

    int shiftAmount = bin2int(reg2);
    for (size_t i = 0; i < regDest.size(); i++) {
        if (i >= shiftAmount && i - shiftAmount < reg1.size()) {
            regDest[i] = (reg1[i - shiftAmount] == 1) ? 1 : 0;
        } else {
            regDest[i] = (reg1.front() == 1) ? 1 : 0;
        }
    }
}   

// opcode: 9
void ADDI(int dest, int r1, vector<int> imm){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];

    for (size_t i = 0; i < regDest.size(); i++) {
        regDest[i] = (reg1[i] + imm[i]) % 2;
    }
}

// opcode: 10
void ANDI(int dest, int r1, vector<int> imm){
    vector<int>& regDest = registers[dest]; 
    const vector<int>& reg1 = registers[r1];    

    for (size_t i = 0; i < regDest.size(); i++) {
        regDest[i] = reg1[i] & imm[i];
    }
}

// opcode: 11
void ORI(int dest, int r1, vector<int> imm){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];

    for (size_t i = 0; i < regDest.size(); i++) {
        regDest[i] = reg1[i] | imm[i];
    }
}

// opcode: 12
void XORI(int dest, int r1, vector<int> imm){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];

    for (size_t i = 0; i < regDest.size(); i++) {
        regDest[i] = reg1[i] ^ imm[i];
    }
}

// opcode: 13
void SLLI(int dest, int r1, vector<int> imm){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];

    int shiftAmount = bin2int(imm);
    for (size_t i = 0; i < regDest.size(); i++) {
        regDest[i] = (i + shiftAmount < reg1.size()) ? reg1[i + shiftAmount] : 0;
    }
}

// opcode: 14
void SRLI(int dest, int r1, vector<int> imm){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];

    int shiftAmount = bin2int(imm);
    for (size_t i = 0; i < regDest.size(); i++) {
        regDest[i] = (i >= shiftAmount && i - shiftAmount < reg1.size()) ? reg1[i - shiftAmount] : 0;
    }
}

// opcode: 15
void SRAI(int dest, int r1, vector<int> imm){
    vector<int>& regDest = registers[dest];
    const vector<int>& reg1 = registers[r1];

    int shiftAmount = bin2int(imm);
    for (size_t i = 0; i < regDest.size(); i++) {
        if (i >= shiftAmount && i - shiftAmount < reg1.size()) {
            regDest[i] = (reg1[i - shiftAmount] == 1) ? 1 : 0;
        } else {
            regDest[i] = (reg1.front() == 1) ? 1 : 0;
        }
    }
}

void execute_instruction(vector<int> instr){
    vector<int> opcode(4,0);
    vector<int> dest_reg(4,0);
    vector<int> reg1(4,0);
    vector<int> reg2(4,0);
    vector<int> imm;

    opcode.assign(instr.begin(), instr.begin()+4);
    dest_reg.assign(instr.begin()+4, instr.begin()+8);
    reg1.assign(instr.begin()+8, instr.begin()+12);
    reg2.assign(instr.begin()+12, instr.begin()+16);
    for(int i=0;i<12;i++){
        imm.push_back(0);
    } for(auto x: reg2){
        imm.push_back(x);
    }

    int op = bin2int(opcode);
    int dest = bin2int(dest_reg);
    int r1 = bin2int(reg1);
    int r2 = bin2int(reg2);
    
    switch (op) {
        case 0:
            MOV(dest, r1, r2);
            clean();
            break;
        case 1:
            ADD(dest, r1, r2);
            clean();
            break;
        case 2:
            SUB(dest, r1, r2);
            clean();
            break;
        case 3:
            AND(dest, r1, r2);
            clean();
            break;
        case 4:
            OR(dest, r1, r2);
            clean();
            break;
        case 5:
            XOR(dest, r1, r2);
            clean();
            break;
        case 6:
            SLL(dest, r1, r2);
            clean();
            break;
        case 7:
            SRL(dest, r1, r2);
            clean();
            break;
        case 8:
            SRA(dest, r1, r2);
            clean();
            break;
        case 9:
            ADDI(dest, r1, imm);
            clean();
            break;
        case 10:
            ANDI(dest, r1, imm);
            clean();
            break;
        case 11:
            ORI(dest, r1, imm);
            clean();
            break;
        case 12:
            XORI(dest, r1, imm);
            clean();
            break;
        case 13:
            SLLI(dest, r1, imm);
            clean();
            break;
        case 14:
            SRLI(dest, r1, imm);
            clean();
            break;
        case 15:
            SRAI(dest, r1, imm);
            clean();
            break;
        default:
            cout<<"Invalid opcode"<<endl;
    }
}

void write_state(ofstream& outputFile){
    vector<string > hexReg = binaryToHexVector(registers);
    for (const auto& row : hexReg) {
        outputFile << row << endl;
    }
    outputFile << endl;
}

void execute_test(string path){
    ifstream inputFile(path);

    if (!inputFile.is_open()) {
        cerr << "Error opening the file!" << endl;
        return;
    }

    string line;
    ofstream outputFile("output.txt");
    write_state(outputFile);
    while (getline(inputFile, line)) {
        vector<int> instr;
        for (char c : line) {
            if (c == '0' || c == '1') {
                instr.push_back(c - '0');
            }
        }

        execute_instruction(instr);
        write_state(outputFile);
    }
}

void save_matrix(string path, vector<vector<int> > finalState){
    ofstream outputFile("output.txt");
    for (const auto& row : finalState) {
        for (int digit : row) {
            outputFile << digit;
        }
        outputFile << endl;
    }
}

signed int main() {
    init_regs();

    execute_test("input.txt");
    // save_matrix("output.txt", registers);
    
}