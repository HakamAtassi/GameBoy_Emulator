#ifndef PPU_H_
#define PPU_H_

#include "RAM.h"
#include "vector"
#include "RAM.h"
#include "Cartridge.h"

class PPU
{
private:
	
	RAM * ram;
	Cartridge cartridge;

public:
	PPU();	//default constructor
	PPU(RAM * ram);	//for integration with system
	PPU(Cartridge cartridge);	//for integration with system


	/*im going to implement the ppu with a varified correct PPU dump
	then integrate with the rest of the system...*/


	void dumpVram();
	void dumpOAM();
	void dumpPalettes();

};





#endif