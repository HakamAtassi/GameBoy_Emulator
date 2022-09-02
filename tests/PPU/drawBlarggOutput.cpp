#include "../../src/headers/Cartridge.h"
#include "../../src/headers/PPU.h"
#include "../../src/headers/GameBoy.h"


#include <iostream>
#include <vector>

#define SB 0xFF01
#define SC 0xFF02

using namespace std;
int main(){

    /*the data in the dump is everything I need to generate a single frame 
    using the ppu*/


    //Cartridge cartridge("04-op r,imm.dump");    //a varified correct memory dump file


    Cartridge cartridge("../../ROMS/blargg_cpu_instrs/individual/06-ld r, r.gb");
    GameBoy gameboy(cartridge);
    printf("Starting test\n\n");

    for(int i=0;i<90000000;i++){
        gameboy.update();
        if(gameboy.read(SC)==0x81){
            printf("%C",gameboy.read(SB));
            gameboy.write(SC,0);
        }
    }
    //gameboy.dumpVram();



   // ppu.dumpPixelbuffer();
 
    for(int i=0;i<144;i++){
        gameboy.updateGraphics();
    }
    
    gameboy.createWindow();

	SDL_Event eventMain;


	while(1){

        gameboy.drawPixelBuffer();
		SDL_PollEvent(&eventMain);
		if(eventMain.type == SDL_QUIT)
				break;
	}
    
}