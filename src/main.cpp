#include "headers/Cartridge.h"
#include "headers/GameBoy.h"
#include <iostream>

#define SB 0xFF01
#define SC 0xFF02



int main(){

    Cartridge cartridge("../ROMS/blargg_cpu_instrs/individual/04-op r,imm.gb");
    GameBoy gameboy(cartridge);

    //gameboy.printRam(0x0120);	printf("Before Reg1: %X",regs);

    gameboy.printTitle();
    std::cout<<"\n";

    //know bugs:
        //load (a16/a8) A/SP

    //things to look at:
        //CALL a16
        //RET and its variations

    while(1){

//        printf("PC: %X\n",gameboy.getPC());
//        printf("Instruction: %X\n",gameboy.read(gameboy.getPC()));
        gameboy.update();
        //std::cout<<"\n";
        if(gameboy.read(SC)==0x81){
            printf("SB: %C\n",gameboy.read(SB));
            gameboy.write(SC,0);
            std::cout<<"\n";
        }
    }

    //gameboy.printRamRange(0x8000,0x9BFF);



}