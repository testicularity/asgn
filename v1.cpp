#include "hi.h"
#include <iostream>
#include <vector>

using namespace std;

//global variables
Memory mem;
Regs reg;
Flags flag;



void operate(const string& s) { //check for number of operands
    vector<string> words = split(s , ' ');
    if(words == "IN") {
        in
    }
}

void runner() { //open files etc
    openfile();
    ifstream fs;
    fs.open("filename");
    string s;
    while(getline(fs , s)) {
        operate(s); //s = ADD R1, R6
    }
}




int main() { //to specify input/output file, can add options as well like verbose mode...
    
    string line; //Pc
    string s;
    stringstream ss;


    while(getline(cin , line , '\n')) {
        ss.str(line);
        ss >> s; //first word of the line
        if(!fc.contains(s)) {
            //operator not found (prints the error line and reg and table val b4 encountering the error)
            return 1;
        }



    }
    
}    
