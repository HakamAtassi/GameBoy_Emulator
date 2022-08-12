#include "headers/GameBoy.h"
#include "headers/Cartridge.h"
#include "headers/CPU.h"
#include <iostream>

#define INTERRUPT_ENABLE 0xFFFF
#define INTERRUPT_FLAGS 0xFF0F

//ISR addresses
#define VBlank 0x40
#define LCDSTAT 0x48
#define TIMER 0x50
#define SERIAL 0x58
#define JOYPAD 0x60


GameBoy::GameBoy(){};

GameBoy::GameBoy(Cartridge _cartridge):ram(new RAM){
	cartridge=_cartridge;
	for(int i=0;i<0xFFFF;i++){
		ram->write(i,_cartridge.read(i));
	}
	timers=Timers(ram);
	CPU cpu(ram);
}

GameBoy::~GameBoy(){
	free(ram);
}

void GameBoy::pushPC(uint16_t addr){	//TODO:a Implement this function

}

void GameBoy::setPC(uint16_t _PC){
	cpu.setPC(_PC);
}


void GameBoy::requestInterrupt(int interruptVal){    //sets bit corresponding to interrupt type
	int interruptReg=ram->read(INTERRUPT_FLAGS);
	interruptReg=interruptReg|(1<<interruptVal);
	return;
}

void GameBoy::handleInterrupts(){
	if(IME==true){
		uint8_t interruptRequests=ram->read(INTERRUPT_FLAGS);
		uint8_t interruptEnable=ram->read(INTERRUPT_ENABLE);
		if(interruptRequests>0){	//dont handel interrupts if they are all 0
			for(int i=0;i<5;i++){	//bit 0 is higest prio.
				if(testBit<uint8_t>(interruptEnable,i)==true){	//is that intr. enabled
					if(testBit<uint8_t>(interruptRequests,i)==true){	//is that intr. requested
						ISR(i);	//run isr for that interrupt

					}
				}
			}
		}
	}
}

void GameBoy::ISR(int interruptVal){
	pushPC(cpu.getPC());	//save current state

	IME=false;
	uint8_t interruptRequests=ram->read(INTERRUPT_FLAGS);
	uint8_t interruptEnable=ram->read(INTERRUPT_ENABLE);

	interruptRequests=interruptRequests&(~(1<<interruptVal));	//reset IR bit
	ram->write(INTERRUPT_FLAGS,interruptRequests);

	//call ISR;
	switch (interruptVal)
	{
	case 0:	//VBLANK
		setPC(VBlank);
		break;
	case 1:	//LCDSTAT
		setPC(LCDSTAT);
		break;
	case 2:	//TIMER
		setPC(TIMER);
		break;

	case 3:	//SERIAL
		setPC(SERIAL);
		break;
	case 4:	//JOYPAD
		setPC(JOYPAD);
		break;
	
	default:
		break;
	}

}


void GameBoy::refreshDisplay(){
};


void GameBoy::updateTimers(int requiredClocks){
	timers.updateTimers(requiredClocks);
}


void GameBoy::update(){
    const int cyclesPerUpdate=69905;
	//4194304 clocks per second of CPU
	//60 updates per second (to achive a refresh rate of 60hz)
	//results in 4194304/60 = 69905 updates per second

	int clocks=0;
	while(clocks<69905){	//this function is called 60 times a second. Hence, this is done at a rate of 4 mHz
        int requiredClocks=cpu.fetchExecute();
		updateTimers(requiredClocks);
		ppu.updateGraphics(requiredClocks);
		handleInterrupts();
		clocks+=requiredClocks;
	}
	refreshDisplay();	//since this function is called 60 times a second, refresh rate with be 60Hz. 
}

void GameBoy::printRam(int maxAddr){
	for(int i=0;i<=maxAddr;i++){
		printf("%X: %X  ",i,ram->read(i));
	}
	std::cout<<"\n";
}

void GameBoy::printTitle(){
	std::cout<<"Cartridge title: "<<cartridge.Title<<"\n";
}