#pragma once

using namespace std;


class Memory {
    unsigned char data[64] = {};
    public:
        const int MAX_SIZE = 64;
        void print(ofstream& ofs);
        void writeVal(const int& val , const int& addr);
        int getVal(const int& addr);
        void clear(const int& addr);

        Memory() {
            for(int i = 0; i < MAX_SIZE; i++) 
                data[i] = 0; 
        }
    
};

class Regs {
    unsigned char data[7] = {};
    public:
        const int MAX_SIZE = 7;
        void print(ofstream& ofs);
        void writeVal(const int& val , const string& reg);
        int getVal(const string& reg);
        void clear(const string& reg);

        Regs() {
            for(int i = 0; i < MAX_SIZE; i++) 
                data[i] = 0; 
        }
};

class Flags {
    bool data[4] = {}; //{zero, carry , of , uf}
    public:
        const int MAX_SIZE = 4;
        const int ZF = 0;
        const int CF = 1; 
        const int OF = 2; 
        const int UF = 3; 
        void print(ofstream& ofs);
        void set(const int& flag , const bool& val);
        bool get(const int& addr);

        Flags() {
            for(int i = 0; i < MAX_SIZE; i++) 
                data[i] = 0; 
        }
};

void runner(); //openfile etc
void operate(const string&); //operate a line, call func based on first word

//helper functions:
bool isValid(const char&);
char lowerCase(const char&);
vector<string> split(const string&); //split a string into words, then return vector of words
int readFrom(const string&);
void writeTo(const string& , const int&);
int* toBinary(int n);
int toInt(int* arr);



//operation functions:
void in(vector<string>& words);
void out(vector<string>& words);
void mov(vector<string>& words);
void arth(const char& c , vector<string>& words);
void shift(const char& c , vector<string>& words);
void rotate(const char& c , vector<string>& words);
void load(vector<string>& words);
void store(vector<string>& words);

//other functions:
void errorHandler(const string&);

