#include "../../src/headers/Cartridge.h"
#include "../../src/headers/PPU.h"
#include "../../src/headers/GameBoy.h"


#include <iostream>
#include <vector>


using namespace std;
int main(){

    /*the data in the dump is everything I need to generate a single frame 
    using the ppu*/
    Cartridge cartridge("04-op r,imm.dump");
    GameBoy gameboy(cartridge);



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