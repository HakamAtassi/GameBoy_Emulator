#include "../../src/headers/Cartridge.h"
#include "../../src/headers/PPU.h"
#include "../../src/headers/GameBoy.h"
#include "../../src/headers/Debugger.h"


#include <iostream>
#include <vector>

#define SB 0xFF01
#define SC 0xFF02

using namespace std;
int main(){


    Cartridge cartridge("../../ROMS/dmg-acid2.gb");
  

    GameBoy gameboy(cartridge);
    printf("Starting test\n\n");
    gameboy.printTitle();


    gameboy.createWindow();


	SDL_Event eventMain;
	while(1){
        gameboy.update();
        gameboy.printRam(0xff00,0xff70);
        gameboy.drawPixelBuffer();
		SDL_PollEvent(&eventMain);
		if(eventMain.type == SDL_QUIT)
				break;
	}
    //gameboy.printRam(0xff00,0xff70);

 /*
    GameBoy * gameboy= new GameBoy(cartridge);
    Debugger debugger(gameboy);
    //printf("Starting test\n\n");

    for(int i=0;i<100000;i++){
        debugger.run();        
        gameboy->printRam(0xff00,0xff70);
    }
   */
}