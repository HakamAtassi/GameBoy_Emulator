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
        bool * IME=new bool;	//Inetrrupt Master Enable, seperate from the Inerrupt Enable

    
        template<typename T> //such that it works with 8 and 16 bit uint
        bool testBit(T data,int bitPos){
            return (data&(1<<bitPos));
        }
        void pushWordToStack(uint16_t data);
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
        void updateHalt();      //checks halt status
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

        uint8_t read(uint16_t addr);
        void write(uint16_t addr,uint8_t data);    //for testing
        void getRegs();    //for testing
        void printRamRange(uint16_t begin, uint16_t end);   //for testing


        uint8_t getInstruction();    //return the next instruction to be executed
        uint16_t getPC();

        //TODO: read SB


};





#endif
