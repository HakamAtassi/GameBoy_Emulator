#include "../../src/headers/Cartridge.h"
#include "../../src/headers/GameBoy.h"
#include "../../src/headers/Debugger.h"

#include <iostream>
#include <vector>
#define SB 0xFF01
#define SC 0xFF02


/*
int main(){

    Cartridge cartridge("../../ROMS/blargg_cpu_instrs/individual/07-jr,jp,call,ret,rst.gb");
    GameBoy gameboy(cartridge);
    //printf("Starting test\n\n");

    std::vector<int> used_instructions(0xFF,0);

    while(1){
        if(gameboy.getPC()==0xFFFF){
            break;
        }
        //printf("PC: %X\n",gameboy.getPC());
        //printf("Instr: %X\n",gameboy.getInstruction());
        used_instructions[gameboy.getInstruction()]++;

        gameboy.update();
        if(gameboy.read(SC)==0x81){
            printf("%C",gameboy.read(SB));
            gameboy.write(SC,0);
        }
    }
    for(int i=0;i<=0xFF;i++){
        printf("instruction: 0x%X => %d\n",i,used_instructions[i]);
    }
}
        //FAILED (hangs)

*/

int main(){

    Cartridge cartridge("../../ROMS/blargg_cpu_instrs/individual/07-jr,jp,call,ret,rst.gb");
    GameBoy * gameboy= new GameBoy(cartridge);
    Debugger debugger(gameboy);
    //printf("Starting test\n\n");


    while(1){
        debugger.run();
        if(gameboy->read(SC)==0x81){
            printf("%C",gameboy->read(SB));
            gameboy->write(SC,0);
        }
    }

}