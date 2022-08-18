#include <cstdint>
#include <iostream>
#include "assert.h"
#include "../src/headers/CPU.h"

using namespace std;

int main(){ 

    RAM * ram=new RAM;
    bool * IME;
	CPU cpu(ram,IME);    //initialize CPU
                //PC starts at 0x100
    cpu.write(0x100,0x11);  //load next 8 bits into reg BC;
    cpu.write(0x101,0x00);  //DE lower byte
    cpu.write(0x102,0x80);  //DE upper byte
    cpu.write(0x103,0x13);  //increment bc

    cpu.fetchExecute();
    cpu.fetchExecute();

    cpu.getRegs(); 

    /*
    Expected:
    AF: 183
    BC: 13
    DE: 8001
	HL: 14D
    */
    
}
