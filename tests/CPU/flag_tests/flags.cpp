#include <cstdint>
#include <iostream>
#include "assert.h"
#include "../../src/headers/CPU.h"
#include "../../src/headers/GameBoy.h"


int main(){ 
	GameBoy gameboy;    //initialize CPU




    gameboy.write(0x100,0x3E);  //ld A, d8
    gameboy.write(0x101,0b10000001);  //ld A, d8

    gameboy.write(0x102,0x1F);  //RrCA



    
    gameboy.update();  
    gameboy.getRegs(); 
    printf("\n");

    gameboy.update();  
    gameboy.getRegs();
    printf("\n");
    printf("Zero: %d\n",gameboy.getFlag("zero"));
    printf("negative: %d\n",gameboy.getFlag("negative"));
    printf("halfCarry: %d\n",gameboy.getFlag("halfCarry"));
    printf("carry: %d\n",gameboy.getFlag("carry"));


}
