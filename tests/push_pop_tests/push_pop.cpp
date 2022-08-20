#include <cstdint>
#include <iostream>
#include "assert.h"
#include "../../src/headers/CPU.h"
#include "../../src/headers/GameBoy.h"


int main(){ 
	GameBoy gameboy;    //initialize CPU

    gameboy.getRegs(); 


    gameboy.write(0x100,0x21);  //load next word into BC
    gameboy.write(0x101,0x80);
    gameboy.write(0x102,0x47);  
    gameboy.write(0x103,0xE5);  //push BC to stack
    gameboy.write(0x104,0x21);  //load next word into BC
    gameboy.write(0x105,0x00);
    gameboy.write(0x106,0x00);  
    gameboy.write(0x107,0xE1);  //pop BC from stack





    
    gameboy.update();  
    gameboy.update();  
    gameboy.update();  
    gameboy.update();  
    gameboy.getRegs();

}
