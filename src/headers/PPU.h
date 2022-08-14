#ifndef PPU_H_
#define PPU_H_
#include "cstdint"

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

    public:
        PPU();
        void updateGraphics(int requiredClocks);

	private:
		uint16_t combineTileBytes(uint8_t left, uint8_t right);	//combines the two bytes of vram
};


#endif
