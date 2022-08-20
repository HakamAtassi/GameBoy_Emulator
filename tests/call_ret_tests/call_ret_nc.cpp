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
    gameboy.write(0x102,0x47);  //ld B, A
    gameboy.write(0x103,0xCD);  //push PC, jump 0xC0DO
    gameboy.write(0x104,0xD0);
    gameboy.write(0x105,0xC0);
    gameboy.write(0xC0D0,0x06);  //ld A, d8
    gameboy.write(0xC0D1,0xF0);
    gameboy.write(0xC0D2,0x3C); //inc A
    gameboy.write(0xC0D3,0xD0); //return if carry is 0
    gameboy.write(0x106,0x06);  //ld A, d8
    gameboy.write(0x107,0xFF);


    

    gameboy.update();  
    gameboy.update();  
    gameboy.update();  
    gameboy.update();  
    gameboy.update();  
    gameboy.update();  
    gameboy.update();  


}
