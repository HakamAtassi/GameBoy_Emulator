#include "../../../src/headers/Cartridge.h"
#include "../../../src/headers/GameBoy.h"
#include "../../../src/headers/Debugger.h"

#include <iostream>
#include <vector>

#define SB 0xFF01
#define SC 0xFF02



int main(){

    Cartridge cartridge("../../../ROMS/blargg_cpu_instrs/individual/03-op sp, hl.gb");

/*
    GameBoy gameboy(cartridge);
    printf("Starting test\n\n");


    for(int i=0;i<1000000;i++){
        gameboy.update();
        if(gameboy.read(SC)==0x81){
            printf("%C",gameboy.read(SB));
            gameboy.write(SC,0);
        }
    }
*/

    GameBoy * gameboy= new GameBoy(cartridge);
    Debugger debugger(gameboy);
    //printf("Starting test\n\n");

    for(int i=0;i<100000;i++){
        debugger.run();        
    }


}

//passed