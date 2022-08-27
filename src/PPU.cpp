#include "headers/PPU.h"


//TODO: there has to be a better way...
#define SCY 0xFF42	//0xFF42. Scroll Y (Viewport)
#define SCX 0xFF43	//0xFF43. Scroll X (viewport)
#define LY 0xFF44	//0xFF44 LCD Y coord (current scanline)
#define LYC 0xFF45	//0xFF45 LY compare
#define WY 0xFF4A	//0xFF4A. Window Y. 
#define WX 0xFF4B	//0xFF4B. Window X. 



PPU::PPU(){
    ram=new RAM;
	LCDControlRegister=0;
	LCDStatusRegister=0;
};

PPU::PPU(RAM * ram):ram(ram){
	LCDControlRegister=0;
	LCDStatusRegister=0;

}

void PPU::requestInterrupt(int interruptNum){
	

}

//draws scan lines from 0 to 144.
//144 to 153 is V-Blank (draws invisible scanlines). See Timing diagram of pandocs
//A scanline takes 456 cycles to complete. 
//only draw a scanline once the counter is <=0
void PPU::updateGraphics(int requiredClocks){
	//TODO: implement updateRegs();
	updateRegs();	//update all registers
	//TODO: set status maybe?
	if(LCDDisplayEnable==1){
		scanlineCounter--;
	}
	else{	//PPU disabled
		return;
	}

	if(scanlineCounter<=0){
		ram->write(LY, ram->read(LY)+1);
		scanlineCounter=456;	//reset counter for next scanline
		
		if(LY==144){
			//request V-Blank interrupt
			requestInterrupt(0);
		}
		else if(LY>153){
			ram->write(LY, 0);
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

