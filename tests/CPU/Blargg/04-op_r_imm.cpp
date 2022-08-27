#include "../../src/headers/Cartridge.h"
#include "../../src/headers/GameBoy.h"
#include "../../src/headers/Debugger.h"

#include <iostream>

#define SB 0xFF01
#define SC 0xFF02





int main(){

    Cartridge cartridge("../../ROMS/blargg_cpu_instrs/individual/04-op r,imm.gb");
    GameBoy * gameboy= new GameBoy(cartridge);
    Debugger debugger(gameboy);
    //printf("Starting test\n\n");


    while(1){
        debugger.run();        
    }

}

//passed