#include <cstdint>
#include <iostream>
#include "assert.h"
#include "../src/CPU.h"

using namespace GameBoy;
using namespace std;

int main(){ 

    CPU cpu;    //initialize CPU

    cpu.loadRom("../ROMS/blargg_cpu_instrs/cpu_instrs.gb");

    //start executing the rom

}
