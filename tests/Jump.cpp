
#include <cstdint>
#include <iostream>
#include "assert.h"
#include "../src/CPU.h"

using namespace GameBoy;
using namespace std;

int main(){ 

	CPU cpu;    //initialize CPU
                //PC starts at 0x100
    cpu.write(0x100,0x21);	//load into hl 
    cpu.write(0x101,0x00);	//hl low byte
    cpu.write(0x102,0x80);	//hl high byte
	cpu.write(0x103,0xE9);	//jump to contents in hl
	cpu.write(0x8000,0x01);	//load into bc
	cpu.write(0x8001,0x50);	//load low byte
	cpu.write(0x8002,0x00);	//load high byte
	


    cpu.fetch();
    cpu.execute();
    
    cpu.fetch();
    cpu.execute();

    cpu.fetch();
    cpu.execute();

    cpu.getRegs();

	/*
	AF: 0
	BC: 50
	DE: 0
	HL: 8000
	*/



}
