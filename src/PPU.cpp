#include "headers/PPU.h"
#include <cassert>

#define LCDControlRegister_Address 0xFF40
#define LCDStatusRegister_Address 0xFF41

#define SCY_Address 0xFF42	//0xFF42. Scroll Y (Viewport)
#define SCX_Address 0xFF43	//0xFF43. Scroll X (viewport)
#define LY_Address 0xFF44	//0xFF44 LCD Y coord (current scanline)
#define LYC_Address 0xFF45	//0xFF45 LY compare
#define WY_Address 0xFF4A	//0xFF4A. Window Y. 
#define WX_Address 0xFF4B	//0xFF4B. Window X. 



PPU::PPU(){
    ram=new RAM;
	LCDControlRegister=0;
	LCDStatusRegister=0x80;	//unused always 1
};

PPU::PPU(RAM * ram):ram(ram){
	LCDControlRegister=0;
	LCDStatusRegister=0x80;	//unused always 1

}

void PPU::requestInterrupt(int interruptNum){
	

}

void PPU::updateStatusRegister(){
	//read register, update contents, write to register.

	LCDStatusRegister=ram->read(LCDStatusRegister_Address); 
	LCDControlRegister=ram->read(LCDControlRegister_Address);

	uint8_t prevMode=PPUMode;	//if PPUMode changes, an interrupt *may* take place

	bool reqInt=false;	//should an interrupt be requested?
	int LY=ram->read(LY_Address);
	int LYC=ram->read(LYC_Address);

	//ppu goes mode 2, mode 3, mode 0, then mode 1. 
	//when disabled, mode must be mode 1
	if(LCDDisplayEnable==0){
		//if PPU is disabled, set into mode 1, reset LY, and return
		scanlineCounter=456;	
		ram->write(LY_Address,0);
		PPUMode=1;
		return;
	}

	if(LY>=144){
		PPUMode=1;
		reqInt=Mode1StatInterrupt;
	}
	else{
		int mode2Boundry=456-80;	//mode 2 always takes 80 clocks. clock counter starts at 456
		int mode3Boundry=mode2Boundry-172;	//mode 2 takes at least 172 clocks and occurs after mode 2

		if(scanlineCounter>=mode2Boundry){	//still in mode2
			PPUMode=2;
			reqInt=Mode2StatInterrupt;
		}
		else if(scanlineCounter>=mode3Boundry){	//in mode 3
			PPUMode=3;
			//mode 3 has no interrupt
		}
		else{	//mode 0 (h blank)
			PPUMode=0;
			reqInt=Mode0StatInterrupt;
		}
	}

	if(reqInt && ((int)prevMode!=(int)PPUMode)){	//mode has changed and you want this to trigger a stat interrupt
		requestInterrupt(1);
	}

	CoincidenceFlag=(LY==LYC);
	ram->write(LCDStatusRegister_Address,LCDStatusRegister);	//write back any changes to LCDStatusRegister

}



PPU::COLOUR PPU::getColor(uint8_t colourNum, uint16_t address){

	COLOUR res = WHITE;
	uint8_t palette = ram->read(address);
	int hi = 0;
	int lo = 0;

	switch (colourNum)
	{
		case 0: hi = 1 ; lo = 0 ;break;
		case 1: hi = 3 ; lo = 2 ;break;
		case 2: hi = 5 ; lo = 4 ;break;
		case 3: hi = 7 ; lo = 6 ;break;
		default: assert(false) ; break;
	}

	int colour = 0;
	colour = BitGetVal(palette, hi) << 1;
	colour |= BitGetVal(palette, lo) ;

	switch (colour)
	{
		case 0: res = WHITE ;break ;
		case 1: res = LIGHT_GRAY ;break ;
		case 2: res = DARK_GRAY ;break ;
		case 3: res = BLACK ;break ;
		default: assert(false) ; break ;
	}

	return res ;
}



void PPU::renderBG(){

	LCDControlRegister=ram->read(LCDControlRegister_Address);
	uint8_t LY=ram->read(LY_Address);

	uint16_t tileData=0;			//the data for the actual tile
	uint16_t backgroundMap=0;	//where to get tile data from 
	bool unsigned_addressing=true;	//use for 8000 vs 8800 addressing 

   // where to draw the visual area and the window

	//these are the locations of the viewport and window
	//ie, where the start rendering
	//0,0 means start rendering from top left corner. 
	uint8_t scrollY = ram->read(SCY_Address) ;
	uint8_t scrollX = ram->read(SCX_Address) ;
	uint8_t windowY = ram->read(WY_Address) ;
	uint8_t windowX = ram->read(WX_Address) - 7; 

	bool usingWindow=false;

	
	if(WindowDisplayEnable==1){
		//LY is what is cu		void getColor(uint8_t colourNum, uint16_t address);
		//if windowY is less than or equal to what is currently being rendered
		//start adding it to the frame. 
		//otherwise, its too early to add frame.
		if(windowY<=LY){
			usingWindow=1;
		}
	}

	//where is tile data the actual data, not location
	if(TileDataSelect==1){
		tileData=0x8000;
	}
	else{
		tileData=0x8800;
		unsigned_addressing=false;	//0x8800 uses signed addressing from 0x8800
	}

	if(usingWindow==false){	//if window is disabled, just draw background.
		if(BGTileMapSelect==1){
			backgroundMap=0x9C00;
		}
		else{
			backgroundMap=0x9800;
		}
	}
	else{	//if window is on, it overlaps background, no matter what (healthbar, etc...)
		if(WindowTileMapSelect==1){
			backgroundMap=0x9C00;
		}
		else{
			backgroundMap=0x9800;
		}
	}


	uint8_t yPos=0;	//the y axis of the tile is on [0,31]

	if(usingWindow==false){
		yPos=scrollY+LY;
		//LY is number of current scanline.
		//scrollY is where the viewport starts
		//add to get total yPos
	}
	else{
		yPos=LY-windowY;
	}

	//current row in the actual 8x8 tile
	uint16_t tileRow=(((uint8_t)(yPos/8))*32);

	/*actual drawing of the tile*/
	for (int pixel = 0 ; pixel < 160; pixel++){	//view port is 160 pixels wide
		uint8_t xPos = pixel+scrollX ;
		// translate the current x pos to window space if necessary
		if (usingWindow==1){
			if (pixel >= windowX){
					xPos = pixel - windowX ;
			}
		}

		uint16_t tileCol = (xPos/8);
		int16_t tileNum;


		uint16_t tileAddress = backgroundMap+tileRow+tileCol;
		if(unsigned_addressing){
			//8000 vs 8800 addressing mode based on lcd control register
			tileNum =(uint8_t)ram->read(tileAddress);
		}
		else{
			tileNum =(int8_t)ram->read(tileAddress);
		}


		/*base tile location. can be either 0x8000 or 0x8800*/
		uint16_t tileLocation = tileData;

		//8000 vs 8800 algo
		if (unsigned_addressing){
			tileLocation += (tileNum * 16);
		}
		else{
			tileLocation += ((tileNum+128) *16);
		}

		// find the correct vertical line we're on of the
		// tile to get the tile data
		//from in memory
		uint8_t line = yPos % 8 ;
		line *= 2; // each vertical line takes up two bytes of memory
		uint8_t data1 = ram->read(tileLocation + line) ;
		uint8_t data2 = ram->read(tileLocation + line + 1) ;

		// pixel 0 in the tile is it 7 of data 1 and data2.
		// Pixel 1 is bit 6 etc..
		int colourBit = xPos % 8 ;
		colourBit -= 7 ;
		colourBit *= -1 ;

		// combine data 2 and data 1 to get the colour id for this pixel
		// in the tile
		int colourNum = BitGetVal(data2,colourBit) ;
		colourNum <<= 1;
		colourNum |= BitGetVal(data1,colourBit) ;

		// now we have the colour id get the actual
		// colour from palette 0xFF47
		COLOUR col = getColor(colourNum, 0xFF47) ;
		int red = 0;
		int green = 0;
		int blue = 0;

		// setup the RGB values
		switch(col)
		{
			case WHITE: red = 255; green = 255 ; blue = 255; break ;
			case LIGHT_GRAY:red = 0xCC; green = 0xCC ; blue = 0xCC; break ;
			case DARK_GRAY: red = 0x77; green = 0x77 ; blue = 0x77; break ;
		}

		int finaly = ram->read(0xFF44) ;

		// safety check to make sure what im about
		// to set is in the 160x144 bounds
		if ((finaly<0)||(finaly>143)||(pixel<0)||(pixel>159)){
			continue;
		}

			screenData[pixel][finaly][0] = red ;
			screenData[pixel][finaly][1] = green ;
			screenData[pixel][finaly][2] = blue ;
		}

}
void PPU::renderSprite(){
	//TODO: 
}

void PPU::updateControlRegister(){

}

void PPU::drawScanline(){
	LCDStatusRegister=ram->read(LCDStatusRegister_Address);
	LCDControlRegister=ram->read(LCDControlRegister_Address);

	if(BGWindowEnable==1){
		renderBG();
	}
	if(SpriteEnable==1){
		renderSprite();
	}

}

//draws scan lines from 0 to 144.
//144 to 153 is V-Blank (draws invisible scanlines). See Timing diagram of pandocs
//A scanline takes 456 cycles to complete. 
//only draw a scanline once the counter is <=0
void PPU::updateGraphics(int requiredClocks){
	int LY=ram->read(LY_Address);
	LCDStatusRegister=ram->read(LCDStatusRegister_Address);
	LCDControlRegister=ram->read(LCDControlRegister_Address);


	LCDStatusRegister=ram->read(LCDStatusRegister_Address); 
	if(LCDDisplayEnable==1){
		scanlineCounter-=requiredClocks;
	}
	else{	//PPU disabled
		return;
	}

	if(scanlineCounter<=0){
		LY++;
		ram->write(LY_Address, LY);
		scanlineCounter=456;	//reset counter for next scanline
		
		if(LY==144){
			//request V-Blank interrupt
			requestInterrupt(0);
		}
		else if(LY>153){
			LY=0;
			ram->write(LY_Address, LY);
		}

		//draw visible scanline
		else if(LY<144){
			drawScanline();
		}
		//if scanline is invisible, do nothing (just accept clocks for timing purposes)
		//no need to actually draw an invisible scanline...
	}

};

uint16_t PPU::combineTileBytes(uint8_t left, uint8_t right){
    uint16_t result=0;
    int resultPos=15;
    while((left|right)!=0){ //stop when both are 0
        bool rightMSB=((right&0x80)>0);
        bool leftMSB=((left&0x80)>0);
        result|=rightMSB<<resultPos;    //put right MSB in correct spot;
        resultPos--;    //left msb to the right of rightMSB
        result|=leftMSB<<resultPos;    //put right MSB in correct spot;
        resultPos--;
        right<<=1;
        left<<=1;
    }
    return result;
}

