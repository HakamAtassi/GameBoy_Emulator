#include "headers/RAM.h"
#include <cstdint>
#include <string>
#include <iostream>

#define DIV 0xFF04


RAM::RAM(){
    memory=std::vector<uint8_t>(0xFFFF,0);   
}

uint8_t RAM::read(uint16_t PC){
    if(PC==0xFF00){
        return 0x0F;
    }
    return memory[PC];
}

void RAM::DMATransfer(uint8_t data){

    uint16_t address=data*100;
    for(int i=0;i<0xA0;i++){
        write(0xFE00+i,read(address+i));
    }
}

//run OAM DMA transfer if writing to 0xFF46
void RAM::write(uint16_t PC, uint8_t val){

    if(PC==0xFF00){
        memory[PC]=0x0F;
        return;
    }


    if(PC==0xFF46){     //writing to this address copies sprites from ROM to OAM
        DMATransfer(val);
        return;
    }



    memory[PC]=val;
    return;
    

}

