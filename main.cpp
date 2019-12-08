// Jheovanny Camacho 109697217
// CSCI 113
// Final Project - MIPS Simulator
// C++ Version
#include "operations.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <unordered_map>
using namespace std;

    //global variables

    vector<string> reg(32, "");   //registers 0-31
    //REGISTERS
    /*
    $zero      //0:            zero register, always set to 0;
    $at        //1:            assembler temporary
    $v0-$v1    //2-3:          values, from expression evaluation and function results
    $a0-$a3    //4-7:          arguments
    $t0-$t9    //8-15, 24-25:  temporaries
    $s0-$s7    //16-23:        saved
    $k0-$k1    //26-27:        reserved for OS kernel
    $gp        //28:           global pointer
    $sp        //29:           stack pointer, initial value is address 0x7fffefff
    $fp        //30:           frame pointer
    $ra        //31:           return address
    */
    string hi, lo;                //HI and LO special registers

    unordered_map<int, string> text, data, stack; //hash map for the three arrays
        //int is the key, the address of that place in memory
        //string is the value, the content of that address location

    int lastTextAddr;   //holds last address in text segment (from reading file)
    int lastDataAddr;   //holds last address in data segment (from reading file)

    int programCounter = 0x00400000;    //program counter, starts at first instruction: initial address 0x00400000

string hexToBinary (string sourceHex);  //converts a hex string into a binary string

void displayRegisters();    //prints all contents of register 0-31, pc, hi, lo
void displayTables();       //prints all contents of text, data, and stack tables
void displayReadings();     //prints all contents which were read from given file, separated by text and data segment

void readFile(string file, bool &flag); //reads contents of a file and places them into proper table
void decode(string instruction);        //parses and decodes an instruction string and executes whichever operation it is
string regNames(int counter);           //returns the register name, based on input, of any register from string vector reg

bool exitFlag = false; //if activated (in syscall 10), it tells the program to stop executing instructions and print registers, then quit

int main()
{
    reg.at(0)  = "00000000000000000000000000000000";          //zero register ($zero) holds binary value of 0

    //reg.at(28) = "00010000000000001000000000000000";         //global pointer ($gp) holds address 0x10008000
    //reg.at(29) = "01111111111111111110111111111100";         //stack pointer ($sp) holds address 0x7fffeffc
    //reg.at(31) = "00000000010000000000000000000000";         //return address ($ra) holds adress 0x400000

    //assumptions for demo.txt
	/*
    reg.at(13) = decToBin(0x53fc); //$t5
    reg.at(14) = decToBin(0x6405); //$t6
    reg.at(16) = decToBin(0x10010000); //$s0
    reg.at(24) = decToBin(0x0); //$t8
    reg.at(25) = decToBin(0x0); //$t9
	*/

    //text.insert({0x00400000, registers.at(31)}); //the contents of register 31 ($ra) is assigned to map
    //data.insert({0x10010000, ""});
    //stack.insert({0x7fffeffc, registers.at(29)}); //stack starts at 0x7fffeffc, pointer at 0x7fffefff

    //user inputs a file to be read and simulated
    string fileName;
    cout << "************MIPS Simulator**************" << endl;
    cout << "Enter name of file to run: ";
    getline(cin, fileName);

    bool flag = true;           //flag will let us know if a file was found, or if program calls to exit
    readFile(fileName, flag);   //read file, distributing text and data segments to appropriate arrays
    if(flag == false) return 0; //if file was not found (flag becomes false), exit program

    //user chooses which mode to run
    char choice;
    cout << "Run-To-Completion (c) or Single-Step (s)?" << endl;
    cin >> choice;
    cout << endl;

            //ability to choose an execution mode
        // single-step
            //print out the value of a specified register or to print all registers
            //p reg print a specific register (e.g., p 4, prints the contents in hex of register 4)
            //p all print the contents of all registers, including the PC, HI, & LO in hex
            //d addr print the contents of memory location addr in hex, assume addr is a word address in hex.
            //s n execute the next n instructions and stop (should print each instruction executed),
                //then wait for the user to input another command
        // run-to-completion
            //simply execute the entire program and print out all the register contents at the end

    if(choice == 'c')
    {
        //RUN-TO-COMPLETION MODE
        while(programCounter <= lastTextAddr)
        {
            //decode and execute each element (instruction) of text array
            decode(text.at(programCounter));
                if(exitFlag) {continue;}       //if exit flag is triggered (in syscall execution), exit the loop and quit the program
            programCounter += 0x4;             //advancing 4 bytes (to next instruction)
        }
        cout << endl;
        displayReadings();
        displayTables();
        displayRegisters(); //after all instructions are executed, display contents of all registers
        if(exitFlag) {return 0;}        //checking if exit flag has been triggered, if it has: quit program
    }
    else if(choice == 's')
    {
        //SINGLE-STEP MODE
        while(programCounter <= lastTextAddr) //continue doing commands until program reaches its end (exceeds final text address)
        {
            cin.sync();                     //clears the input stream, allowing another input
            cout << "Use a command: ";      //user inputs command
            string input;
            getline(cin, input);

            char command = input[0];    //command holds the first character of input (p, d, s, etc.)
            string r; r.assign(input, 2, input.length()-2); //r holds the second part of input as a string

            if     (command == 'p') // p reg, p all
            {
                if(r == "all")
                    displayRegisters();
                else if(r == "pc" || r == "PC")
                        cout << hex << "PC: " << programCounter << dec << endl;
                else if(r == "hi" || r == "HI")
                {
                        cout << hex;
                        if(hi != "")    //if hi register is not empty,
                        {
                            int HI = stoull(hi, nullptr, 2);
                            cout << "HI: " << HI << endl; //print numerical value in hex
                        }
                        else
                            cout << "HI: " << hi << endl; //else, print empty string
                        cout << dec;
                }
                else if(r == "lo" || r == "LO")
                {
                        cout << hex;
                        if(lo != "")    //if lo register is not empty,
                        {
                            int LO = stoull(lo, nullptr, 2);
                            cout << "LO: " << LO << endl; //print numerical value in hex
                        }
                        else
                            cout << "LO: " << lo << endl; //else, print empty string
                        cout << dec;
                }
                else if(r.find_first_not_of( "0123456789" ) == string::npos) //checks if second part of input has anything that isn't a digit, making it unconvertible to an integer
                {
                    int regNum = stoi(r);       //since it is a number, convert and store in an int
                    string name = regNames(regNum);
                    int num;

                    if(reg.at(regNum) != "")
                    {
                        num = stoull(reg.at(regNum), nullptr, 2);
                        cout << "register " << regNum << " (" << name << ") " << ": " << hex << num << dec << endl;    //display the contents of that register
                    }
                    else
                        cout << "register " << regNum << " (" << name << ") " << ": " << reg.at(regNum) << endl;    //display empty register
                }
                else                                        //if the second part is not a number and is not "all", then it is not a valid command
                    cout << "not a valid command" << endl;
            }

            else if(command == 'd') // d addr
            {
                if(r.find_first_not_of( "0123456789xabcdef" ) == string::npos) //checks if second part of input has anything that isn't a hex digit, making it unconvertible to an integer
                {
                    int regNum = stoi(r, nullptr, 16);       //since it is a number, convert and store in an int
                    int contents;                            //holds contents of that area in memory after it is converted to an int

                    if(regNum >= 0x00400000 && regNum < 0x10010000) //bounds for text segment
                    {
                        if(text.find(regNum) != text.end())
                        {
                            contents = stoull(text.at(regNum), nullptr, 2); // converts binary string found in table to an integer
                            cout << "0x" << hex << regNum << ": " << contents << dec << endl;
                        }
                        else
                            cout << "that address does not exist" << endl;
                    }
                    else if(regNum >= 0x10010000 && regNum < 0x10018000) //bounds for data segment
                    {
                        if(data.find(regNum) != data.end())
                        {
                            contents = stoull(data.at(regNum), nullptr, 2); // converts binary string found in table to an integer
                            cout << "0x" << hex << regNum << ": " << data.at(regNum) << dec << endl;
                        }
                        else
                            cout << "that address does not exist" << endl;
                    }
                    else if(regNum >= 0x10018000 && regNum < 0x7fffefff) //bounds for stack segment
                    {
                        if(stack.find(regNum) != stack.end())
                        {
                            contents = stoull(stack.at(regNum), nullptr, 2); // converts binary string found in table to an integer
                            cout << "0x" << hex << regNum << ": " << stack.at(regNum) << dec << endl;
                        }
                        else
                            cout << "that address does not exist" << endl;
                    }
                    else
                        cout << "not a valid address" << endl;
                }
                else
                    cout << "not a valid command" << endl;
            }

            else if(command == 's') // s n
            {
                if(r.find_first_not_of( "0123456789" ) == string::npos) //checks if second part of input has anything that isn't a digit, making it unconvertible to an integer
                {
                    int n = stoi(r);       //since it is a number, convert and store in an int (number of instructions to execute)
                    int contents;
                    for(int i = 0; i < n; i++) //execute n instructions, n given by user
                    {
                        if(programCounter > lastTextAddr)   //if pc has exceeded the last text address, exit the loop to prevent out of range error
                            continue;
                        else                                //otherwise, decode and execute the instructions
                        {
                            decode(text.at(programCounter));

                            contents = stoull(text.at(programCounter), nullptr, 2); // converts binary string found in table to an integer
                            cout << "0x" << hex << programCounter << ": " << contents << dec << endl;

                                    if(exitFlag) {return 0;}       //if exit flag is triggered (in syscall execution), quit the program

                            programCounter += 0x4;             //advancing 4 bytes (to next instruction)

                            //print out each instruction executed
                        }
                    }

                    //after loop is finished must require user input again
                }

                else
                    cout << "not a valid command" << endl;

            }

            else cout << "not a valid command" << endl;

            cout << endl;
        }
    }
    else
        cout << "invalid input" << endl;
    cout << endl;

    return 0;
}

void displayRegisters()
{
    //displaying PC, HI, and LO
    cout << hex;
    cout << "PC: " << programCounter << endl; //print pc contents in hex

    if(hi != "")    //if hi register is not empty,
    {
        int HI = stoull(hi, nullptr, 2);
        cout << "HI: " << HI << endl; //print numerical value in hex
    }
    else
        cout << "HI: " << hi << endl; //else, print empty string
    if(lo != "")    //if lo register is not empty,
    {
        int LO = stoull(lo, nullptr, 2);
        cout << "LO: " << LO << endl; //print numerical value in hex
    }
    else
        cout << "LO: " << lo << endl; //else, print empty string

    cout << dec;

    //displaying contents of all 32 registers (in hex)
    for(int i = 0; i < 32; i++)
    {
        string name = regNames(i);
        int num;

        if(reg.at(i) != "")
        {
            num = stoull(reg.at(i), nullptr, 2);
            //cout << num << endl;
            cout << "register " << i << " (" << name << ") " << ": " << hex << num << dec << endl;    //display all contents of registers
        }
        else
            cout << "register " << i << " (" << name << ") " << ": " << reg.at(i) << endl;    //display empty registers
    }
}

void displayTables()
{
    //displaying contents of all three arrays
    for (auto& t: text)
        {cout << "0x" << hex << t.first << ": " << t.second << endl;}
    cout << endl;

    for (auto& d: data)
        {cout << "0x" << hex << d.first << ": " << d.second << endl;}
    cout << endl;

    for (auto& s: stack)
        {cout << "0x" << hex << s.first << ": " << s.second << endl;}
    cout << dec << endl;
}

void displayReadings()
{
    //display the contents which were read from file and stored into either text or data arrays
    for(int i = 0x00400000; i <= lastTextAddr; i += 0x4)
        cout << "text: " << text.at(i) << endl;
    for(int i = 0x10010000; i <= lastDataAddr; i += 0x4)
        cout << "data: " << data.at(i) << endl;

    cout << endl;
}


void readFile(string file, bool &flag)      //function to read the "binary" files contents
{
    ifstream input(file);       //using file given

    if(!input.is_open())        //if file is not open, display message
    {
        cout << file << " cannot be opened" << endl;
        flag = false;
        return;
    }

    int textCounter = 0x00400000;            //increment after every addition to text array, ensures proper placement of text
    bool nowInDataSegment = false;           //flag will tell us if we pass the DATA SEGMENT
    string temp;                             //temporary place for each line of text from file
    string binary;                           //we will place converted line into this string
    while(!input.eof())                      //while we have not reached end of file
    {
        getline(input, temp);                   //place line from file into temporary string

        if(temp == "DATA SEGMENT")
        {
             nowInDataSegment = true;           //we have reached DATA SEGMENT, set flag to true
             continue;                          //continue without storing "DATA SEGMENT" line
        }
        else
            if(!nowInDataSegment)               //if we haven't reached DATA SEGMENT,
            {
                binary = hexToBinary(temp);     //convert hex to binary and place into binary string

                text.insert({textCounter, binary});
                lastTextAddr = textCounter;
                textCounter += 0x4;                  //increment textCounter by four
            }

            else                                //otherwise we have reached DATA SEGMENT
            {
                string hex; hex.assign(temp, 0, 10);    //hex address of data is saved in string address
                int address = stoi(hex, 0, 16);         //convert hex address string to int

                temp.assign(temp, 11, 10);            //assign temp string to only the second half, e.g. from "0x10010000 0x6c65480a" to "0x6c65480a"
                binary = hexToBinary(temp);           //convert hex to binary and place into binary string

                lastDataAddr = address;
                data.insert({address, binary});         //hex address and binary data combo is stored into data map
            }
    }
}

void decode(string instruction)
{
    /*
        //break down binary to opcode, registers, address, etc.
            index 0-5   (bits 31-26) is opcode

            if(register format (opcode == 0))
                index 6-10  (bits 25-21) is source register 1
                index 11-15 (bits 20-16) is source register 2
                index 16-20 (bits 15-11) is destination register
                index 20-24 (bits 10-6)  is shift amount
                index 25-31 (bits 5-0)   is function
                //function determines which type of r-format instruction

            if(jump format (opcode == 2 or 3))
                index 6-31 (bits 25-0)  is address
                //opcode determines 2 -> jump,  3 -> jump and link

            if(immediate format (all others))
                index 6-10  (bits 25-21) is source register 1
                index 11-15 (bits 20-16) is source register 2
                index 16-31 (bits 15-0)  is immediate/address
                //opcode determines which type of i-format instruction
    */

    string opcode;  // operation code string (most significant 6 bits of instruction)
    string funct;   // function string (least significant 6 bits of instruction)

    opcode.assign(instruction, 0, 6);   // assign first 6 (most significant) bits of instruction to opcode string
    funct.assign(instruction, 26, 6);   // assign last 6 (least significant) bits of instruction to function string

    if(opcode == "000000")
    {
        //r-format
        //extract register positions and assign to separate strings
        string rs; rs.assign(instruction, 6, 5);
        string rt; rt.assign(instruction, 11, 5);
        string rd; rd.assign(instruction, 16, 5);
        string shamt; shamt.assign(instruction, 21, 5);

        //convert from binary string to decimal integer to give us position for registers vector
        int RSposition = binToDec(rs);
        int RTposition = binToDec(rt);
        int RDposition = binToDec(rd);

        if 		(funct == "000000") {sll(reg.at(RDposition), reg.at(RTposition), shamt);} // 0  sll
        else if (funct == "000010") {srl(reg.at(RDposition), reg.at(RTposition), shamt);} // 2  srl
        else if (funct == "000011") {sra(reg.at(RDposition), reg.at(RTposition), shamt);} // 3  sra
        else if (funct == "000100") {sllv(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 4  sllv
        else if (funct == "000110") {srlv(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 6  srlv
        else if (funct == "000111") {srav(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 7  srav
        else if (funct == "001000") {jr(reg.at(RSposition), programCounter);} // 8  jr
        else if (funct == "001001") {jalr(reg.at(RDposition), reg.at(RSposition), programCounter);} // 9  jalr
        else if (funct == "001010") {movz(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 10 movz
        else if (funct == "001011") {movn(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 11 movn
        else if (funct == "001100") {syscall(reg.at(2), reg.at(4), data, lastDataAddr, exitFlag);} // 12 syscall
        else if (funct == "010000") {mfhi(reg.at(RDposition), hi);} // 16 mfhi
        else if (funct == "010001") {mthi(reg.at(RDposition), hi);} // 17 mthi
        else if (funct == "010010") {mflo(reg.at(RDposition), lo);} // 18 mflo
        else if (funct == "010011") {mtlo(reg.at(RDposition), lo);} // 19 mtlo
        else if (funct == "011000") {mult(lo, reg.at(RSposition), reg.at(RTposition));} // 24 mult
        else if (funct == "011001") {multu(lo, reg.at(RSposition), reg.at(RTposition));} // 25 multu
        else if (funct == "011010") {div(lo, hi, reg.at(RSposition), reg.at(RTposition));} // 26 div
        else if (funct == "011011") {divu(lo, hi, reg.at(RSposition), reg.at(RTposition));} // 27 divu
        else if (funct == "100000") {add(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 32 add
        else if (funct == "100001") {addu(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 33 addu
        else if (funct == "100010") {sub(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 34 sub
        else if (funct == "100011") {subu(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 35 subu
        else if (funct == "100100") {And(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 36 and
        else if (funct == "100101") {Or(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 37 or
        else if (funct == "100110") {Xor(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 38 xor
        else if (funct == "100111") {nor(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 39 nor
        else if (funct == "101010") {slt(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 42 slt
        else if (funct == "101011") {sltu(reg.at(RDposition), reg.at(RSposition), reg.at(RTposition));} // 43 sltu
		else cout << "not supported" << endl;
    }

    if(opcode == "000010" || opcode == "000011")
    {
        //j-format
        string address; address.assign(instruction, 6, 26);

        if(opcode == "000010")
            {j(address, programCounter);}   // 2  jump
        else //(000011)
            {jal(address, programCounter, reg.at(31));} // 3  jump and link
    }

    else
    {
        //i-format
        string rs; rs.assign(instruction, 6, 5);
        string rt; rt.assign(instruction, 11, 5);
        string imm; imm.assign(instruction, 16, 16);

        int RSposition = binToDec(rs);
        int RTposition = binToDec(rt);

        if 		(opcode == "000100") {beq(reg.at(RTposition), reg.at(RSposition), imm, programCounter);} // 4  beq
        else if (opcode == "000101") {bne(reg.at(RTposition), reg.at(RSposition), imm, programCounter);} // 5  bne
        else if (opcode == "000110") {blez(reg.at(RSposition), imm, programCounter);} // 6  blez
        else if (opcode == "000111") {bgtz(reg.at(RSposition), imm, programCounter);} // 7  bgtz
        else if (opcode == "001000") {addi(reg.at(RTposition), reg.at(RSposition), imm);} // 8  addi
        else if (opcode == "001001") {addiu(reg.at(RTposition), reg.at(RSposition), imm);} // 9  addiu
        else if (opcode == "001010") {slti(reg.at(RTposition), reg.at(RSposition), imm);} // 10 slti
        else if (opcode == "001011") {sltiu(reg.at(RTposition), reg.at(RSposition), imm);} // 11 sltiu
        else if (opcode == "001100") {andi(reg.at(RTposition), reg.at(RSposition), imm);} // 12 andi
        else if (opcode == "001101") {ori(reg.at(RTposition), reg.at(RSposition), imm);} // 13 ori
        else if (opcode == "001110") {xori(reg.at(RTposition), reg.at(RSposition), imm);} // 14 xori
        else if (opcode == "001111") {lui(reg.at(RTposition), imm);} // 15 lui
        else if (opcode == "100000") {lb(reg.at(RTposition), reg.at(RSposition), imm, data, stack);} // 32 lb
        else if (opcode == "100001") {lh(reg.at(RTposition), reg.at(RSposition), imm, data, stack);} // 33 lh
        else if (opcode == "100011") {lw(reg.at(RTposition), reg.at(RSposition), imm, data, stack);} // 35 lw
        else if (opcode == "100100") {lbu(reg.at(RTposition), reg.at(RSposition), imm, data, stack);}// 36 lbu
        else if (opcode == "100101") {lhu(reg.at(RTposition), reg.at(RSposition), imm, data, stack);} // 37 lhu
        else if (opcode == "101000") {sb(reg.at(RTposition), reg.at(RSposition), imm, data, stack);} // 40 sb
        else if (opcode == "101001") {sh(reg.at(RTposition), reg.at(RSposition), imm, data, stack);} // 41 sh
        else if (opcode == "101011") {sw(reg.at(RTposition), reg.at(RSposition), imm, data, stack);} // 43 sw
        //else cout << "not supported" << endl;
    }
}

string hexToBinary (string sourceHex)
{
	string binary = "";
	for (int i = 2; i < sourceHex.length(); i++)    // starts at 2 to avoid considering "0x" at the beginning
	{
		switch (sourceHex[i])
		{
			case '0': binary.append("0000"); break;
			case '1': binary.append("0001"); break;
			case '2': binary.append("0010"); break;
			case '3': binary.append("0011"); break;
			case '4': binary.append("0100"); break;
			case '5': binary.append("0101"); break;
			case '6': binary.append("0110"); break;
			case '7': binary.append("0111"); break;
			case '8': binary.append("1000"); break;
			case '9': binary.append("1001"); break;
			case 'a': binary.append("1010"); break;
			case 'b': binary.append("1011"); break;
			case 'c': binary.append("1100"); break;
			case 'd': binary.append("1101"); break;
			case 'e': binary.append("1110"); break;
			case 'f': binary.append("1111"); break;
		}
	}
	return binary;
}

string regNames(int counter)
{
    switch(counter)
    {
        case 0: return "$zero"; break;

        case 1: return "$at"; break;

        case 2: return "$v0"; break;
        case 3: return "$v1"; break;

        case 4: return "$a0"; break;
        case 5: return "$a1"; break;
        case 6: return "$a2"; break;
        case 7: return "$a3"; break;

        case 8: return "$t0"; break;
        case 9: return "$t1"; break;
        case 10: return "$t2"; break;
        case 11: return "$t3"; break;
        case 12: return "$t4"; break;
        case 13: return "$t5"; break;
        case 14: return "$t6"; break;
        case 15: return "$t7"; break;

        case 16: return "$s0"; break;
        case 17: return "$s1"; break;
        case 18: return "$s2"; break;
        case 19: return "$s3"; break;
        case 20: return "$s4"; break;
        case 21: return "$s5"; break;
        case 22: return "$s6"; break;
        case 23: return "$s7"; break;

        case 24: return "$t8"; break;
        case 25: return "$t9"; break;

        case 26: return "$k0"; break;
        case 27: return "$k1"; break;

        case 28: return "$gp"; break;
        case 29: return "$sp"; break;
        case 30: return "$fp"; break;
        case 31: return "$ra"; break;
        default: return " "; break;
    }
}
