#include "../../src/headers/Cartridge.h"
#include "../../src/headers/GameBoy.h"
#include <iostream>
#include <vector>

#define SB 0xFF01
#define SC 0xFF02



int main(){
    std::vector<int> used_instructions(0xFF,0);

    Cartridge cartridge("../../ROMS/blargg_cpu_instrs/individual/03-op sp, hl.gb");
    GameBoy gameboy(cartridge);
    printf("Starting test\n\n");

    for(int i=0;i<1000000;i++){
        used_instructions[gameboy.getInstruction()]++;
        gameboy.update();
        if(gameboy.read(SC)==0x81){
            printf("%C",gameboy.read(SB));
            gameboy.write(SC,0);
        }
    }
    for(int i=0;i<0xFF;i++){
        printf("instruction: 0x%X => %d\n",i,used_instructions[i]);
    }
}

//passed