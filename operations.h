#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <algorithm>
#include <unordered_map>

using namespace std;

string regNames(int counter);           //returns the register name, based on input, of any register from string vector reg
int binToDec(string);
string decToBin(int);

// R-FORMAT
void sll(string &, string, string);     //opcode: 0, function: 0
void srl(string &, string, string);     //opcode: 0, function: 2
void sra(string &, string, string);     //opcode: 0, function: 3
void sllv(string &, string, string);    //opcode: 0, function: 4
void srlv(string &, string, string);    //opcode: 0, function: 6
void srav(string &, string, string);    //opcode: 0, function: 7

void jr(string, int &);                 //opcode: 0, function: 8
void jalr(string &, string, int &);     //opcode: 0, function: 9
void movz(string &, string, string);    //opcode: 0, function: 10
void movn(string &, string, string);    //opcode: 0, function: 11
void syscall(string &, string, unordered_map<int,string>, int, bool &);   //opcode: 0, function: 12

void mfhi(string &, string);            //opcode: 0, function: 16
void mthi(string, string &);            //opcode: 0, function: 17
void mflo(string &, string);            //opcode: 0, function: 18
void mtlo(string, string &);            //opcode: 0, function: 19

void mult(string &, string, string);    //opcode: 0, function: 24
void multu(string &, string, string);   //opcode: 0, function: 25
void div(string &, string &, string, string);     //opcode: 0, function: 26
void divu(string &, string &, string, string);    //opcode: 0, function: 27
void add(string &, string, string);     //opcode: 0, function: 32
void addu(string &, string, string);    //opcode: 0, function: 33
void sub(string &, string, string);     //opcode: 0, function: 34
void subu(string &, string, string);    //opcode: 0, function: 35
void And(string &, string, string);     //opcode: 0, function: 36
void Or(string &, string, string);      //opcode: 0, function: 37
void Xor(string &, string, string);     //opcode: 0, function: 38
void nor(string &, string, string);     //opcode: 0, function: 39
void slt(string &, string, string);     //opcode: 0, function: 42
void sltu(string &, string, string);    //opcode: 0, function: 43

// J-FORMAT
void j(string, int &);                              //opcode: 2
void jal(string, int &, string &);                  //opcode: 3

// I-FORMAT
void beq(string, string, string, int &);            //opcode: 4
void bne(string, string, string, int &);            //opcode: 5
void blez(string, string, int &);                   //opcode: 6
void bgtz(string, string, int &);                   //opcode: 7

void addi(string &, string, string);                //opcode: 8
void addiu(string &, string, string);               //opcode: 9

void slti(string &, string, string);                //opcode: 10
void sltiu(string &, string, string);               //opcode: 11

void andi(string &, string, string);                //opcode: 12
void ori(string &, string, string);                 //opcode: 13
void xori(string &, string, string);                //opcode: 14

void lui(string &, string);                         //opcode: 15

void lb(string &, string, string, unordered_map<int,string> &, unordered_map<int,string> &);           //opcode: 32
void lh(string &, string, string, unordered_map<int,string> &, unordered_map<int,string> &);           //opcode: 33
void lw(string &, string, string, unordered_map<int,string> &, unordered_map<int,string> &);           //opcode: 35
void lbu(string &, string, string, unordered_map<int,string> &, unordered_map<int,string> &);          //opcode: 36
void lhu(string &, string, string, unordered_map<int,string> &, unordered_map<int,string> &);          //opcode: 37

void sb(string &, string, string, unordered_map<int,string> &, unordered_map<int,string> &);           //opcode: 40
void sh(string &, string, string, unordered_map<int,string> &, unordered_map<int,string> &);           //opcode: 41
void sw(string &, string, string, unordered_map<int,string> &, unordered_map<int,string> &);           //opcode: 43
