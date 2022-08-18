#include <cstdint>
#include <iostream>
#include "assert.h"
#include "../../src/headers/CPU.h"
#include "../../src/headers/GameBoy.h"


int main(){ 
	GameBoy gameboy;    //initialize CPU

    gameboy.getRegs(); 


    gameboy.write(0x100,0x3E);  //load next byte to reg A
    gameboy.write(0x101,0x80);
    gameboy.write(0x102,0x47);  //load reg b into reg a
    

    gameboy.update();  
    gameboy.update();  


    gameboy.getRegs(); 

}
