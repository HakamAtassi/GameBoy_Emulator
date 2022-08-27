#include <cstdint>
#include <iostream>
#include "assert.h"
#include "../src/CPU.h"

using namespace GameBoy;
using namespace std;

int main(){ 
    assert(cerr);
	CPU cpu;    //initialize CPU
                //PC starts at 0x100
    cpu.write(0x100,0x01);  //load next 8 bits into reg BC;
    cpu.write(0x101,0x00);  //BC lower byte
    cpu.write(0x102,0x80);  //BC upper byte
    cpu.write(0x103,0x03);  //increment bc

    cpu.fetch();
    cpu.execute();

    cpu.fetch();
    cpu.execute();

    cpu.getRegs(); 

    /*
    Expected:
    AF: 0
    BC: 8001
    DE: 0
	HL: 0
    */

}
