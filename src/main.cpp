#include "headers/Cartridge.h"
#include "headers/GameBoy.h"



int main(){

    Cartridge cartridge("../ROMS/blargg_cpu_instrs/cpu_instrs.gb");
    GameBoy gameboy(cartridge);

    gameboy.printRam(0x00FF);
    gameboy.printTitle();

    for(int i=0;i<10;i++){
        gameboy.update();
    }

}