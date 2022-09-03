#ifndef PPU_H_
#define PPU_H_

#include "RAM.h"
#include "vector"
#include "RAM.h"
#include "Cartridge.h"
#include "InterruptHandler.h"

#include "SDL2/SDL.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#define WIDTH 160
#define HEIGHT 144

class PPU
{
private:
	
	RAM * ram;
	Cartridge cartridge;


	uint8_t pixelBuffer[WIDTH*HEIGHT*3];
	int pixelNumber=0;

	SDL_Window *window;
	SDL_Event event;
	SDL_Renderer *renderer;
	SDL_Texture *texture;

	int scanlineClocks=456;	//each scanline takes 456 cycles


	union{
		struct
		{
			uint8_t BGWindowEnable:1;		//Bit 0
			uint8_t SpriteEnable:1;			//Bit 1
			uint8_t SpriteSize:1;			//Bit 2
			uint8_t BGTileMapSelect:1;		//Bit 3
			uint8_t TileDataSelect:1;		//Bit 4
			uint8_t WindowDisplayEnable:1;	//Bit 5
			uint8_t WindowTileMapSelect:1;	//Bit 6
			uint8_t LCDDisplayEnable:1;		//Bit 7
		};
		uint8_t LCDC;
	};	//LCD Control, 0xFF40
	

	union{
		struct
		{
			uint8_t PPUMode:2;
			uint8_t CoincidenceFlag:1;
			uint8_t Mode0Interrupt:1;
			uint8_t Mode1Interrupt:1;
			uint8_t Mode2Interrupt:1;
			uint8_t LYLYC:1;
			uint8_t Unused:1;
		};
		uint8_t STAT;
	};	//LCD status, 0xFF41


	union{
		struct
		{
			uint8_t notUnused:4;			//Bit 0-3
			uint8_t PaletteNumber:1;	//Bit 4
			uint8_t XFlip:1;			//Bit 5
			uint8_t YFlip:1;			//Bit 6
			uint8_t Priority:1;			//Bit 7
		};
		uint8_t Attributes;
	};


public:
	PPU();	//default constructor
	PPU(RAM * _ram);	//for integration with system
	PPU(Cartridge cartridge);	//for integration with system

	~PPU();

	/*im going to implement the ppu with a varified correct PPU dump
	then integrate with the rest of the system...*/

	void dumpVram();
	void dumpOAM();
	void dumpPalettes();
	void dumpPixelbuffer();

	void createWindow();
	void drawPixelBuffer();

	void drawVram();

	void updateGraphics(int clocks);	//The actual update grpahics function



private:
	void setSTAT();
	void updateControlReg();
	//void requestInterrupt();
	void drawScanlineVram();
	void renderTiles();
	void renderSprites();

	void drawScanline();

	uint16_t combineTileBytes(uint8_t left, uint8_t right);
	void drawToPixelData(uint16_t lineSegment);

};





#endif