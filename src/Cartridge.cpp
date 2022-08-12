#include "headers/Cartridge.h"
#include <fstream>


void Cartridge::write(uint16_t addr, uint8_t data){
    cartridgeData[addr]=data;
}

uint8_t Cartridge::read(uint16_t addr){
    return cartridgeData[addr];
}

Cartridge::Cartridge(){};

void Cartridge::loadRom(std::string rom){
    std::ifstream file(rom, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		for (long i = 0; i < size; ++i)
		{
			write(i,buffer[i]);
		}
		delete[] buffer;
	}
}

Cartridge::Cartridge(std::string rom){
	cartridgeData=std::vector<uint8_t>(0xFFFF,0);
    loadRom(rom);

	cartridgeType=read(0x147);
	romSize=32 * (read(0x148)<<1);

	switch (read(0x149))	
	{
	case 0x00:
		//no ram on cartridge
		ramSize=0;
		break;
	case 0x01:
		//unused
		break;
	case 0x02:
		ramSize=8;
		break;
	case 0x03:
		ramSize=32;
		break;
	case 0x04:
		ramSize=128;
		break;
	case 0x05:
		ramSize=64;
		break;
	default:
		break;
	}

	for (uint16_t address = 0x0134; address <= 0x014C; address++) {
		checksum = checksum - read(address) - 1;
	}

	for (uint16_t address = 0x0134; address <= 0x0142; address++) {
		Title.push_back((char)read(address));
	}
	CGBFlag=read(0x143);
	SGBFlag=read(0x146);
}


void Cartridge::printCartridge(int maxAddr){
    for(int i=0;i<=maxAddr;i++){
        printf("%X: %X\n",i, read(i));
    }
}
