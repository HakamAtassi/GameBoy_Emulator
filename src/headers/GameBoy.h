#ifndef GAMEBOY_H_
#define GAMEBOY_H_
#include <string>
#include <memory>
#include "RAM.h"
#include "CPU.h"
#include "PPU.h"
#include "Cartridge.h"


class GameBoy
{
    private:
		RAM * ram;	//pointer that self deletes when GameBoy 
									//no longer exists
        CPU cpu;
        PPU ppu;
        Cartridge cartridge;
    

    public:
        GameBoy();
        ~GameBoy();

        GameBoy(Cartridge cartridge);

        void update();
		void refreshDisplay(); 

};





#endif
