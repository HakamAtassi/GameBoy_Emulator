#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#include "GameBoy.h"
#include "string"
using namespace std;

class Debugger
{
private:
    GameBoy * gameboy;

    uint16_t PC;
    uint16_t prevPC;
    uint8_t opcode;
    uint8_t instruction;
    bool CBFlag=false;


    std::vector<string> opcodeLUT;
    std::vector<string> opcodeLUTCB;
    std::string testResult="";

    void printRegs();
    void printPC();
    void printInterrupts();
    void printInstruction();
    void printMemory();
    void printStack();
    void printResult();
    void printNextWord();
    void printNextWordSigned();
    void checkSerialOut();
    void printFlags();

    void debug();



    void printState();  //prints cpu state

public:
    Debugger(GameBoy * gameboy);
    ~Debugger();


    void run();


};


#endif DEBUGGER_H_
