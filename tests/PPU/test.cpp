#include "../../src/headers/Cartridge.h"
#include "../../src/headers/PPU.h"

#include <iostream>
#include <vector>

#define SB 0xFF01
#define SC 0xFF02

using namespace std;
int main(){

    /*the data in the dump is everything I need to generate a single frame 
    using the ppu*/
    Cartridge cartridge("04-op r,imm.dump");
    PPU ppu(cartridge);
    //ppu.dumpVram();
   // ppu.dumpPixelbuffer();

/*
    for(int i=0;i<500;i++){
        ppu.drawVram();
    }
*/
    ppu.createWindow();



	SDL_Event eventMain;

	while(1){
        ppu.drawVram();
        ppu.drawPixelBuffer();
		SDL_PollEvent(&eventMain);
		if(eventMain.type == SDL_QUIT)
				break;
	}
    
}