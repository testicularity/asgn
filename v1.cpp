
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "h1.h"

using namespace std;

//global variables
Memory mem;
Regs reg;
Flags flag;
int pc = 0;



void operate(const string& s) { //check for number of operands
    vector<string> words = split(s);
    if(s == "") return;
    pc++;
    if(words[0] == "in") {
        if(words.size() == 2)
            in(words);
    }
    else if(words[0] == "out") {
        if(words.size() == 2)
            out(words);
    }
    else if(words[0] == "mov") {
        if(words.size() == 3){
            mov(words);
            
        }
    }
    else if(words[0] == "add") {
        arth( '+' , words);
    }
    else if(words[0] == "sub") {
        arth( '-' , words);
    }
    else if(words[0] == "mul") {
        arth( '*' , words);
    }
    else if(words[0] == "div") {
        arth( '/' , words);
    }
    else if(words[0] == "store") {
        store(words);
    }
    else if(words[0] == "load") {
        load(words);
    }
    else {//operation not found


    }
}

void runner() { //open files etc
    ifstream ifs {"/home/a/test/cppfiles/x/input.txt"};
    string s;
    while(getline(ifs , s)) {
        operate(s); //s = ADD R1, R6
    }
    ofstream ofs {"/home/a/test/cppfiles/x/output.txt"};
    
    reg.print(ofs);
    flag.print(ofs);
    ofs << setw(10) << left << "PC" << ": " << pc << "\n\n";
    mem.print(ofs);
    ofs.close();
}




int main() { //to specify input/output file, can add options as well like verbose mode...
    
    runner();

    return 0;
    
}    
