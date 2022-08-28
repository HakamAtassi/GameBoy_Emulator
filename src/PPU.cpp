#include "headers/PPU.h"

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


void PPU::updateControlRegister(){



}

//draws scan lines from 0 to 144.
//144 to 153 is V-Blank (draws invisible scanlines). See Timing diagram of pandocs
//A scanline takes 456 cycles to complete. 
//only draw a scanline once the counter is <=0
void PPU::updateGraphics(int requiredClocks){
	int LY=ram->read(LY_Address);


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

