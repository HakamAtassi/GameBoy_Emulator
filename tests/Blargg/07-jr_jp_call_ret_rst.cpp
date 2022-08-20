#include "../../src/headers/Cartridge.h"
#include "../../src/headers/GameBoy.h"
#include <iostream>

#define SB 0xFF01
#define SC 0xFF02



int main(){

    Cartridge cartridge("../../ROMS/blargg_cpu_instrs/individual/07-jr,jp,call,ret,rst.gb");
    GameBoy gameboy(cartridge);
    printf("Starting test\n\n");

    while(1){
        gameboy.update();
        if(gameboy.read(SC)==0x81){
            printf("%C",gameboy.read(SB));
            gameboy.write(SC,0);
        }
    }
}
        //FAILED (hangs)