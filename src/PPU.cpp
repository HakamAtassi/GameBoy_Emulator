#include "headers/PPU.h"
#include <fstream>
#include <vector>
#include "SDL2/SDL.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>


#define PIXELSIZE 1;

PPU::PPU(){
	ram=new RAM;
}

PPU::PPU(RAM * ram):ram(ram){

}


PPU::PPU(Cartridge _cartridge){
	ram=new RAM;

	/*allocate vectors*/

	cartridge=_cartridge;
	for(int i=0;i<0xFFFF;i++){
		ram->write(i,_cartridge.read(i));
	}
	for (auto& it : pixelBuffer) {
		it=150;
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

void PPU::updateGraphics(){
	LCDC=ram->read(0xFF40);
	STAT=ram->read(0xFF41);
	drawScanline();
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

	uint8_t yPos=scrollY+ram->read(0xFF44);	//scrollY + LY regsiter
	uint16_t tileRow=(((uint8_t)(yPos/8))*32);

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

		if(unsig==true){
			tileLocation+=tileNum*16;
		}
		else{
			tileLocation+=((tileNum+128)*16);
		}

		uint8_t line=yPos%8;
		line*=2;
		
		uint8_t byte0=ram->read(tileLocation+line);
		uint8_t byte1=ram->read(tileLocation+line+1);

		int colourBit = xPos % 8 ;
		colourBit -= 7 ;
		colourBit *= -1 ;

		int colourNum = ((byte1&(1<<colourBit))>0) ;
			colourNum <<= 1;
			colourNum |= ((byte0&(1<<colourBit))>0) ;

		int finaly = ram->read(0xFF44) ;


		pixelBuffer[pixelNumber] = colourNum*50 ;
		pixelNumber++;
		pixelBuffer[pixelNumber] = colourNum*50  ;
		pixelNumber++;
		pixelBuffer[pixelNumber] = colourNum*50  ;
		pixelNumber++;

	}
	ram->write(0xFF44,ram->read(0xFF44)+1);

}

void PPU::renderSprites(){

}


void PPU::updateStatusReg(){
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
