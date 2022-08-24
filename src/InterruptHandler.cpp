#include "headers/InterruptHandler.h"

#define INTERRUPT_ENABLE 0xFFFF
#define INTERRUPT_FLAGS 0xFF0F

//ISR addresses
#define VBlank 0x40
#define LCDSTAT 0x48
#define TIMER 0x50
#define SERIAL 0x58
#define JOYPAD 0x60

InterruptHandler::InterruptHandler(){};

InterruptHandler::InterruptHandler(CPU * cpu, RAM * ram, bool * IME):cpu(cpu), ram(ram),IME(IME)
{

}

InterruptHandler::~InterruptHandler(){   //do I need to delete cpu?
}

void InterruptHandler::pushWordToStack(uint16_t data){
    cpu->pushWordToStack(data);
}
void InterruptHandler::ISR(int interruptVal){
	pushWordToStack(cpu->getPC());	//save current state
	cpu->setHalt(false);
	*IME=false;
	uint8_t interruptRequests=ram->read(INTERRUPT_FLAGS);
	uint8_t interruptEnable=ram->read(INTERRUPT_ENABLE);

	interruptRequests=interruptRequests&(~(1<<interruptVal));	//reset IR bit
	ram->write(INTERRUPT_FLAGS,interruptRequests);

	//call ISR;
	switch (interruptVal)
	{
	case 0:	//VBLANK
		VBlankInterrupt();
		break;
	case 1:	//LCDSTAT
		STATInterrupt();
		break;
	case 2:	//TIMER
		TimerInterrupt();
		break;
	case 3:	//SERIAL
		SerialInterrupt();
		break;
	case 4:	//JOYPAD
		JoypadInterrupt();
		break;
	
	default:
		break;
	}
}
void InterruptHandler::handleInterrupts(){
		uint8_t interruptFlags=ram->read(INTERRUPT_FLAGS);
		uint8_t interruptEnable=ram->read(INTERRUPT_ENABLE);
		if(interruptFlags>0){	//dont handel interrupts if they are all 0
			for(int i=0;i<8;i++){	//bit 0 is higest prio.
				if(testBit<uint8_t>(interruptEnable,i)==true){	//is that intr. enabled
					if(testBit<uint8_t>(interruptFlags,i)==true){	//is that intr. requested
						if(*IME==true){
							ISR(i);	//run isr for that interrupt
						}
					}
				}
			}
		}
		if((interruptFlags&interruptEnable)>0){
			cpu->setHalt(false);
		}
}
void InterruptHandler::VBlankInterrupt(){
    cpu->setPC(VBlank);
}
void InterruptHandler::STATInterrupt(){
    cpu->setPC(LCDSTAT);
}
void InterruptHandler::TimerInterrupt(){
    cpu->setPC(TIMER);
}
void InterruptHandler::SerialInterrupt(){
    cpu->setPC(SERIAL);
}
void InterruptHandler::JoypadInterrupt(){
    cpu->setPC(JOYPAD);
}