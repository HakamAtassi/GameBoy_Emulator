#include <cstdint>
#include <iostream>
#include "assert.h"
#include "../../src/headers/CPU.h"
#include "../../src/headers/GameBoy.h"


int main(){ 
	GameBoy gameboy;    //initialize CPU

    gameboy.getRegs(); 


    gameboy.write(0x100,0x21);  //ld d16 into hl
    gameboy.write(0x101,0x70);
    gameboy.write(0x102,0x80);
    gameboy.write(0x103,0x3E);  //write d8 into reg A
    gameboy.write(0x104,0x66);
    gameboy.write(0x105,0x22);  //write A into (HL)



    gameboy.update();  
    gameboy.getRegs(); 
    std::cout<<"\n";
    gameboy.update();  
    gameboy.getRegs(); 
    std::cout<<"\n";
    gameboy.update();  
    gameboy.getRegs(); 

    printf("Memory at 0x8070: %X",gameboy.read(0x8070));
}
