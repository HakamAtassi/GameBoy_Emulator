#ifndef GAMEBOY_H_
#define GAMEBOY_H_
#include <string>
#include <memory>
#include "RAM.h"
#include "CPU.h"
#include "PPU.h"
#include "Cartridge.h"
#include "Timers.h"


class GameBoy
{
    private:
		RAM * ram;
        Timers timers;
        CPU cpu;
        PPU ppu;
        Cartridge cartridge;
        bool * IME=new bool(false);	//Inetrrupt Master Enable, seperate from the Inerrupt Enable

    
        template<typename T> //such that it works with 8 and 16 bit uint
        bool testBit(T data,int bitPos){
            return (data&bitPos);
        }
        void pushPC(uint16_t addr);
        void setPC(uint16_t _PC);

        /**interrupt handeling**/
		/*
		*	Bit 0 | VBlank	|	int 0x40
		*	Bit 1 | LCD STAT|	int 0x48
		*	Bit 2 | TIMER	|	int 0x50
		*	Bit 3 | Serial	|	int 0x58
		*	Bit 4 | Joypad	|	int 0x60
		*/
        void requestInterrupt(int interruptVal);    //sets bit corresponding to interrupt type
        void handleInterrupts(); //checks if ISR should be called
        void ISR(int interruptVal);    //calls interrupt service routine


    public:
        GameBoy();
        GameBoy(Cartridge cartridge);
        ~GameBoy();

        void update();
        void updateTimers(int requiredClocks);
		void refreshDisplay(); 
        void printRam(int maxAddr);
        void printTitle();
};





#endif
