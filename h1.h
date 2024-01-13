#pragma once


class Memory {
    int data[64] = {};
    public:
        const int& MAX_SIZE = data.size();
        void print();
        void writeVal(const int& val , const int& addr);
        int getVal(const int& addr);
        void clear(const int& addr);

        Memory() {}
    
};

class Regs {
    int data[7] = {};
    public:
        const int& MAX_SIZE = data.size();
        void print();
        void writeVal(const int& val , const string& reg);
        int getVal(const string& reg);
        void clear(const string& reg);

        Regs() {}
};

class Flags {
    bool data[2] = {}; //{zero, carry}
    public:
        const int& ZF = 0;
        const int& CF = 1; 
        void print();
        void set(const int& flag , const bool& val);
        bool get(const int& addr);

        Flags() {}
};

void runner(); //openfile etc
void operate(const string&); //operate a line, call func based on first word

//helper functions:
vector<string> split(const string& , const char&); //split a string into words, then return vector of words
int readFrom(const string&);
void writeTo(const string& , const int&);
int toAByte(int const& r);
void conversion (int n);



//operation functions:
void in(vector<string>& words);
void out(vector<string>& words);
void mov(vector<string>& words);
void arth(const char& c , vector<string>& words);
void shift(const char& c , vector<string>& words)
void rotate(const char& c , vector<string>& words);
void load(vector<string>& words);
void store(vector<string>& words);

