#include "headers/Timers.h"

#define DIV 0xFF04
#define TIMA 0xFF05
#define TMA 0xFF06
#define TAC 0xFF07 

#define INTERRUPT_ENABLE 0xFFFF
#define INTERRUPT_FLAGS 0xFF0F

#define CLOCK_SPEED 4194304

Timers::Timers(){

}

Timers::Timers(RAM * ram):ram(ram){
    updateTAC();
}

void Timers::updateTAC(){

    switch (ram->read(TAC)&0x03)
    {
    case 0b00:
        TIMASpeed=CLOCK_SPEED/1024;
        break;
    case 0b01:
        TIMASpeed=CLOCK_SPEED/16;
        break;
    case 0b10:
        TIMASpeed=CLOCK_SPEED/64;
        break;
    case 0b11:
        TIMASpeed=CLOCK_SPEED/256;
        break;
    default:
        break;
    }
    TIMAEnable=(ram->read(TAC)&0x04)>>2;    //bit 2 of TAC toggles TIMA
}

void Timers::updateTMA(){
    TIMAStart=ram->read(TMA);
}

void Timers::requestTimerInterrupt(){
    uint8_t interruptFlags=ram->read(INTERRUPT_FLAGS);
    interruptFlags|=1<<2;
    ram->write(INTERRUPT_FLAGS, interruptFlags);
}


void Timers::updateTimers(int requiredClocks){
	//0xFF04	DIV		256 clocks
    updateTAC();    //check to see if TAC has changed since last update. 
    updateTMA();    //check if TIMAStart has changed
    uint8_t DIVReg=ram->read(DIV);

    for(int i=0;i<requiredClocks;i++){
        if(DIVCounter==255){
            ram->write(DIV,DIVReg+1);	//increment contents at 0xFF04
            //TODO: writing to div should actually reset it. find another way. 
            DIVCounter=0;
        }
        else{
            DIVCounter++;
        }

        _TIMA=ram->read(TIMA);


        if(TIMACounter==((CLOCK_SPEED/TIMASpeed)-1)){
            _TIMA++;
            if(_TIMA>0xFF){
                requestTimerInterrupt();  
                _TIMA=TIMAStart;
            }
            ram->write(TIMA,_TIMA&0xFF);
            TIMACounter=0;
        }
        else{
            TIMACounter++;
        }
    }
}
