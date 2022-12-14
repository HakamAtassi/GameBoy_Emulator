#include "../../../src/headers/Cartridge.h"
#include "../../../src/headers/GameBoy.h"
#include <iostream>

#define SB 0xFF01
#define SC 0xFF02



int main(){

    Cartridge cartridge("../../../ROMS/blargg_cpu_instrs/individual/11-op a,(hl).gb");
    GameBoy gameboy(cartridge);

    bool flag=1;

    for(int i=0;i<9000000;i++){
        gameboy.update();
        if(gameboy.read(SC)==0x81){
            printf("%C",gameboy.read(SB));
            gameboy.write(SC,0);
        }
    }
}

//passed