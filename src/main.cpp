#include "headers/Cartridge.h"
#include "headers/PPU.h"
#include "headers/GameBoy.h"
#include "headers/Debugger.h"
#include <iostream>
#include <vector>


using namespace std;
int main(){



    //Cartridge cartridge("../ROMS/Dr_Mario.gb");
    //Cartridge cartridge("../ROMS/blargg_cpu_instrs/individual/06-ld r, r.gb");
    //Cartridge cartridge("../ROMS/mooneye_tests/acceptance/ppu/hblank_ly_scx_timing-GS.gb");

/*
    Cartridge cartridge("../ROMS/dmg-acid2.gb");
    

    GameBoy gameboy(cartridge);
    gameboy.printTitle();
    gameboy.createWindow();
	SDL_Event eventMain;

	while(1){
        gameboy.update();
		SDL_PollEvent(&eventMain);
		if(eventMain.type == SDL_QUIT)
				break;
	}
*/





    Cartridge cartridge("../ROMS/dmg-acid2.gb");
    GameBoy * gameboy= new GameBoy(cartridge);
    Debugger debugger(gameboy);
    //printf("Starting test\n\n");

    for(int i=0;i<100000;i++){
        debugger.run();
    }


}
