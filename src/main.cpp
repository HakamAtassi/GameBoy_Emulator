#include "headers/Cartridge.h"
#include "headers/GameBoy.h"



int main(){

    Cartridge cartridge("../ROMS/Tetris.gb");
    GameBoy gameboy(cartridge);

    gameboy.printRam(0x00FF);
    gameboy.printTitle();


}