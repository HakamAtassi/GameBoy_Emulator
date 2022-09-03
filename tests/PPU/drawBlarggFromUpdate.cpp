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
    gameboy.printTitle();
    gameboy.createWindow();


    for(int i=0;i<500;i++){ //each update is 69905 clocks
        gameboy.update();
        //gameboy.printRam(0x8000,0x9000);
    }


  
    //gameboy.dumpVram();


    //gameboy.drawPixelBuffer();
    //gameboy.printRam(0,0xFFFF);


//    gameboy.dumpPixelbuffer();




	SDL_Event eventMain;



	while(1){

		SDL_PollEvent(&eventMain);
		if(eventMain.type == SDL_QUIT)
				break;
	}

}