#include "../../src/headers/Cartridge.h"
#include "../../src/headers/PPU.h"
#include "../../src/headers/GameBoy.h"


#include <iostream>
#include <vector>

#define SB 0xFF01
#define SC 0xFF02

using namespace std;
int main(){


    //Cartridge cartridge("04-op r,imm.dump");    //a varified correct memory dump file


    Cartridge cartridge("../../ROMS/Tetris.gb");
    GameBoy gameboy(cartridge);
    printf("Starting test\n\n");
    gameboy.printTitle();


    for(int i=0;i<99900000;i++){
        gameboy.update();
        if(gameboy.read(SC)==0x81){
            printf("%C",gameboy.read(SB));
            gameboy.write(SC,0);
        }
    }
 
    gameboy.printRam(0xFFFF);

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