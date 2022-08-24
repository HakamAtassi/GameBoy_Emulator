#ifndef INTERRUPTHANDLER_H_
#define INTERRUPTHANDLER_H_

#include "CPU.h"
#include "RAM.h"

class InterruptHandler
{
private:

    CPU * cpu;
    RAM * ram=nullptr;
    bool * IME=nullptr;

    void VBlankInterrupt();
    void STATInterrupt();
    void TimerInterrupt();
    void SerialInterrupt();
    void JoypadInterrupt();

    void ISR(int interruptVal);


    void pushWordToStack(uint16_t data);

    template<typename T> //such that it works with 8 and 16 bit uint
    bool testBit(T data,int bitPos){
        return (data&(1<<bitPos))>0;
    }

public:
    InterruptHandler();
    InterruptHandler(CPU * cpu, RAM * ram, bool * IME);
    ~InterruptHandler();

    void handleInterrupts();

};






#endif