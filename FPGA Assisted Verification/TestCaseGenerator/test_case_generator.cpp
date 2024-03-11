#include<bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <random>

using namespace std;

vector<int> int2bin(int n) {
    vector<int> bin;
    for(int i=0;i<4;i++){
        bin.push_back(n%2);
        n = n/2;
    }
    reverse(bin.begin(), bin.end());    
    return bin;
}

vector<int> generate_instruction(vector<int> instructions, vector<int> registers){
    vector<int> instr;
    int op, dest, r1, r2;

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    
    uniform_int_distribution<size_t> dist(0, instructions.size() - 1);
    
    size_t randomIndex = dist(rng);
    op = instructions[randomIndex];

    randomIndex = dist(rng);
    dest = registers[static_cast<size_t>(randomIndex)];
        
    randomIndex = dist(rng);
    r1 = registers[static_cast<size_t>(randomIndex)];

    if(op<=8){
        randomIndex = dist(rng);
        r2 = registers[static_cast<size_t>(randomIndex)];
    } else {
        uniform_int_distribution<size_t> dist2(0, 15);
        size_t randomIndex2 = dist2(rng);
        r2 = static_cast<int>(randomIndex2);
    }
    
    vector<int> opcode = int2bin(op);
    vector<int> dest_reg = int2bin(dest);
    vector<int> r1_reg = int2bin(r1);
    vector<int> r2_reg = int2bin(r2);
    
    instr.insert(instr.end(), opcode.begin(), opcode.end());
    instr.insert(instr.end(), dest_reg.begin(), dest_reg.end());
    instr.insert(instr.end(), r1_reg.begin(), r1_reg.end());
    instr.insert(instr.end(), r2_reg.begin(), r2_reg.end());

    return instr;
}

void generateTestcase(string path, int num_instructions, vector<int> instructions, vector<int> registers){
    ofstream outputFile(path);
    
    for(int i=0;i<num_instructions;i++){
        vector<int> instr = generate_instruction(instructions, registers);
        for(int j=0;j<instr.size();j++){
            outputFile << instr[j];
        }
        outputFile << endl;
    }
}

void generate_test_case(string path){
    ifstream inputFile(path);

    int num_test_cases;
    int num_instructions;
    vector<int>  vec1;
    vector<int> vec2;

    vector<int> instructions;
    vector<int> registers;

    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        return;
    }

    inputFile >> num_test_cases;
    inputFile >> num_instructions;

    string binaryString;
    inputFile >> binaryString;
    for (char bit : binaryString) {
        if (bit == '0') {
            vec1.push_back(0);
        } else if (bit == '1') {
            vec1.push_back(1);
        } 
    } for(int i=0;i<vec1.size();i++){
        if(vec1[i]==1){
            instructions.push_back(i);
        }
    }

    inputFile >> binaryString;
    for (char bit : binaryString) {
        if (bit == '0') {
            vec2.push_back(0);
        } else if (bit == '1') {
            vec2.push_back(1);
        } 
    } for(int i=0;i<vec2.size();i++){
        if(vec2[i]==1){
            registers.push_back(i);
        }
    }

    for (int i=0;i<num_test_cases;i++){
        string path = "test_case_";
        path += to_string(i);
        path += ".txt";
        generateTestcase(path, num_instructions, instructions, registers);
    }

    
}

signed int main(){
    generate_test_case("input.txt");  
}