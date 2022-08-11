#include "headers/GameBoy.h"



GameBoy::GameBoy(Cartridge cartridge):ram(new RAM){
	for(int i=0;i<0xFFFF;i++){
		ram->write(i,cartridge.read(i));
	}
	
}

GameBoy::~GameBoy(){
	free(ram);
}





void GameBoy::update(){
    const int cyclesPerUpdate=69905;
	//4194304 clocks per second of CPU
	//60 updates per second (to achive a refresh rate of 60hz)
	//results in 4194304/60 = 69905 updates per second

	int clocks=0;
	while(clocks<69905){	//this function is called 60 times a second. Hence, this is done at a rate of 4 mHz
        int requiredClocks=cpu.fetchExecute();
		cpu.updateTimers(requiredClocks);
		ppu.updateGraphics(requiredClocks);
		cpu.Handleinterrupts();
		clocks+=requiredClocks;
	}
	refreshDisplay();	//since this function is called 60 times a second, refresh rate with be 60Hz. 
}
