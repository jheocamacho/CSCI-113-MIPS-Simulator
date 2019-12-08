#include "operations.h"

int binToDec(string bin)
{
    int dec;

    if(bin.length() == 5) //5 bit register
        dec = bitset<5>(bin).to_ulong();
    else if (bin.length() == 16) //16 bit immediate/branch address
        dec = bitset<16>(bin).to_ulong();
    else if (bin.length() == 26) //26 bit jump address
        dec = bitset<26>(bin).to_ulong();
    else if (bin.length() == 32) //32 bit word/instruction
        dec = bitset<32>(bin).to_ulong();

    return dec;
}

string decToBin(int dec)
{
    string bin;

    bin = bitset<32>(dec).to_string();

    return bin;
}

//-----------R-FORMAT-------------------------------------------------//
void sll(string &rd, string rt, string shamt)
{
    //R[rd] = R[rt] << shamt
    int reg1 = binToDec(rt);
    int shift = binToDec(shamt);
    int result = reg1 << shift;
    rd = decToBin(result);
}

void srl(string &rd, string rt, string shamt) // uses shift right arithmetic operand (>>) //use & to mask out the bits before original signed bit
{
    //R[rd] = R[rt] >> shamt
    unsigned int reg1 = binToDec(rt);
    unsigned int shift = binToDec(shamt);
    unsigned int result = reg1 >> shift;
    rd = decToBin(result);
}

void sra(string &rd, string rt, string shamt)
{
    //R[rd] = R[rt] >>> shamt
    int reg1 = binToDec(rt);
    int shift = binToDec(shamt);
    int result = reg1 >> shift;
    rd = decToBin(result);
}

void sllv(string &rd, string rs, string rt)
{
    //R[rd] = R[rt] << R[rs]
    int reg1 = binToDec(rt);
    int shift = binToDec(rs);
    int result = reg1 << shift;
    rd = decToBin(result);
}

void srlv(string &rd, string rs, string rt) // uses shift right arithmetic operand (>>)
{
    //R[rd] = R[rt] >> R[rs]
    unsigned int reg1 = binToDec(rt);
    unsigned int shift = binToDec(rs);
    unsigned int result = reg1 >> shift;
    rd = decToBin(result);
}

void srav(string &rd, string rs, string rt)
{
    //R[rd] = R[rt] >>> rs
    int reg1 = binToDec(rt);
    int shift = binToDec(rs);
    int result = reg1 >> shift;
    rd = decToBin(result);
}

void jr(string rs, int &pc)
{
    int addr = binToDec(rs);
    pc = addr;  //set pc to address given from register
    pc -= 0x4;  //this offset prevents the program counter from incrementing past the intended address
                //(the program counter ALWAYS increments by 4 after EVERY instruction)
}

void jalr(string &rd, string rs, int &pc)
{
    pc += 0x4;          //advance pc so when it is pulled from rd, it will point to the next instruction
    rd = decToBin(pc);  //store that location into register rd

    int regs = binToDec(rs);
    pc = regs;
    pc -= 0x4;  //this offset prevents the program counter from incrementing past the intended address
                //(the program counter ALWAYS increments by 4 after EVERY instruction)

}

void movz(string &rd, string rs, string rt)
{
    int regt = binToDec(rt);
    if(regt == 0)
    {
        rd = rs;
    }
}

void movn(string &rd, string rs, string rt)
{
    int regt = binToDec(rt);
    if(regt != 0)
    {
        rd = rs;
    }
}

//syscall goes here


void mfhi(string &rd, string hi)        //? does rd have to be cleared after moving contents to hi?
{
    rd = hi;
}

void mthi(string rd, string &hi)
{
    hi = rd;
}

void mflo(string &rd, string lo)
{
    rd = lo;
}

void mtlo(string rd, string &lo)
{
    lo = rd;
}

void mult(string &lo, string rs, string rt)
{
    int regs = binToDec(rs);
    int regt = binToDec(rt);
    lo = decToBin(regs * regt);
}

void multu(string &lo, string rs, string rt)
{
    unsigned int regs = binToDec(rs);  //storing the conversion into an unsigned int will have it hold the unsigned version of the binary string value
    unsigned int regt = binToDec(rt);
    lo = decToBin(regs * regt);
}

void div(string &lo, string &hi, string rs, string rt)
{
    int regs = binToDec(rs);
    int regt = binToDec(rt);
    lo = decToBin(regs / regt);
    hi = decToBin(regs % regt);
}

void divu(string &lo, string &hi, string rs, string rt)
{
    unsigned int regs = binToDec(rs);
    unsigned int regt = binToDec(rt);
    lo = decToBin(regs / regt);
    hi = decToBin(regs % regt);
}

void add(string &rd, string rs, string rt)
{
    //R[rd] = R[rs] + R[rt]
    int reg1 = binToDec(rs);
    int reg2 = binToDec(rt);
    rd = decToBin(reg1 + reg2);
}

void addu(string &rd, string rs, string rt)
{
    //R[rd] = R[rs] + R[rt]
    unsigned int reg1 = binToDec(rs);
    unsigned int reg2 = binToDec(rt);
    rd = decToBin(reg1 + reg2);
}

void sub(string &rd, string rs, string rt)
{
    //R[rd] = R[rs] - R[rt]
    int reg1 = binToDec(rs);
    int reg2 = binToDec(rt);
    rd = decToBin(reg1 - reg2);
}

void subu(string &rd, string rs, string rt)
{
    //R[rd] = R[rs] - R[rt]
    unsigned int reg1 = binToDec(rs);
    unsigned int reg2 = binToDec(rt);
    rd = decToBin(reg1 - reg2);
}

void And(string &rd, string rs, string rt)
{
    //R[rd] = R[rs] & R[rt]
    int reg1 = binToDec(rs);
    int reg2 = binToDec(rt);
    rd = decToBin(reg1 & reg2);
}

void Or(string &rd, string rs, string rt)
{
    //R[rd] = R[rs] | R[rt]
    int reg1 = binToDec(rs);
    int reg2 = binToDec(rt);
    rd = decToBin(reg1 | reg2);
}

void Xor(string &rd, string rs, string rt)
{
    //R[rd] = R[rs] ^ R[rt]
    int reg1 = binToDec(rs);
    int reg2 = binToDec(rt);
    rd = decToBin(reg1 ^ reg2);
}

void nor(string &rd, string rs, string rt)
{
    //R[rd] = ~ (R[rs] | R[rt])
    int reg1 = binToDec(rs);
    int reg2 = binToDec(rt);
    rd = decToBin(~(reg1 | reg2));
}

void slt(string &rd, string rs, string rt)
{
    // R[rd] = (R[rs] < R[rt]) ? 1 : 0
    int reg1 = binToDec(rs);
    int reg2 = binToDec(rt);
    if(reg1 < reg2)
        rd.assign(decToBin(1));
    else
        rd.assign(decToBin(0));
}

void sltu(string &rd, string rs, string rt)
{
    // R[rd] = (R[rs] < R[rt]) ? 1 : 0
    unsigned int reg1 = binToDec(rs);
    unsigned int reg2 = binToDec(rt);
    if(reg1 < reg2)
        rd.assign(decToBin(1));
    else
        rd.assign(decToBin(0));
}

//-----------J-FORMAT-------------------------------------------------//
void j(string address, int &pc) //?
{
    //PC=JumpAddr
       //JumpAddr =    { PC+4[31:28], address, 2’b0 }

    //PC = nPC; nPC = (PC & 0xf0000000) | (target << 2);

       //you go PC=(PC&first four bits) | (addr*4)
       //so the first 4 bits of the PC stays the same, the next 26 are what was in the last 26 of the jump instruction,
       //the last 2 bits are zeros

    int newpc = pc & 0xf0000000;
    int addr = binToDec(address);
    addr <<= 2;
    pc = newpc | addr;

    pc -= 0x4;  //this offset prevents the program counter from incrementing past the intended address
                //(the program counter ALWAYS increments by 4 after EVERY instruction)
}

void jal(string address, int &pc, string &reg31) //?
{
    //R[31]=PC+8;PC=JumpAddr
                  //JumpAddr =    { PC+4[31:28], address, 2’b0 }

    reg31 = pc + 0x8;           //register 31 ($ra) will store the next next instruction
    pc = binToDec(address);
}

//-----------I-FORMAT-------------------------------------------------//
void beq(string rs, string rt, string address, int &pc) //?
{
    //if(R[rs]==R[rt])  PC=PC+4+BranchAddr
                              //BranchAddr = { 14{immediate[15]}, immediate, 2’b0 }

    //if $s == $t advance_pc (offset << 2)); else advance_pc (4);

    int brAddr = binToDec(address);
    if (address[0]=='1') brAddr |= 0xFFFF0000; //important! checks if branch address is negative (sign bit is 1), then fills the higher 16 bits with 1's, leaving the lower 16 bits as the address
    brAddr <<= 2;

    if(rs == rt)
    {
        pc += brAddr; //pc = brAddr
        pc -= 0x4;    //this offset prevents the program counter from incrementing past the intended address
                      //(the program counter ALWAYS increments by 4 after EVERY instruction)
    }
}

void bne(string rs, string rt, string address, int &pc) //?
{
    //if(R[rs]!=R[rt])  PC=PC+4+BranchAddr
                              //BranchAddr = { 14{immediate[15]}, immediate, 2’b0 }

    int brAddr = binToDec(address);
    if (address[0]=='1') brAddr |= 0xFFFF0000; //important! checks if branch address is negative (sign bit is 1), then fills the higher 16 bits with 1's, leaving the lower 16 bits as the address
    brAddr <<= 2;

    if(rs != rt)
    {
        pc += brAddr;
        pc -= 0x4;    //this offset prevents the program counter from incrementing past the intended address
                      //(the program counter ALWAYS increments by 4 after EVERY instruction)
    }
}

void blez(string rs, string address, int &pc) //?
{
    //if(R[rs]<=0)  PC=PC+4+BranchAddr
                              //BranchAddr = { 14{immediate[15]}, immediate, 2’b0 }
    int regs = binToDec(rs);
    int brAddr = binToDec(address);
    if (address[0]=='1') brAddr |= 0xFFFF0000; //important! checks if branch address is negative (sign bit is 1), then fills the higher 16 bits with 1's, leaving the lower 16 bits as the address
    brAddr <<= 2;

    if(regs <= 0)
    {
        pc += brAddr;
        pc -= 0x4;    //this offset prevents the program counter from incrementing past the intended address
                      //(the program counter ALWAYS increments by 4 after EVERY instruction)
    }
}

void bgtz(string rs, string address, int &pc) //?
{
    //if(R[rs]>0)  PC=PC+4+BranchAddr
                              //BranchAddr = { 14{immediate[15]}, immediate, 2’b0 }
    int regs = binToDec(rs);
    int brAddr = binToDec(address);
    if (address[0]=='1') brAddr |= 0xFFFF0000; //important! checks if branch address is negative (sign bit is 1), then fills the higher 16 bits with 1's, leaving the lower 16 bits as the address
    brAddr <<= 2;

    if(regs > 0)
    {
        pc += brAddr;
        pc -= 0x4;    //this offset prevents the program counter from incrementing past the intended address
                      //(the program counter ALWAYS increments by 4 after EVERY instruction)
    }
}

void addi(string &rt, string rs, string imm)
{
    //R[rt] = R[rs] + SignExtImm
    int reg = binToDec(rs);
    int im = binToDec(imm);
    rt = decToBin(reg + im);
}

void addiu(string &rt, string rs, string imm)
{
    //R[rt] = R[rs] + SignExtImm
    unsigned int reg = binToDec(rs);
    unsigned int im = binToDec(imm);
    rt = decToBin(reg + im);
}

void slti(string &rt, string rs, string imm)
{
    //R[rt] = (R[rs] < SignExtImm) ? 1 : 0
    int reg = binToDec(rs);
    int im = binToDec(imm);
    if(reg < im)
        rt.assign(decToBin(1));
    else
        rt.assign(decToBin(0));
}

void sltiu(string &rt, string rs, string imm)
{
    //R[rt] = (R[rs] < SignExtImm) ? 1 : 0
    unsigned int reg = binToDec(rs);
    unsigned int im = binToDec(imm);
    if(reg < im)
        rt.assign(decToBin(1));
    else
        rt.assign(decToBin(0));
}

void andi(string &rt, string rs, string imm)
{
    //R[rt] = R[rs] & ZeroExtImm
    int reg = binToDec(rs);
    int im = binToDec(imm);
    rt = decToBin(reg & im);
}

void ori(string &rt, string rs, string imm)
{
    //R[rt] = R[rs] | ZeroExtImm (rs OR immediate -> rt)
    int reg = binToDec(rs);
    int im = binToDec(imm);
    rt = decToBin(reg | im);
}

void xori(string &rt, string rs, string imm)
{
    //R[rt] = R[rs] ^ ZeroExtImm
    int reg = binToDec(rs);
    int im = binToDec(imm);
    rt = decToBin(reg ^ im);
}

void lui(string &dReg, string address)
{
    //R[rt] = {imm, 16’b0}
    dReg = address;
    dReg.append("0000000000000000");
}

void lb(string &rt, string rs, string offset, unordered_map<int,string> &data, unordered_map<int,string> &stack)
{
    //R[rt] = M[R[rs]+SignExtImm]
    int reg = binToDec(rs);
    int off = binToDec(offset);
    int result = reg + off;

    if(result >= 0x10010000 && result < 0x10018000)     //bounds for data array, if address lands in here
        rt = data.at(result);                           //address is taken from data array and stored into register rt
    else if(result >= 0x10018000 && result < 0x7fffefff)//bounds for stack array, if address lands in here
        rt = stack.at(result);                          //address is taken from stack array and stored into register rt
}

void lh(string &rt, string rs, string offset, unordered_map<int,string> &data, unordered_map<int,string> &stack)
{
    //R[rt] = M[R[rs]+SignExtImm]
    int reg = binToDec(rs);
    int off = binToDec(offset);
    int result = reg + off;

    if(result >= 0x10010000 && result < 0x10018000)     //bounds for data array, if address lands in here
        rt = data.at(result);                           //address is taken from data array and stored into register rt
    else if(result >= 0x10018000 && result < 0x7fffefff)//bounds for stack array, if address lands in here
        rt = stack.at(result);                          //address is taken from stack array and stored into register rt
}

void lw(string &rt, string rs, string offset, unordered_map<int,string> &data, unordered_map<int,string> &stack)
{
    //R[rt] = M[R[rs]+SignExtImm]
    int reg = binToDec(rs);
    int off = binToDec(offset);
    int result = reg + off;

    if(result >= 0x10010000 && result < 0x10018000)     //bounds for data array, if address lands in here
        rt = data.at(result);                           //address is taken from data array and stored into register rt
    else if(result >= 0x10018000 && result < 0x7fffefff)//bounds for stack array, if address lands in here
        rt = stack.at(result);                          //address is taken from stack array and stored into register rt
}

void lbu(string &rt, string rs, string offset, unordered_map<int,string> &data, unordered_map<int,string> &stack)
{
    //R[rt]={24’b0,M[R[rs] +SignExtImm](7:0)}
    unsigned int reg = binToDec(rs);
    unsigned int off = binToDec(offset);
    unsigned int result = reg + off;

    if(result >= 0x10010000 && result < 0x10018000)     //bounds for data array, if address lands in here
        rt = data.at(result);                           //address is taken from data array and stored into register rt
    else if(result >= 0x10018000 && result < 0x7fffefff)//bounds for stack array, if address lands in here
        rt = stack.at(result);                          //address is taken from stack array and stored into register rt
}

void lhu(string &rt, string rs, string offset, unordered_map<int,string> &data, unordered_map<int,string> &stack)
{
    //R[rt]={16’b0,M[R[rs] +SignExtImm](15:0)}
    unsigned int reg = binToDec(rs);
    unsigned int off = binToDec(offset);
    unsigned int result = reg + off;

    if(result >= 0x10010000 && result < 0x10018000)     //bounds for data array, if address lands in here
        rt = data.at(result);                           //address is taken from data array and stored into register rt
    else if(result >= 0x10018000 && result < 0x7fffefff)//bounds for stack array, if address lands in here
        rt = stack.at(result);                          //address is taken from stack array and stored into register rt
}

void sb(string &rt, string rs, string offset, unordered_map<int,string> &data, unordered_map<int,string> &stack)
{
    //M[R[rs]+SignExtImm](7:0) = R[rt](7:0)
    int reg = binToDec(rs);
    int off = binToDec(offset);
    int result = reg + off;

    if(result >= 0x10010000 && result < 0x10018000)     //bounds for data array, if address lands in here
    {
        if(data.find(result) != data.end())         //if key already exits in map
            data.erase(result);                     //erase contents of that key/value pair, replace in the next line
        data.insert({result, rt});                  //contents of rt are stored in data array at address that results
    }

    else if(result >= 0x10018000 && result < 0x7fffefff)//bounds for stack array, if address lands in here
    {
        if(stack.find(result) != stack.end())
        stack.erase(result);
        stack.insert({result, rt});                     //contents of rt are stored in stack array at address that results
    }
}

void sh(string &rt, string rs, string offset, unordered_map<int,string> &data, unordered_map<int,string> &stack)
{
    //M[R[rs]+SignExtImm](15:0) = R[rt](15:0)
    int reg = binToDec(rs);
    int off = binToDec(offset);
    int result = reg + off;

    if(result >= 0x10010000 && result < 0x10018000)     //bounds for data array, if address lands in here
    {
        if(data.find(result) != data.end())
            data.erase(result);
        data.insert({result, rt});                      //contents of rt are stored in data array at address that results
    }

    else if(result >= 0x10018000 && result < 0x7fffefff)//bounds for stack array, if address lands in here
    {
        if(stack.find(result) != stack.end())
        stack.erase(result);
        stack.insert({result, rt});                     //contents of rt are stored in stack array at address that results
    }
}

void sw(string &rt, string rs, string offset, unordered_map<int,string> &data, unordered_map<int,string> &stack)
{
    //M[R[rs]+SignExtImm] = R[rt]
    int reg = binToDec(rs);
    int off = binToDec(offset);
    int result = reg + off;

    if(result >= 0x10010000 && result < 0x10018000)     //bounds for data array, if address lands in here
    {
        if(data.find(result) != data.end())
            data.erase(result);
        data.insert({result, rt});                      //contents of rt are stored in data array at address that results
    }

    else if(result >= 0x10018000 && result < 0x7fffefff)//bounds for stack array, if address lands in here
    {
        if(stack.find(result) != stack.end())
            stack.erase(result);
        stack.insert({result, rt});                     //contents of rt are stored in stack array at address that results
    }
}

//in sum.s
//1111111111111111110111111111111 (7FFFEFFF) - 40 =
//1111111111111111110111111010111 (7FFFEFD7) (2147479511)
//0000000000100100 (36)
//2147479511 + 36 = 2147479547 -> 01111111111111111110111111111011 (7FFFEFFB)




void syscall(string &$v0, string $a0, unordered_map<int,string> data, int endOfData, bool &exitFlag)
{
    if      (binToDec($v0) == 1)   //if code is 1, print integer from $a0
    {
        int integer = binToDec($a0);   //convert binary string to decimal integer
        cout << integer << endl;       //print integer
    }

    else if (binToDec($v0) == 4)   //if code is 4, print string from $a0 (address of string)
    {
        //go to last line of ascii data
        //add first 8 char of data string to new string
        //add next 8 char to front of that string
        //keep doing that for rest of line
        //go to line above that and do again

        bitset<8> binaryString;

        //convert $a0 to int
        int address = stoi($a0, nullptr, 2);    //convert contents of $a0 (reg 4) into an integer, this the address where the string is located

        string inorder; //this will hold all our bits together which will be converted to ascii

        for(endOfData; endOfData >= address; endOfData -= 0x4)
        {
            string rawData = data.at(endOfData); //place contents, of that address in data, into a buffer string

            inorder.insert(0, rawData, 0, 8);   //assign the data array element (raw data) to string inorder
            inorder.insert(0, rawData, 8, 8);   //must assign 8 bits at a time, each time in front of the previous 8 bits
            inorder.insert(0, rawData, 16, 8);  //this is due to Qtspim using Little Endian for string data, setting everything reversed
            inorder.insert(0, rawData, 24, 8);  //we must start at the last string data address and get the bits in reverse
        }                                       //and work our way up to the address given in $a0 (beginning of string data)

        istringstream in(inorder);  //push in stream
        while(in >> binaryString)
            cout << char(binaryString.to_ulong());  //convert to character (ascii)
    }
    else if (binToDec($v0) == 5)   //if code is 5, read integer
    {
        cin.sync();                         //clears input stream
        int input;                          //variable to hold input
        cin >> input;                       //user inputs number
        $v0 = decToBin(input);              //number converted to binary string and stored into $v0 (reg 2)
    }

    else if (binToDec($v0) == 8)   //if code is 8, read string
    {
        /*
        cin.sync();                  //clears input stream
        string input;                //input string will hold the user input
        string holder = "";          //holder will hold the bits in the proper order before being placed into the right spot in memory
        getline(cin, input);         //gather user input

        //convert input into a binary string
        for(int i = 0; i < input.length(); i++)
        {
            holder += bitset<8>((int)input.at(i)).to_string();
        }

        cout << holder << endl;

        //do some magic to set holder into proper order inside inorder
        string inorder;
        for(int i = 0; i < holder.length(); i+=8)
        {
            inorder.insert(0, holder, i, 8);
        }
        cout << inorder << endl;

        //add 32 bit chunks of new reversed string back into memory
        string chunk;
        for(int i = 0; i < inorder.length(); i+=32)
        {
            chunk.assign(inorder, i, i+32);
            cout << chunk << endl;
            //data.insert({new address, chunk});
        }
        */;
    }
    else if (binToDec($v0) == 10)   //if code is 10, exit
        exitFlag == true;           //this flag will initialize exit procedure in main function
    else
        cout << "not applicable to syscall" << endl << endl;    //otherwise, print out message
}
