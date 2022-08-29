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
	drawScanline();

}


void PPU::updateStatusReg(){


}


void PPU::drawScanline(){
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

void PPU::renderTiles(){
	
}

