#include "h1.h"
#include <string>
#include <vector>

using namespace std;

//global variables
extern Memory mem;
extern Regs reg;
extern Flags fl;

//Memory member functions:
int Memory::getVal(const int& addr) {
    if (addr < MAX_SIZE && addr >= 0) return data[addr] ;
}

void Memory::writeVal(const int& val , const int& addr) {
    if(addr >= 0 && addr < MAX_SIZE) data[addr] = val;
} 

void Memory::clear(const int& addr) {
    if(addr >= 0 && addr < MAX_SIZE) data[addr] = 0;
}
void Memory::print() {
    
}
//Regs member functions:
int Regs::getVal(const string& reg) {
    string s = reg.substr(1);
    int i = stoi(s);
    if(i >= 0 && i < MAX_SIZE) return data[i];
}
int Regs::writeVal(const int& val , const string& reg) {
    string s = reg.substr(1);
    int i = stoi(s);
    if(i >= 0 && i < MAX_SIZE) data[i] = val;
}
void Regs::clear(const string& reg) {
    string s = reg.substr(1);
    int i = stoi(s);
    if(i >= 0 && i < MAX_SIZE) data[i] = 0;
}
void Regs::print() {

}
//Flags member functions:
void Flags::set(const int& flag , const bool& val) {
    data[flag] = val;
} 

bool Flags::get(const int& flag) {
    return data[flag];
}

void Flags::print() {

}

//helper functions:
vector<string> split(const string& s , const char& delim) {
    vector<string> words;
    string str = "";
    for(int i = 0; i < s.size(); i++) {
        if(s[i] != delim) str += s[i]; 
        else {
            words.push_back(str);
            str = "";
        }
    }
    words.push_back(str);
    return words;
    
}

int readFrom(const string& s) { //get val of loc
    char c = s[0];
    switch (c) {
        case '[': //mem [R0]
            string loc;
            for(auto c : s) {
                if(c == ']') break;
                loc += c;
            }
            if(c != ']') { //missing closing bracket
                
            }
            int i = reg.getVal(loc); //get address stored in reg s
            return mem.getVal(i);
            break;

        case 'R': //register
            return reg.getVal(s);
            break;
    }

    return stoi(s); //literal
}

void writeTo (const string& s , const int& i) { //write i to location in s
    char c = s[0];
    string loc;
    switch (c) { //[R0] 0-63
        case '[': //mem
            for(auto c : s) {
                if(c == ']') break;
                loc += c;
            }
            if(c != ']') { //missing closing bracket

            }
            int addr = reg.getVal(loc); //write to address stored in reg s
            mem.writeVal(i , addr);
            break;

        case 'R': //register
            reg.writeVal(i , s);
            break;

        default: //literal means addr of mem

            break;    
    }   
}

/*int toAByte(int const& r) { //gets the value after OF/UF/CF
    int finalRes = 0;
    if(r > 127 || r < -128) { //set overflow flag
        int i = (r&127);
        if(r&128) { //8th bit is 1
            finalRes = i - 128;
        }
    }

    else finalRes = i;
    return finalRes;
}*/


//operation functions:
void in(vector<string>& words) { //IN R1

    int inp;
    cin >> inp;
    writeTo(words[1] , inp);

}

void out(vector<string>& words) {
    int i = readFrom(words[1]);
    cout << i << '\n';
}

void mov(vector<string>& words) {
    int i = readFrom(words[1]);
    writeTo(words[2] , i);
}

void arth(const char& c , vector<string>& words) {
    int firstVal = readFrom(words[1]);
    int secVal = readFrom(words[2]);
    int r = 0;

    switch(c) {
        case '+':
            r = firstVal + secVal;
            break;

        case '-':
            r = secVal - firstVal;
            break;

        case '*':
            r = firstVal * secVal;
            break;

        case '/':
            r = secVal / firstVal;
            break;
    }

    if(r > 255) flag.set(flag.CF , true);
    else if(r == 0) flag.set(flag.ZF , true); 
    int finalRes = r&255;
    writeTo(words[2] , finalRes);

}

void rotate(const char& c , vector<string>& words) {
    int val = readFrom(words[0]); // the value in register
    int n = readFrom(words[1]) % 8; // rotate how many times

    switch(c) {
        case 'l': 
            int tmp = val >> (8 - n);
            int val = (val << n) + tmp;
            break;

        case 'r':
            int tmp = val >> (n);
            int val = (val << (8-n)) + tmp;
            break;
    }

    int finalRes = val & 255;
    if(finalRes == 0) flag.set(flag.ZF , true); 

    writeTo(words[1] , finalRes);
}
void shift(const char& c , vector<string>& words) { 
    int val = readFrom(words[0]); // the value in register
    int n = readFrom(words[1]); // shift how many times

    switch(c) {
        case 'l':
            val << n;
            break;

        case 'r':
            val >> n;
            break;
    }

    int finalRes = val & 255;
    if(finalRes == 0) flag.set(flag.ZF , true);

    writeTo(words[1] , finalRes);
    
}
void load(vector<string>& words) {
    if(words[2][0] == '[') { //2nd operand is [mem]
        int val = readFrom(words[2]);
        writeTo(words[1] , val);
    }

    else { //2nd operand is addr
        int val = mem.getVal(stoi(words[2]));
        writeTo(words[1] , val);
    }
}
void store(vector<string>& words) {
    int val = readFrom(words[1]);
    if(words[2][0] == '[') { //2nd operand is [mem]
        writeTo(words[2] , val);
    }

    else { //2nd operand is addr
        mem.writeVal(val , stoi(words[2]));
    }
}
