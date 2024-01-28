#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "h1.h"

using namespace std;

//global variables
extern Memory mem;
extern Regs reg;
extern Flags flag;

//Memory member functions:
int Memory::getVal(const int& addr) {
    if (addr < Memory::MAX_SIZE && addr >= 0) return int(data[addr]);
    else {
        cout << "Error getting memory value\n";
        return -1;
    }
}

void Memory::writeVal(const int& val , const int& addr) {
    if(addr >= 0 && addr < Memory::MAX_SIZE) data[addr] = val;
    else 
        cout << "Memory writing error\n";
    
} 

void Memory::clear(const int& addr) {
    if(addr >= 0 && addr < Memory::MAX_SIZE) data[addr] = 0;
}

void Memory::print(ofstream& ofs) { //location is file location
    ofs << setw(10) << left <<  "Memory";
    ofs << ":\n";
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++)
            ofs << setw(2) << setfill('0') << int(data[(8*i) + j]) << ' ';
        ofs << '\n';
    }
    ofs << "#\n";
}
//Regs member functions:
int Regs::getVal(const string& reg) {
    if(reg.substr(1).size() != 1 || !isdigit(reg[1])) { //invalid register
        cout << "Error reading register value\n";
        return -1;
    }
    int i = reg[1] - 48; //get the value of X in RX in integer 
    if(i >= 0 && i < Regs::MAX_SIZE) return int(data[i]);
    else {
        cout << "Register does not exist for reading\n";
        return -1;
    }
}
void Regs::writeVal(const int& val , const string& reg) {
    if(reg.substr(1).size() != 1 || !isdigit(reg[1])) { //invalid register
        cout << "Error writing register value\n";
        return;
    }
    int i = reg[1] - 48;
    
    if(i >= 0 && i < Regs::MAX_SIZE) 
        data[i] = val;
    else {
        cout << "Register does not exist for writing\n";
        return;
    }
}
void Regs::clear(const string& reg) {
    int i = reg[1] - 48;
    if(i >= 0 && i < Regs::MAX_SIZE) data[i] = 0;
}
void Regs::print(ofstream& ofs) {
    ofs << setw(10) << left << "Registers";
    ofs << ':';
    for(int i = 0; i < Regs::MAX_SIZE; i++) {
        ofs << " "  << int(data[i]);
    }
    ofs << "#\n";
}
//Flags member functions:
void Flags::set(const int& flag , const bool& val) {
    data[flag] = val;
} 

bool Flags::get(const int& flag) {
    return data[flag];
}

void Flags::print(ofstream& ofs) {
    ofs << setw(10) << left << "Flags";
    ofs << ':';
    for(int i = 0; i < Flags::MAX_SIZE; i++) {
        ofs << " " << setprecision(2) << data[i];
    }
    ofs << "#\n";
}

//helper functions:

bool isValid(const char& c) { //checks if c is []/alphabet/number
    return (int(c) >= 48 && int(c) <= 57 || //number
            int(c) >= 65 && int(c) <= 90 || //uppercase
            int(c) >= 97 && int(c) <= 122 || //lowercase
            c == '[' || c == ']' || c == ' ' || c == ','); //others
}

char lowerCase(const char& c) {
    return (int(c) >= 65 && int(c) <= 90) ? c + 32 : c;
}

vector<string> split(const string& s) { //need to tolower(c)
    vector<string> words;
    string str = "";
    for(int i = 0; i < s.size(); i++) {
        if(isValid(s[i])) {
            switch(s[i]) {
                case ' ': case ',':
                    if(str != "") words.push_back(str);
                    str = "";

                break;

                default:
                    str += lowerCase(s[i]);  //ascii
                    break;
            }
        }
        
        else { //error in input
            
        }
    }
    if(str != "") words.push_back(str);
    return words;
    
}

int readFrom(const string& s) { //get val of loc
    char c = s[0];
    if(c == '[') {//mem [R0]
        string loc;
        for(auto c : s) {
            if(c == ']') break;
            loc += c;
        }
        if(c != ']') { //missing closing bracket
            cout << "Missing closing bracket in operand\n";
            return -1;
        }
        int i = reg.getVal(loc); //get address stored in reg s
        return mem.getVal(i);
    }

    else if(c == 'r') { //register
        return reg.getVal(s);
    }

    else if(isdigit(c)) {
        for(auto c : s) {
            if(!isdigit(c)) { //digit mixed with non digit
                cout << "Read error\n";
                return -1;
            }
        }
        return stoi(s); //literal
    }

    else {
        cout << "Unrecognised read: " << c << '\n';
        return -1; //unrecognisable
    }
    

    
}

void writeTo (const string& s , const int& i) { //write i to location in s
    char c = s[0];
    string loc;
    if(c == '[') {//mem
        for(auto c : s) {
            if(c == ']') break;
            loc += c;
        }
        if(c != ']') { //missing closing bracket
            cout << "Missing closing bracket in operand\n";
            return;
        }
        int addr = reg.getVal(loc); //write to address stored in reg s
        mem.writeVal(i , addr);
    }

    else if(c == 'r') { //register
        reg.writeVal(i , s);
    }

    else if(isdigit(c)) {
        for(auto c : s) {
            if(!isdigit(c)) { //digit mixed with non digit
                cout << "Write error\n";
                return;
            }
        }
        mem.writeVal(i , stoi(s));
    }
    return;
}


int* toBinary(int n) {
    int* arr = new int[8];
    int cur = 128;
    for(int i = 0; i < 8; i++) {
        if (n >= cur){
            n -= cur;
            arr[i] = 1;
        }
        else arr[i] = 0;    
        cur /= 2;
    }

    return arr;
} //to use this: int* arr = toBinary(int)


int toInt(int* arr) {
    int ans = 0;
    int cur = 128;
    for(int i = 0; i < 8; i++) {
        if(arr[i]) ans += cur;
        cur /= 2;
    }

    return ans;
}



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

    if (c == '+') 
        r = firstVal + secVal;
            
    else if(c == '-')
        r = secVal - firstVal;
            
    else if(c == '*')
        r = firstVal * secVal;

    else if(c == '/') {
        if(firstVal == 0) {
            cout << "Error: dividing by 0\n";
        }
        else 
            r = secVal / firstVal;
    }
        
            
    if(r > 255) flag.set(flag.CF , true);
    else if(r == 0) flag.set(flag.ZF , true); 
    int finalRes = r&255;
    writeTo(words[2] , finalRes);

}

/*void rotate(const char& c , vector<string>& words) {
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
    
}*/

void rotate(const char& c , vector<string>& words) { //reverse first and second part of array and reverse the whole thing
    int val = readFrom(words[0]); // the value in register
    int n = readFrom(words[1]) % 8; // rotate how many times
    int* arr = toBinary(val);
    n = n%8;
    n = (c == 'l') ? n : 8-n; //rotate left n times is rotate right 8-n times
    int sz = 8; //size of array

    if(n != 0) {
        for(int i = 0; i < n/2; i++) //reverse first n elements
            swap(arr[i] , arr[n - i - 1]);

        for(int i = 0; i < (8-n)/2; i++) //reverse last 8-n elements
            swap(arr[i+n] , arr[sz - i - 1]);

        for(int i = 0; i < sz/2; i++) //reverse whole arr
            swap(arr[i] , arr[sz - i - 1]);
    }

    int finalRes = toInt(arr);

    if(finalRes == 0) flag.set(flag.ZF , true); 

    writeTo(words[1] , finalRes);
}

void shift(const char& c , vector<string>& words) { 
    int val = readFrom(words[0]); // the value in register
    int n = readFrom(words[1]); // shift how many times

    int* arr = toBinary(val);

    switch(c) {
        case 'l':
            for(int i = 0 , j = n; j < 8; i++ , j++) //i is left pointer, j is right pointer
                arr[i] = arr[j];

            for(int i = 0; i < n; i++) 
                arr[7 - i] = 0;

            break;

        case 'r':
            for(int i = 7 - n, j = 7; i >= 0; i-- , j--) //i is left pointer, j is right pointer
                arr[j] = arr[i];

            for(int i = 0; i < n; i++) 
                arr[i] = 0;

            break;

        default: //unrecognised 
            break;

    }

    int finalRes = toInt(arr);
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
    writeTo(words[2] , val);
}

//other functions:
void errorHandler(const string& s) {

}
