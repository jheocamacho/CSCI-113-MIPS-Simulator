This is the MIPS Simulator project for CSCI 113, written by Jheovanny Camacho.

This program will take in a text file containing hexadecimal instructions and data and will convert it to binary strings.
The instructions are stored in an integer address using unordered map.
There are three maps, one for text, data, and stack.
The key is an integer representing the address, and the value is a string representing the contents in binary.

In the main function, depending on the mode, the program counter will go through a loop, starting at the first instruction.
It will decode the specific instruction at that location in the text map.
Based on opcode and function bits, once it knows the equivalent instruction, it will execute it.
After all instructions have been executed (or if syscall 10 is called), the program will exit.
Nearly every line of code has comments explaining the purpose and expectations of that code.

Every operation should be working, however, I could not complete syscall 8, read string.
There is code attempting to complete it but I commented it out to avoid any conflicts with the rest of the program.

Other than that, everything should, hopefully, be working.