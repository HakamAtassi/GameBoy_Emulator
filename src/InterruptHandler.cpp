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
	printf("\n==\nHandeling interruptVal\n==\n");
	pushWordToStack(cpu->getPC());	//save current state
	cpu->setHalt(false);
	*IME=false;
	uint8_t interruptRequests=ram->read(INTERRUPT_FLAGS);
	uint8_t interruptEnable=ram->read(INTERRUPT_ENABLE);

	interruptRequests=interruptRequests&(~(1<<interruptVal));	//reset IR bit
	ram->write(INTERRUPT_FLAGS,interruptRequests);


	pushWordToStack(cpu->getPC());
	cpu->setHalt(false);


	switch(interruptVal)
	{
		case 0: cpu->setPC(0x40); break ;// V-Blank
		case 1: cpu->setPC(0x48); break ;// LCD-STATE
		case 2: cpu->setPC(0x50); break ;// Timer
		case 4: cpu->setPC(0x60); break ;// JoyPad
	}

	*IME = false ;
	interruptRequests=interruptRequests&(~(1<<interruptVal));	//reset IR bit
	ram->write(INTERRUPT_FLAGS,interruptRequests);
}


void InterruptHandler::handleInterrupts(){
	if (*IME==true)
	{
		// has anything requested an interrupt?
		uint8_t requestFlag = ram->read(0xFF0F);
		if (requestFlag > 0)
		{
			// which requested interrupt has the lowest priority?
			for (int bit = 0; bit < 8; bit++)
			{
				if ((requestFlag&(1<<bit))>0)
				{
					// this interupt has been requested. But is it enabled?
					uint8_t enabledReg = ram->read(0xFFFF);
					if ((enabledReg&(1<<bit))>0)
					{
						ISR(bit) ;
					}
				}
			}
		}
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