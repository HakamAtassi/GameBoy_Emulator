#include "../../src/headers/Cartridge.h"
#include "../../src/headers/GameBoy.h"
#include <iostream>

#define SB 0xFF01
#define SC 0xFF02



int main(){

    Cartridge cartridge_01("../../ROMS/mooneye_tests/acceptance/instr/daa.gb");
    GameBoy gameboy_01(cartridge_01);
    gameboy_01.printTitle();
    printf("Starting test\n\n");

    while(1){
        gameboy_01.update();
        if(gameboy_01.read(SC)==0x81){
            printf("%C",gameboy_01.read(SB));
            gameboy_01.write(SC,0);
        }
    }


}

//passed