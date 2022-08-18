#include "headers/Cartridge.h"
#include "headers/GameBoy.h"
#include <iostream>

#define SB 0xFF01


int main(){

    Cartridge cartridge("../ROMS/blargg_cpu_instrs/individual/ld_r_r.gb");
    GameBoy gameboy(cartridge);

    //gameboy.printRam(0x0120);	printf("Before Reg1: %X",regs);

    gameboy.printTitle();
    std::cout<<"\n";

    gameboy.printRamRange(0xC000,0xC0FF);
    gameboy.printRamRange(0x4000,0x40FF);


    while(gameboy.read(0x40FF)==0){
        printf("PC: %X\n",gameboy.getPC());
        printf("Instruction: %X\n",gameboy.read(gameboy.getPC()));
        gameboy.update();
        std::cout<<"\n";
    }
    printf("\n\nAFTER COPY\n\n");
    gameboy.printRamRange(0x4000,0x40FF);

    //the PC is writing some stuff to WRAM (0xCXXX)
    //but once done, it is jumping to 0xc2
    //it then executes several no ops, gets to 0x0100
    //then repeats the entire test. 

}