#include "headers/RAM.h"
#include <cstdint>
#include <string>

#define DIV 0xFF04


RAM::RAM(){
    memory=std::vector<uint8_t>(0xFFFF,0);   
}

uint8_t RAM::read(uint16_t PC){
    return memory[PC];
}

void RAM::write(uint16_t PC, uint8_t val){

    memory[PC]=val;
    return;
}

