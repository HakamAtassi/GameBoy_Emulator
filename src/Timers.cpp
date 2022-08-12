#include "headers/Timers.h"

#define DIV 0xFF04
#define TIMA 0xFF05
#define TMA 0xFF06
#define TAC 0xFF07 
#define CLOCK_SPEED 4194304

Timers::Timers(){

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


Timers::Timers(RAM * ram):ram(ram){
    updateTAC();
}


void Timers::updateTimers(int requiredClocks){
	//0xFF04	DIV		256 clocks
    updateTAC();    //check to see if TAC has changed since last update. 
    updateTMA();    //check if TIMAStart has changed
    for(int i=0;i<requiredClocks;i++){
        if(DIVCounter==255){
            ram->write(DIV,ram->read(DIV)+1);	//increment contents at 0xFF04
            DIVCounter=0;
        }
        else{
            DIVCounter++;
        }

        _TIMA=ram->read(TIMA);
        if(TIMACounter==((CLOCK_SPEED/TIMASpeed)-1)){
            _TIMA++;
            if(_TIMA>0xFF){
                //TODO: add interrupt handeling here   
                _TIMA=TIMAStart;
            }
            ram->write(TIMA,(uint8_t)_TIMA);
            TIMACounter=0;
        }
        else{
            TIMACounter++;
        }
    }
}
