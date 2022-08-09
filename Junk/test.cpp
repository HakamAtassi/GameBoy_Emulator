#include <cstdint>
#include <iostream>
#include "../src/Registers.h"

using namespace GameBoy;

int main(){

	Registers regs;


	regs.AF=0;
	regs.zero=1;

	printf("0x%X",regs.AF);

	

}
