#include "../../src/headers/PPU.h"
#include "iostream"

int main(){

    PPU ppu;

	ppu.PPUMode=1;
	
	std::cout<<"BGWindowEnable:"<<(int)ppu.LCDStatusRegister<<"\n";
}
