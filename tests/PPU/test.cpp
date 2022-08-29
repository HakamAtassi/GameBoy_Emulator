#include "../../src/headers/Cartridge.h"
#include "../../src/headers/PPU.h"

#include <iostream>
#include <vector>

#define SB 0xFF01
#define SC 0xFF02

using namespace std;
int main(){

    /*the data in the dump is everything I need to generate a single frame 
    using the ppu*/
    Cartridge cartridge("04-op r,imm.dump");
    PPU ppu(cartridge);

    ppu.dumpVram();


}