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

#define INTERRUPT_ENABLE 0xFFFF
#define INTERRUPT_FLAGS 0xFF0F

#define PIXELSIZE 1;

PPU::PPU(){
	ram=new RAM;
}

PPU::PPU(RAM * _ram){
	ram=_ram;
	ram->write(0xFF44,0);

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

void PPU::drawVram(){
	for(int i=0;i<144;i++){
		drawScanlineVram();
	}
}

void PPU::updateGraphics(int clocks){
	LCDC=ram->read(0xFF40);
	STAT=ram->read(0xFF41);
	//setSTAT();	//update status register

	if(LCDDisplayEnable==1){
		scanlineClocks-=clocks;
	}
	else{	//dont draw anything if ppu is disabled
		return;
	}
	int LY=ram->read(LY_ADDR);

	if(scanlineClocks<=0){	//each scanline take 456 cycles, always.
							//remember, each scanline is padded so above is true (h-blank)
		//LY++;
		//ram->write(LY_ADDR,LY);
		scanlineClocks=456;	//reset back to 456

		if(LY==144){
			//TODO: add this functionality to interrupt handler. 
			uint8_t interruptFlags=ram->read(INTERRUPT_FLAGS);
			interruptFlags|=1<<0;
			ram->write(INTERRUPT_FLAGS, interruptFlags);
		}
		else if(LY>153){
			ram->write(LY_ADDR,0);
		}
		else{
			drawScanline();
		}
	}
	printf("LY: %X\n",(int)ram->read(LY_ADDR));
}

void PPU::drawScanline(){

	renderTiles();
	renderSprites();
}

void PPU::renderTiles(){
	int scrollY=ram->read(0xFF42);
	int scrollX=ram->read(0xFF43);
	int windowY=ram->read(0xFF4A);
	int windowX=ram->read(0xFF4B)-7;
	bool unsig=true;

	//what tile map and tile data to use
	//both based on the values of ppu registers set by the game
	int tileDataBaseAddress=0;
	if(TileDataSelect==1){
		tileDataBaseAddress=0x8000;
	}
	else{
		tileDataBaseAddress=0x8800;
		unsig=false;
	}

	int tileMapBaseAddress=0;
	if(BGTileMapSelect==1){
		tileMapBaseAddress=0x9C00;
	}
	else{
		tileMapBaseAddress=0x9800;
	}

	//scrollY is the number of pixels offset from the top of the screen
	uint8_t LY=ram->read(LY_ADDR);
	uint8_t yPos=scrollY+LY;	//where do the rendered pixels go?
	//Y axis of the pixel currently being drawn
	//both in pixels, not tiles

	uint16_t tileRow=(((uint8_t)(yPos/8))*32);
	//what number tile is currently being rendered? include all tiles
	//remember, the background is actually 32x32


	//each scanline draws 160 pixels
	//20 tiles wide...
	for(int i=0;i<160;i++){
		uint8_t xPos=i+scrollX;
		uint16_t tileCol = (xPos/8);
		int16_t tileNum;

		uint16_t tileAddress=tileMapBaseAddress+tileRow+tileCol;
		if(unsig==true){
			tileNum=(uint8_t)ram->read(tileAddress);
		}
		else{
			tileNum=(int8_t)ram->read(tileAddress);

		}
		uint16_t tileLocation=tileDataBaseAddress;

		//get the final, absolute index of the tile that needs to be rendered in
		//depends on 8000 vs 8800 addressing
		if(unsig==true){
			tileLocation+=tileNum*16;
		}
		else{
			tileLocation+=((tileNum+128)*16);
		}

		//the relative line number of the window
		//y pos is the line number relative to the 32x32 window
		uint8_t line=yPos%8;
		line*=2;
		
		uint8_t byte0=ram->read(tileLocation+line);
		uint8_t byte1=ram->read(tileLocation+line+1);


		int colorBit = xPos % 8 ;
		colorBit -= 7 ;
		colorBit *= -1 ;

		int colorNum = ((byte1&(1<<colorBit))>0) ;
		colorNum <<= 1;
		colorNum |= ((byte0&(1<<colorBit))>0) ;

		int finaly = ram->read(0xFF44) ;


		pixelBuffer[pixelNumber] = colorNum*50 ;
		pixelNumber++;
		pixelBuffer[pixelNumber] = colorNum*50  ;
		pixelNumber++;
		pixelBuffer[pixelNumber] = colorNum*50  ;
		pixelNumber++;

	}
	ram->write(0xFF44,ram->read(0xFF44)+1);

}

void PPU::renderSprites(){

}


void PPU::setSTAT(){
	if(LCDDisplayEnable==false){	//PPU is disabled
		scanlineClocks=456;
		ram->write(LY_ADDR,0);	//reset scanline number to 0
		PPUMode=1;
		ram->write(STAT_ADDR,STAT);	//"save" STAT_ADDR
		return;
	}

	uint8_t currentLine=ram->read(LY_ADDR);	//current scanline number
	uint8_t prevMode=PPUMode;	//if the new mode is different than what it is right now
								//an interrupt *may* be requested
	
	bool requestInt=false;

	if(currentLine>=144)
	{
		//set PPU to mode 1 (V-Blank)
		PPUMode=1;
		requestInt=Mode0Interrupt;
	}
	else{
		//see PPU timing diagram on pandocs
		int mode2Bound=456-80;
		int mode3Bound=mode2Bound-172;

		if(scanlineClocks>=mode2Bound){
			PPUMode=2;
			requestInt=Mode2Interrupt;
		}
		else if(scanlineClocks>=mode3Bound){
			PPUMode=3;
		}
		else{
			PPUMode=0;
			requestInt=Mode0Interrupt;
		}
	}

	if(requestInt && (PPUMode!=prevMode)){	//request timer interrupt

		uint8_t interruptFlags=ram->read(INTERRUPT_FLAGS);
    	interruptFlags|=1<<1;
    	ram->write(INTERRUPT_FLAGS, interruptFlags);
	}

	if(ram->read(LY_ADDR)==ram->read(LYC_ADDR)){
		CoincidenceFlag=1;
	}
	else{
		CoincidenceFlag=0;
	}
	ram->write(STAT_ADDR,STAT);	//write new status back... "Save" STAT

}


void PPU::drawScanlineVram(){
	uint8_t LY=ram->read(0xFF44);
	uint16_t tileNumber=0x8000 + LY*2+0x100*(LY/8);	//the location of the tile whose first 8 pixels are being drawn in vram


	for(int i=0;i<20;i++){	//scanline is 160 pixels wide, which is 18 tiles
		printf("addr:%X,\n", tileNumber+i*16);

		uint8_t byte0=ram->read(tileNumber+i*16);
		uint8_t byte1=ram->read(tileNumber+i*16+1);
		//combine data, add coloring, store.
		//printf("Byte0: %X, Byte1: %X\n", byte0,byte1);

		uint16_t combinedData=combineTileBytes(byte0,byte1);
		//printf("combined bytes: %X\n", combinedData);
		//draw combined data to pixel buffer
		drawToPixelData(combinedData);
	}
	printf("============\n\n\n");

	LY++;
	ram->write(0xFF44,LY);

}

void PPU::drawToPixelData(uint16_t lineSegment){
	//printf("line segment: %X\n", lineSegment);
	uint8_t LY=ram->read(0xFF44);
	pixelNumber%=(WIDTH*HEIGHT*3);

	for(int i=0;i<8;i++){
		//printf("pixel %d: ",i);
		uint8_t pixel=0;
		pixel=((lineSegment&0x8000)>0);
		lineSegment=lineSegment<<1;
		//printf("%d",pixel);

		pixel|=((lineSegment&0x8000)>0);
		lineSegment=lineSegment<<1;
		//printf("%d \n",pixel);

		/*dont worry about coloring yet*/
		//printf("pixelBuffer index=%d\n", pixelNumber);

		pixelBuffer[pixelNumber]=pixel*80;
		pixelNumber++;	//red
		pixelBuffer[pixelNumber]=pixel*0;
		pixelNumber++;	//green
		pixelBuffer[pixelNumber]=pixel*0;
		pixelNumber++;	//blue
	}
	printf("8 pixels done\n\n");
}
