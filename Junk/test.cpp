#include <cstdint>
#include <iostream>
#include "../src/CPU.h"

using namespace GameBoy;
using namespace std;
int main(){

	CPU cpu;

	cpu.read(0x00FF);
	cpu.write(0x00FF,0x80);
	cpu.read(0x00FF);


}
