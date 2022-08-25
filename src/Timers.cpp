#include "headers/Timers.h"

#define DIV_ADDRESS 0xFF04
#define TIMA_ADDRESS 0xFF05
#define TMA_ADDRESS 0xFF06
#define TAC_ADDRESS 0xFF07 

#define INTERRUPT_ENABLE 0xFFFF
#define INTERRUPT_FLAGS 0xFF0F

#define CLOCK_SPEED 4194304

Timers::Timers(){

}

Timers::Timers(RAM * ram):ram(ram){
    updateTAC();
}
void Timers::updateDIV(int requiredClocks){

    DIVCounter+=requiredClocks;
    if(DIVCounter>=256){
        DIV=ram->read(DIV_ADDRESS);
        DIV++;
        ram->write(DIV_ADDRESS,DIV);
    }
    DIVCounter=DIVCounter%256;
    
}
void Timers::updateTIMA(int requiredClocks){

    TIMACounter+=requiredClocks;
    if(TIMACounter>=TIMASpeed){
        TIMA=ram->read(TIMA_ADDRESS);
        if(TIMA==255){
            //request interrupt
            requestTimerInterrupt();
            TIMA=TMA;
        }
        else{
            TIMA++;
        }
    }
    ram->write(TIMA_ADDRESS,TIMA);
    TIMACounter=TIMACounter%TIMASpeed;
}
void Timers::updateTMA(){
    TMA=ram->read(TMA_ADDRESS);
}
void Timers::updateTAC(){
    TAC=ram->read(TAC_ADDRESS);
    timersEnabled=((TAC&0x4)>0);

    switch (TAC&0x03)
    {
    case 0b00:
        TIMASpeed=1024;
        break;
    case 0b01:
        TIMASpeed=16;
        break;
    case 0b10:
        TIMASpeed=64;
        break;
    case 0b11:
        TIMASpeed=256;
        break;
    default:
        break;
    }
    TIMAEnable=(ram->read(TAC_ADDRESS)&0x04)>>2;    //bit 2 of TAC toggles TIMA
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
    updateDIV(requiredClocks);

    if(timersEnabled==true){
        updateTIMA(requiredClocks);
    }

}
