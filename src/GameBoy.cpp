#include "headers/GameBoy.h"
#include "headers/Cartridge.h"
#include "headers/CPU.h"
#include "headers/InterruptHandler.h"

#include <iostream>

#define INTERRUPT_ENABLE 0xFFFF
#define INTERRUPT_FLAGS 0xFF0F

//ISR addresses
#define VBlank 0x40
#define LCDSTAT 0x48
#define TIMER 0x50
#define SERIAL 0x58
#define JOYPAD 0x60


GameBoy::GameBoy():ram(new RAM){	//no cartridge... manually writing to ram
	timers=Timers(ram);
	cpu=new CPU(ram,IME);
	interruptHander=InterruptHandler(cpu,ram,IME);

};

GameBoy::GameBoy(Cartridge _cartridge):ram(new RAM){
	cartridge=_cartridge;
	for(int i=0;i<0xFFFF;i++){
		ram->write(i,_cartridge.read(i));
	}
	timers=Timers(ram);
	cpu=new CPU(ram,IME);
	interruptHander=InterruptHandler(cpu,ram,IME);
	ppu=PPU(ram);
}

GameBoy::~GameBoy(){
	delete ram;
	delete IME;
}

void GameBoy::pushWordToStack(uint16_t data){	
	cpu->pushWordToStack(data);
}

void GameBoy::setPC(uint16_t _PC){
	cpu->setPC(_PC);
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
	//while(clocks<69905){	//this function is called 60 times a second. Hence, this is done at a rate of 4 mHz
        int requiredClocks=cpu->fetchExecute();
		updateTimers(requiredClocks);
		//ppu.DoGraphics(requiredClocks);
		interruptHander.handleInterrupts();
		clocks+=requiredClocks;
	//}
	//refreshDisplay();	//since this function is called 60 times a second, refresh rate with be 60Hz. 
	
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


uint8_t GameBoy::read(uint16_t addr){
	return ram->read(addr);
}

void GameBoy::write(uint16_t addr, uint8_t data){	//for testing
	return ram->write(addr,data);
}

void GameBoy::getRegs(){
	cpu->getRegs();
}
uint8_t GameBoy::getInstruction(){
	return cpu->getInstruction();
}

uint16_t GameBoy::getPC(){
	cpu->getPC();
}

bool GameBoy::getFlag(std::string flag){
	return cpu->getFlag(flag);
}


void GameBoy::getTimers(){
	uint8_t DIV=ram->read(0xFF04);
	uint8_t TIMA=ram->read(0xFF05);
	uint8_t TMA=ram->read(0xFF06);
	uint8_t TAC=ram->read(0xFF07);
	printf("DIV: 0x%X => %d\n",DIV,DIV);
	printf("TIMA: 0x%X => %d\n",TIMA,TIMA);
	printf("TMA: 0x%X => %d\n",TMA,TMA);
	printf("TAC: 0x%X => %d\n",TAC,TAC);

}

void GameBoy::printRamRange(uint16_t begin, uint16_t end){
	printf("dumping ram from %X to %X\n",begin,end);
	for(uint16_t i=begin;i<=end;i++){
		printf("%X: %X\n",i,ram->read(i));
	}
}

void GameBoy::dumpVram(){
	ppu.dumpVram();
}

void GameBoy::updateGraphics(){
	ppu.updateGraphics();
}

void GameBoy::createWindow(){
	ppu.createWindow();
}

void GameBoy::drawPixelBuffer(){
	ppu.drawPixelBuffer();
}

void GameBoy::run(){

}

