#ifndef TIMERS_H_
#define TIMERS_H_
#include "headers/RAM.h"

class Timers{

    public:
        Timers(RAM * _ram);
        Timers();

        void updateTimers(int requiredClocks);

    private:
        RAM * ram;
        int DIVCounter=0;   //keep track of clocks for updating DIV counter
        int TIMACounter=0;  //keep track of clocks for updating TIMA counter
        int TIMAStart=0;  

        uint16_t _TIMA=0;   //the value of TIMA is going to be stored here then incremented
                            //this makes it easy to spot overflows
                            //slighty hacky, but works very well...

        bool TIMAEnable=false;    //Bit 2 of 0xFF07
        uint8_t TIMASpeed=0;

        void updateTAC();
        void updateTMA();
};


#endif