#ifndef CARTRIDGE_H_
#define CARTRIDGE_H_

#include "RAM.h"
#include <fstream>
#include <cstdint>
#include <string> 
#include <vector>

class Cartridge
{
    private:


        std::vector<uint8_t> cartridgeData; //the data in the physical cartridge

        void write(uint16_t addr, uint8_t data);
        void loadRom(std::string rom);


    public: 
        Cartridge();
        Cartridge(std::string rom);
        uint8_t read(uint16_t addr);
        void printCartridge(int maxAddr);

	public:
		uint8_t checksum=0;
        uint8_t cartridgeType=0;	//defines the cartridges mapper
                                    //0x00: ROM only
                                    //0x01 MBC1...
        uint8_t romSize=0;			//"This byte indicates how much ROM is present on the cartridge."
                                    //rom size in KB (1024 bits)
        
        uint8_t ramSize=0;			//amount of additional ram on cartridge
        std::string Title;
        uint8_t CGBFlag=0;
        uint8_t SGBFlag=0;

};






#endif
