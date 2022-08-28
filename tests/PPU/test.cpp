#include "../../src/headers/Cartridge.h"
#include "../../src/headers/GameBoy.h"
#include <iostream>

#define SB 0xFF01
#define SC 0xFF02

int main(){

   //Cartridge cartridge_01("../../ROMS/blargg_cpu_instrs/individual/01-special.gb");
    Cartridge cartridge_01("../../ROMS/Tetris.gb");

    GameBoy gameboy_01(cartridge_01);
    gameboy_01.printTitle();
    for(int i=0;i<99999999;i++){
        gameboy_01.update();
        if(gameboy_01.read(SC)==0x81){
            printf("%C",gameboy_01.read(SB));
            gameboy_01.write(SC,0);
        }
    }
}