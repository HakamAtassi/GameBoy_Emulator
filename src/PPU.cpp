#include "headers/PPU.h"
#include <fstream>
#include <vector>
#include "SDL2/SDL.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

#define LCDC_ADDR 0xFF40
#define STAT_ADDR 0xFF41
#define SCY_ADDR 0xFF42
#define SCX_ADDR 0xFF43
#define LY_ADDR 0xFF44
#define LYC_ADDR 0xFF45
#define WY_ADDR 0xFF4A
#define WX_ADDR 0xFF4B

#define OAM_ADDR 0xFE00

#define INTERRUPT_ENABLE 0xFFFF
#define INTERRUPT_FLAGS 0xFF0F

#define PIXELSIZE 1;

PPU::PPU(){
	ram=new RAM;
}

PPU::PPU(RAM * _ram){
	ram=_ram;

}



PPU::PPU(Cartridge _cartridge){
	ram=new RAM;

	/*allocate vectors*/

	cartridge=_cartridge;
	for(int i=0;i<0xFFFF;i++){
		ram->write(i,_cartridge.read(i));
	}

	ram->write(0xFF44,0);
}

PPU::~PPU(){
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void PPU::dumpVram(){
	for(int i=0x8000;i<0xA000;i++){
		printf("%X: %X\n",i, ram->read(i));
	}
}
void  PPU::dumpOAM(){
	for(int i=0xFE00;i<0xFEA0;i++){
		printf("%X: %X\n",i, ram->read(i));
	}
}
void PPU::dumpPalettes(){
	for(int i=0xFF47;i<0xFF4A;i++){
		printf("%X: %X\n",i, ram->read(i));
	}
}
void PPU::dumpPixelbuffer(){
	for(auto elem:pixelBuffer){
		printf("%X\n",elem);
	}
}

void PPU::createWindow(){
	/*initilize sdl2 stuff*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
			return;
		}
		window = SDL_CreateWindow("GameBoy",
						SDL_WINDOWPOS_UNDEFINED,
						SDL_WINDOWPOS_UNDEFINED,
						WIDTH, HEIGHT,
						SDL_WINDOW_RESIZABLE);
		renderer = SDL_CreateRenderer(window, -1, 0);
		texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET,WIDTH,HEIGHT);
		SDL_SetRenderTarget(renderer, NULL);
}
void PPU::drawPixelBuffer(){	//simply draws the pixel buffer to the frame

	SDL_UpdateTexture
	(texture,
					  NULL,
					  pixelBuffer, 
					  WIDTH*3);

	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

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



void PPU::updateGraphics(int clocks){
	LCDC=ram->read(LCDC_ADDR);
	STAT=ram->read(STAT_ADDR);


	setSTAT();	//update status register


	// count down the LY register which is the current line being drawn. When reaches 144 (0x90) its vertical blank time
	if (LCDDisplayEnable==1)
		scanlineClocks -= clocks ;

	if (ram->read(LY_ADDR) > 0x99){
		ram->write(LY_ADDR,0);
	}
	//else if (m_Rom[0xFF44] == 0)
	//	ResetScreen( ) ;

	if (scanlineClocks <= 0){
		drawCurrentLine();
	}

}

void PPU::requestInterrupt(int bit){
	uint8_t interruptFlags=ram->read(INTERRUPT_FLAGS);
	interruptFlags|=1<<bit;
	ram->write(INTERRUPT_FLAGS, interruptFlags);
}


void PPU::drawCurrentLine(){
	if (LCDDisplayEnable==false){
		return;
	}

	uint8_t LY=ram->read(0xFF44);
	LY++;
	ram->write(LY_ADDR, LY);

	scanlineClocks = 456 ;


	if ( LY == 0x90)
		issueVerticalBlank() ;

	if (LY > 0x99)
		LY=0;
		ram->write(LY_ADDR,LY);

	if (LY < 0x90)
	{
		drawScanline();
	}
}

static int vblankcount = 0 ;

void PPU::issueVerticalBlank(){
	vblankcount++ ;
	//requestInterrupt(0);
}



PPU::Color PPU::getColor(uint8_t colorNum, uint16_t address) const{
	Color result=White;
	uint8_t palette=ram->read(address);
	int hi=0;
	int lo=0;

	switch (colorNum)
	{
		case 0: hi=1;lo=0;break;
		case 1: hi=3;lo=2;break;
		case 2: hi=5;lo=4;break;
		case 3: hi=7;lo=6;break;
	}
	int color=0;
	color=((palette&(1<<hi))>0)<<1;
	color|=((palette&(1<<lo))>0);

	switch (color)
	{
		case 0: result = White ;break ;
		case 1: result = LightGray ;break ;
		case 2: result = DarkGray ;break ;
		case 3: result = Black ;break ;
	}
	return result;
}

void PPU::drawScanline(){
	LCDC=ram->read(LCDC_ADDR);
	STAT=ram->read(STAT_ADDR);

	if(LCDDisplayEnable==1){
		renderTiles();
		renderSprites();
	}

}

void PPU::renderTiles(){
	if (BGWindowEnable==1){
		uint16_t tileData = 0 ;
		uint16_t backgroundMemory =0 ;
		bool unsig = true ;

		uint8_t scrollY = ram->read(0xFF42) ;
		uint8_t scrollX = ram->read(0xFF43) ;
		uint8_t windowY = ram->read(0xFF4A) ;
		uint8_t windowX = ram->read(0xFF4B) - 7;

		bool usingWindow = false ;

		if (WindowDisplayEnable)
		{
			if (windowY <= ram->read(0xFF44))
				usingWindow = true ;
		}
		else
		{
			usingWindow = false ;
		}

		// which tile data are we using?
		if (TileDataSelect)
		{
			tileData = 0x8000 ;
		}
		else
		{
			tileData = 0x8800 ;
			unsig= false ;
		}

		// which background mem?
		if (false == usingWindow)
		{
			if (BGTileMapSelect)
				backgroundMemory = 0x9C00 ;
			else
				backgroundMemory = 0x9800 ;
		}
		else
		{
			if (WindowTileMapSelect)
				backgroundMemory = 0x9C00 ;
			else
				backgroundMemory = 0x9800 ;
		}


		uint8_t yPos = 0 ;

		if (!usingWindow)
			yPos = scrollY + ram->read(0xFF44) ;
		else
			yPos = ram->read(0xFF44) - windowY;

		uint16_t tileRow = (((uint8_t)(yPos/8))*32) ;

		int index=160*3*((int)(ram->read(0xFF44)));

		for (int pixel = 0 ; pixel < 160; pixel++)
		{
			uint8_t xPos = pixel+scrollX ;

			if (usingWindow)
			{
				if (pixel >= windowX)
				{
					xPos = pixel - windowX ;
				}
			}

			uint16_t tileCol = (xPos/8) ;
			int16_t tileNum ;

			if(unsig)
				tileNum = (uint8_t)ram->read(backgroundMemory+tileRow + tileCol) ;
			else
				tileNum = (int8_t)ram->read(backgroundMemory+tileRow + tileCol) ;

			uint16_t tileLocation = tileData ;

			if (unsig)
				tileLocation += (tileNum * 16) ;
			else
				tileLocation += ((tileNum+128) *16) ;

			uint8_t line = yPos % 8 ;
			line *= 2;
			uint8_t data1 = ram->read(tileLocation + line) ;
			uint8_t data2 = ram->read(tileLocation + line + 1) ;

			int colorBit = xPos % 8 ;
			colorBit -= 7 ;
			colorBit *= -1 ;

			int colorNum = ((data2&(1<<colorBit))>0) ;
			colorNum <<= 1;
			colorNum |= ((data1&(1<<colorBit))>0) ;

			Color col = getColor(colorNum, 0xFF47) ;
			int red = 0;
			int green = 0;
			int blue = 0;

			switch(col)
			{
				case White:	red = 255; green = 255 ; blue = 255; break ;
				case LightGray:red = 0xCC; green = 0xCC ; blue = 0xCC; break ;
				case DarkGray:	red = 0x77; green = 0x77 ; blue = 0x77; break ;
				case Black:	red = 0; green = 0 ; blue = 0; break ;

			}

			int finaly = ram->read(0xFF44) ;


			pixelBuffer[finaly*160*3+pixel*3+0] = red ;
			index++;
			pixelBuffer[finaly*160*3+pixel*3+3] = green;
			index++;
			pixelBuffer[finaly*160*3+pixel*3+2] = blue;
			index++;
		}
	}

}

void PPU::renderSprites(){
// lets draw the sprites (however it does need to be enabled)

	if (SpriteEnable)
	{
		bool use8x16 = false ;
		if (SpriteSize)
			use8x16 = true ;

		for (int sprite = 0 ; sprite < 40; sprite++)
		{
 			uint8_t index = sprite*4 ;
 			uint8_t yPos = ram->read(0xFE00+index) - 16;
 			uint8_t xPos = ram->read(0xFE00+index+1)-8;
 			uint8_t tileLocation = ram->read(0xFE00+index+2) ;
 			Attributes = ram->read(0xFE00+index+3) ;

			bool yFlip = YFlip ;
			bool xFlip = XFlip ;

			int scanline = ram->read(0xFF44);

			int ysize = 8;

			if (use8x16)
				ysize = 16;

 			if ((scanline >= yPos) && (scanline < (yPos+ysize)))
 			{
 				int line = scanline - yPos ;

 				if (yFlip)
 				{
 					line -= ysize ;
 					line *= -1 ;
 				}

 				line *= 2;
 				uint8_t data1 = ram->read( (0x8000 + (tileLocation * 16)) + line ) ;
 				uint8_t data2 = ram->read( (0x8000 + (tileLocation * 16)) + line+1 ) ;



 				for (int tilePixel = 7; tilePixel >= 0; tilePixel--)
 				{
					int colorBit = tilePixel ;
 					if (xFlip)
 					{
 						colorBit -= 7 ;
 						colorBit *= -1 ;
 					}
					int colorNum = ((data2&(1<<colorBit))>0) ;
					colorNum <<= 1;
					colorNum |= ((data1&(1<<colorBit))>0) ;

					Color col = getColor(colorNum, PaletteNumber?0xFF49:0xFF48) ;

 					// white is transparent for sprites.
 					if (col == White)
 						continue ;

 					int red = 0;
 					int green = 0;
 					int blue = 0;

					switch(col)
					{
					case White:	red = 255; green = 255 ; blue = 255; break ;
					case LightGray:red = 0xCC; green = 0xCC ; blue = 0xCC; break ;
					case DarkGray:	red = 0x77; green = 0x77 ; blue = 0x77; break ;
					}

 					int xPix = 0 - tilePixel ;
 					xPix += 7 ;

					int pixel = xPos+xPix ;

					if ((scanline < 0) || (scanline > 143) || (pixel < 0) || (pixel > 159))
					{
					//	assert(false) ;
						continue ;
					}

					// check if pixel is hidden behind background
					if (Priority == 1)
					{
						if ( (pixelBuffer[scanline*160*3+pixel*3 + 0] != 255) || (pixelBuffer[scanline*160*3+pixel*3 + 1] != 255) || (pixelBuffer[scanline*160*3+pixel*3 + 2] != 255) )
							continue ;
					}

 					pixelBuffer[scanline*160*3+pixel*3 + 0] = red ;
 					pixelBuffer[scanline*160*3+pixel*3 + 1] = green ;
 					pixelBuffer[scanline*160*3+pixel*3 + 2] = blue ;

 				}
 			}
		}
	}

}


void PPU::setSTAT(){

	STAT=ram->read(0xFF41);

	if (LCDDisplayEnable== false)
	{
		scanlineClocks = 456 ;
		ram->write(LY_ADDR,0);

		PPUMode=1;
		ram->write(0xFF41,STAT) ;
		return ;
	}


	uint8_t LY = ram->read(0xFF44) ;

	uint8_t currentMode = PPUMode;

	int mode = 0 ;
	bool reqInt = false ;

	// set mode as vertical blank
	if (LY >= 0x90)
	{
		// mode 1
		mode = 1 ;
		PPUMode=1;
		reqInt = Mode1Interrupt;
	}
	else
	{
		int mode2Bounds = (456 - 80) ;
		int mode3Bounds = (mode2Bounds - 172) ;


		// mode 2
		if (scanlineClocks >= mode2Bounds)
		{
			mode=2;
			PPUMode=2;
			reqInt=Mode2Interrupt;
		}
		// mode 3
		else if (scanlineClocks >= mode3Bounds)
		{
			mode = 3 ;
			PPUMode=3;
		}
		// mode 3
		else
		{
			mode = 0 ;
			PPUMode=0;
			reqInt=Mode0Interrupt;
		}

	}

	// just entered a new mode. Request interupt
	if (reqInt && (currentMode != mode)){

	}

	// check for coincidence flag
	if ( LY == ram->read(0xFF45))
	{
		CoincidenceFlag = 1;

		if (LYLYC)
		{

		}
	}
	else
	{
		CoincidenceFlag = 0;
	}

	ram->write(0xFF41, STAT) ;

}




