#ifndef TIMERS_H_
#define TIMERS_H_
#include "RAM.h"

class Timers{

    public:
        Timers(RAM * _ram);
        Timers();

        void updateTimers(int requiredClocks);
        
    private:
        RAM * ram;

        uint8_t DIV=0;
        int DIVCounter=213;

        uint8_t TIMA=0;
        int TIMACounter=0;

        uint8_t TMA=0;
        uint8_t TAC=0;
    
        bool timersEnabled;

        bool TIMAEnable=false;    //Bit 2 of 0xFF07
        int TIMASpeed=0;

        void updateDIV(int requiredClocks);
        void updateTIMA(int requiredClocks);
        void updateTMA();
        void updateTAC();

        void requestTimerInterrupt();
};


#endif