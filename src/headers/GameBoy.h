#ifndef GAMEBOY_H_
#define GAMEBOY_H_
#include <string>
#include <memory>
#include "RAM.h"
#include "CPU.h"
#include "PPU.h"
#include "Cartridge.h"
#include "Timers.h"
#include "InterruptHandler.h"
#include "Joypad.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include "SDL2/SDL.h"

class GameBoy
{
    private:
		RAM * ram;
        Timers timers;
        CPU * cpu;
        PPU ppu;
        Joypad joypad;
        Cartridge cartridge;
        InterruptHandler interruptHander;

        SDL_Window *window;
        SDL_Event event;
        SDL_Renderer *renderer;
        SDL_Texture *texture;


        bool * IME=new bool;	//Inetrrupt Master Enable, seperate from the Inerrupt Enable

    
        template<typename T> //such that it works with 8 and 16 bit uint
        bool testBit(T data,int bitPos){
            return (data&(1<<bitPos))>0;
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
        void handleInterrupts(); //checks if ISR should be called
        void ISR(int interruptVal);    //calls interrupt service routine



    public:
        void requestInterrupt(int interruptVal);    //sets bit corresponding to interrupt type

        GameBoy();
        GameBoy(Cartridge cartridge);
        ~GameBoy();


        void update();
        void updateDebug();

        void updateTimers(int requiredClocks);
		void refreshDisplay(); 
        void printRam(int minAddr,int maxAddr);
        void dumpVram();
        void printTitle();
        void updateGraphics(int clocks);
        void createWindow();
        void drawPixelBuffer();
        void dumpPixelbuffer();
        void run();

        uint8_t read(uint16_t addr);
        void write(uint16_t addr,uint8_t data);    //for testing
        void getRegs();    //for testing
        void printRamRange(uint16_t begin, uint16_t end);   //for testing


        uint8_t getInstruction();    //return the next instruction to be executed
        uint16_t getPC();
        bool getFlag(std::string flag);
		void getTimers();
        void dumpVmem();


};





#endif
