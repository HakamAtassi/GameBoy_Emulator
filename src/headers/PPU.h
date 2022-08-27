#ifndef PPU_H_
#define PPU_H_
#include "cstdint"
#include "RAM.h"
#include <cstdint>

#define VRAM_BLOCK1 0x8000
#define VRAM_BLOCK2 0x8800
#define VRAM_BLOCK3 0x9000


class PPU
{
    private:
        /*Video ram is between 0x8000 to 0x87FF*/
		/*each tile takes 16 bytes*/
		/*each tile has a color depth of 4*/
			/*gray scale. 4 possible "blacks"*/
		/*each tile is 8x8 and is drawn line by line*/
		/*which means that each line has 2 bytes corresponding to it*/

		//The actual rendering of tiles is slightly tricky to explain...
		//assume the two bytes for a given line are "0xE8 0x8F"
		//these bytes evaulate to 0b1110.1000 and 0b1000.1111
		//each bit maps directly to a pixel, from left to right. 
		//The two bits are combined as follows:
		//	11 01 01 00 11 10 10 01
		//	The result is the exact color each pixel should be once rendered. 
		//	Note that the right bit is the MSB for each pixel. first byte is LSB. 
		//	(remember, 4 possible colors for each pixel (gray scale))
		/*===========================================================================*/
		/*Total video memory is from 0x8000 to 0x9FFF*/
		/*Tile Data from 0x8000 to 0x97FF*/
		/*Tile Map 1 is from 0x9800 to 0x9BFF*/
		/*Tile Map 2 is from 0x9C00 to 0x9FFF*/

		RAM * ram;
		
	
		union{
			struct{
				uint8_t BGWindowEnable:1;		//BIT 0// 0 => neither background nor window are drawn
				uint8_t SpriteEnable:1;			//sprites only drawn to screen if set to 1
				uint8_t SpriteSize:1;			//if set to 1, sprites ate 1x2 TIle, otherwise, 1x2 tile
				uint8_t BGTileMapSelect:1;		//1=> user background map 0x9C00-0x9FFF. else, use 0x9800-0x9BFF
				uint8_t TileDataSelect:1;		//1=> use 8000 addressing mode, else 8800 mode
				uint8_t WindowDisplayEnable:1;	//0 hides window layer
				uint8_t WindowTileMapSelect:1;	//1=> use window map 0x9C00-0x9FFF. else, 0x9800-0x9BFF
				uint8_t LCDDisplayEnable:1;		//BIT 7//0 disables PPU completely 

			};
			uint8_t LCDControlRegister;
		};//0xFF40


		union{
			struct{
				uint8_t PPUMode:2;				//BIT 0//0=>H-Blank, 1=>V-Blank, 2=> OAM Scan, 3=> Drawing
				uint8_t CoincidenceFlag:1;		//LY==LYC
				uint8_t Mode0SttInterrupt:1;	//1 => enables mode 0 condition to trigger a stat interrupt
				uint8_t Mode1SttInterrupt:1;	//1 => enables mode 1 condition to trigger a stat interrupt
				uint8_t Mode2SttInterrupt:1;	//1 => enables mode 2 condition to trigger a stat interrupt
				uint8_t LYCLYStatInterrupt:1;	//LY==LYC triggers an interrupt
				uint8_t Unused:1;				//BIT 7//unused, always 0
	
			};
			uint8_t LCDStatusRegister;
		};//0xFF41
		



		int scanlineCounter=456;	//each scanline takes 456 cycles to complete. draw scanline 
									//once <=0; 

    public:
        PPU();
        PPU(RAM * ram);
        void updateGraphics(int requiredClocks);

	private:
		uint16_t combineTileBytes(uint8_t left, uint8_t right);	//combines the two bytes of vram to draw an 8 pixel line
		void updateRegs();	//update PPU regs
		void drawScanline();
		void requestInterrupt(int interruptNum);	//sets interrupt reg in memory. 
													//Note: interrupt handler is not needed to set interrupt
};




#endif
