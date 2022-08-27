#include <cstdint>
#include <iostream>
#include "assert.h"
#include "../../src/headers/CPU.h"
#include "../../src/headers/GameBoy.h"

#define INTERRUPT_ENABLE 0xFFFF
#define INTERRUPT_FLAGS 0xFF0F

#define DIV_ADDRESS 0xFF04
#define TIMA_ADDRESS 0xFF05
#define TMA_ADDRESS 0xFF06
#define TAC_ADDRESS 0xFF07 

int main(){ 
	GameBoy gameboy;    //initialize CPU

    gameboy.write(TAC_ADDRESS,0x04);
    gameboy.write(0x100,0xFB);
    gameboy.write(0x101,0x21);
    gameboy.write(0x102,0xFF);
    gameboy.write(0x103,0xFF);
    gameboy.write(0x104,0x36);
    gameboy.write(0x105,0x4);


    printf("\n===Update 1===\n");
    gameboy.update();  
	gameboy.getTimers();
    printf("\n===Update 2===\n");
    gameboy.update();  
	gameboy.getTimers();
    printf("\n===Update 3===\n");
    gameboy.update();  
	gameboy.getTimers();
    printf("\n===Update 4===\n");
    gameboy.update();  
	gameboy.getTimers();
    printf("\n===Update 5===\n");
    gameboy.update();  
	gameboy.getTimers();
	



    /*
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

	*/

}
