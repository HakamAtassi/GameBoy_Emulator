#include "headers/GameBoy.h"
#include "headers/Cartridge.h"
#include "headers/CPU.h"
#include "headers/InterruptHandler.h"
#include "headers/Joypad.h"

#include <iostream>
#include <iomanip>


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
	for(int i=0xFF00;i<0xFF80;i++){
		ram->write(i,0xFF);
	}


	ram->write(0xFF00, 0xFF);
	ram->write(0xFF01, 0x00);
	ram->write(0xFF02, 0x7E);
	ram->write(0xFF04, 0xAB);
	ram->write(0xFF05, 0x00);
	ram->write(0xFF06, 0x00);
	ram->write(0xFF07, 0xF8);
	ram->write(0xFF0F, 0xE1);
	ram->write(0xFF10, 0x80);
	ram->write(0xFF11, 0xBF);
	ram->write(0xFF12, 0xF3);
	ram->write(0xFF14, 0xBF);
	ram->write(0xFF16, 0x3F);
	ram->write(0xFF17, 0x00);
	ram->write(0xFF19, 0xBF);
	ram->write(0xFF1A, 0x7F);
	ram->write(0xFF1B, 0xFF);
	ram->write(0xFF1C, 0x9F);
	ram->write(0xFF1E, 0xBF);
	ram->write(0xFF20, 0xFF);
	ram->write(0xFF21, 0x00);
	ram->write(0xFF22, 0x00);
	ram->write(0xFF23, 0xBF);
	ram->write(0xFF24, 0x77);
	ram->write(0xFF25, 0xF3);
	ram->write(0xFF26, 0xF1);
	ram->write(0xFF40, 0x91);
	ram->write(0xFF41, 0x87);
	ram->write(0xFF42, 0x00);
	ram->write(0xFF43, 0x00);
	ram->write(0xFF44, 0x00);
	ram->write(0xFF45, 0x00);
	ram->write(0xFF47, 0xFC);
	ram->write(0xFF48, 0xFF);
	ram->write(0xFF49, 0xFF);
	ram->write(0xFF4A, 0x00);
	ram->write(0xFF4B, 0x00);
	ram->write(0xFFFF, 0x00);
};

GameBoy::GameBoy(Cartridge _cartridge):ram(new RAM){
	cartridge=_cartridge;
	for(int i=0;i<0xFFFF;i++){
		ram->write(i,_cartridge.read(i));
	}
	timers=Timers(ram);
	cpu=new CPU(ram,IME);
	interruptHander=InterruptHandler(cpu,ram,IME);
	joypad=Joypad(ram);
	ppu=PPU(ram);


	for(int i=0xFF00;i<0xFF80;i++){
		ram->write(i,0xFF);
	}

	ram->write(0xFF00, 0xCF);
	ram->write(0xFF01, 0x00);
	ram->write(0xFF02, 0x7E);
	ram->write(0xFF04, 0xAB);
	ram->write(0xFF05, 0x00);
	ram->write(0xFF06, 0x00);
	ram->write(0xFF07, 0xF8);
	ram->write(0xFF0F, 0xE1);
	ram->write(0xFF10, 0x80);
	ram->write(0xFF11, 0xBF);
	ram->write(0xFF12, 0xF3);
	ram->write(0xFF14, 0xBF);
	ram->write(0xFF16, 0x3F);
	ram->write(0xFF17, 0x00);
	ram->write(0xFF19, 0xBF);
	ram->write(0xFF1A, 0x7F);
	ram->write(0xFF1B, 0xFF);
	ram->write(0xFF1C, 0x9F);
	ram->write(0xFF1E, 0xBF);
	ram->write(0xFF20, 0xFF);
	ram->write(0xFF21, 0x00);
	ram->write(0xFF22, 0x00);
	ram->write(0xFF23, 0xBF);
	ram->write(0xFF24, 0x77);
	ram->write(0xFF25, 0xF3);
	ram->write(0xFF26, 0xF1);
	ram->write(0xFF40, 0x91);
	ram->write(0xFF41, 0x87);

	ram->write(0xFF42, 0x00);
	ram->write(0xFF43, 0x00);
	ram->write(0xFF44, 0x00);
	ram->write(0xFF45, 0x00);
	ram->write(0xFF47, 0xFC);
	ram->write(0xFF48, 0xFF);
	ram->write(0xFF49, 0xFF);
	ram->write(0xFF4A, 0x00);
	ram->write(0xFF4B, 0x00);
	ram->write(0xFFFF, 0x00);


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
	while(clocks<69905){	//this function is called 60 times a second. Hence, this is done at a rate of 4 mHz
        int requiredClocks=cpu->fetchExecute();
		//printf("required clocks= %d\n",requiredClocks);
		int instruction=cpu->getInstruction();
		updateTimers(requiredClocks);
		updateGraphics(requiredClocks);
		interruptHander.handleInterrupts();
		clocks+=requiredClocks;
		//joypad.getInput();
/**
		if(ram->read(0xFF02)==0x81){
            printf("%C",ram->read(0xFF01));
            ram->write(0xFF02,0);
        }
**/
	}
	drawPixelBuffer();	//update what is displayed to the window only after a scanline is drawn


	//refreshDisplay();	//since this function is called 60 times a second, refresh rate with be 60Hz. 
	
}

void GameBoy::updateDebug(){
    const int cyclesPerUpdate=69905;
	//4194304 clocks per second of CPU
	//60 updates per second (to achive a refresh rate of 60hz)
	//results in 4194304/60 = 69905 updates per second

	int clocks=0;
	int requiredClocks=cpu->fetchExecute();
	updateTimers(requiredClocks);
	updateGraphics(requiredClocks);
	interruptHander.handleInterrupts();
	clocks+=requiredClocks;
	//joypad.getInput();


	drawPixelBuffer();	//update what is displayed to the window only after a scanline is drawn


	//refreshDisplay();	//since this function is called 60 times a second, refresh rate with be 60Hz. 
	
}

void GameBoy::printRam(int minAddr, int maxAddr){

	for(int i=minAddr;i<=(maxAddr&0xFFF0);i+=0x10){
    	std::cout<<std::uppercase<<std::hex<<i<<"\t";
		for(int j=0; j<=0xF;j++){
			std::cout<<std::left<<std::setw(3)<<std::uppercase<<std::hex<<(int)ram->read(i+j);
		}
		std::cout<<"\n";
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

void GameBoy::updateGraphics(int clocks){
	ppu.updateGraphics(clocks);
	
}

void GameBoy::createWindow(){
	ppu.createWindow();
}

void GameBoy::drawPixelBuffer(){
	ppu.drawPixelBuffer();
}

void GameBoy::dumpPixelbuffer(){
	ppu.dumpPixelbuffer();
}

void GameBoy::run(){

}

