#include "../../src/headers/Cartridge.h"
#include "../../src/headers/GameBoy.h"
#include <iostream>

#define SB 0xFF01
#define SC 0xFF02



int main(){

    Cartridge cartridge_01("../../ROMS/blargg_cpu_instrs/individual/01-special.gb");
    GameBoy gameboy_01(cartridge_01);
    printf("Starting test\n\n");

    for(int i=0;i<1000000;i++){
        gameboy_01.update();
        if(gameboy_01.read(SC)==0x81){
            printf("%C",gameboy_01.read(SB));
            gameboy_01.write(SC,0);
        }
    }

    printf("=============\n");

    Cartridge cartridge_02("../../ROMS/blargg_cpu_instrs/individual/02-interrupts.gb");
    GameBoy gameboy_02(cartridge_02);
    printf("Starting test\n\n");

    for(int i=0;i<1000000;i++){
        gameboy_02.update();
        if(gameboy_02.read(SC)==0x81){
            printf("%C",gameboy_02.read(SB));
            gameboy_02.write(SC,0);
        }
    }

        printf("=============\n");

    Cartridge cartridge_03("../../ROMS/blargg_cpu_instrs/individual/03-op sp, hl.gb");
    GameBoy gameboy_03(cartridge_03);
    printf("Starting test\n\n");

    for(int i=0;i<1000000;i++){
        gameboy_03.update();
        if(gameboy_03.read(SC)==0x81){
            printf("%C",gameboy_03.read(SB));
            gameboy_03.write(SC,0);
        }
    }

    printf("=============\n");

    Cartridge cartridge_04("../../ROMS/blargg_cpu_instrs/individual/04-op r,imm.gb");
    GameBoy gameboy_04(cartridge_04);
    //printf("Starting test\n\n");

    for(int i=0;i<1000000;i++){
        gameboy_04.update();
        if(gameboy_04.read(SC)==0x81){
            printf("%C",gameboy_04.read(SB));
            gameboy_04.write(SC,0);
        }
    }

    printf("=============\n");

    Cartridge cartridge_05("../../ROMS/blargg_cpu_instrs/individual/05-op rp.gb");
    GameBoy gameboy_05(cartridge_05);
    printf("Starting test\n\n");

    for(int i=0;i<1000000;i++){
        gameboy_05.update();
        if(gameboy_05.read(SC)==0x81){
            printf("%C",gameboy_05.read(SB));
            gameboy_05.write(SC,0);
        }
    }

    printf("=============\n");

    Cartridge cartridge_06("../../ROMS/blargg_cpu_instrs/individual/06-ld r, r.gb");
    GameBoy gameboy_06(cartridge_06);
    printf("Starting test\n\n");

    for(int i=0;i<1000000;i++){
        gameboy_06.update();
        if(gameboy_06.read(SC)==0x81){
            printf("%C",gameboy_06.read(SB));
            gameboy_06.write(SC,0);
        }
    }

        printf("=============\n");

    Cartridge cartridge_07("../../ROMS/blargg_cpu_instrs/individual/07-jr,jp,call,ret,rst.gb");
    GameBoy gameboy_07(cartridge_07);
    printf("Starting test\n\n");

    for(int i=0;i<10000000;i++){
        gameboy_07.update();
        if(gameboy_07.read(SC)==0x81){
            printf("%C",gameboy_07.read(SB));
            gameboy_07.write(SC,0);
        }
    }

    printf("=============\n");

    Cartridge cartridge_08("../../ROMS/blargg_cpu_instrs/individual/08-misc instrs.gb");
    GameBoy gameboy_08(cartridge_08);
    printf("Starting test\n\n");

    for(int i=0;i<1000000;i++){
        gameboy_08.update();
        if(gameboy_08.read(SC)==0x81){
            printf("%C",gameboy_08.read(SB));
            gameboy_08.write(SC,0);
        }
    }

    printf("=============\n");

    Cartridge cartridge_09("../../ROMS/blargg_cpu_instrs/individual/09-op r,r.gb");
    GameBoy gameboy_09(cartridge_09);
    printf("Starting test\n\n");

    for(int i=0;i<1000000;i++){
        gameboy_09.update();
        if(gameboy_09.read(SC)==0x81){
            printf("%C",gameboy_09.read(SB));
            gameboy_09.write(SC,0);
        }
    }
        printf("=============\n");

    Cartridge cartridge_10("../../ROMS/blargg_cpu_instrs/individual/10-bit ops.gb");
    GameBoy gameboy_10(cartridge_10);
    printf("Starting test\n\n");

    for(int i=0;i<1000000;i++){
        gameboy_10.update();
        if(gameboy_10.read(SC)==0x81){
            printf("%C",gameboy_10.read(SB));
            gameboy_10.write(SC,0);
        }
    }
        printf("=============\n");

    Cartridge cartridge_11("../../ROMS/blargg_cpu_instrs/individual/11-op a,(hl).gb");
    GameBoy gameboy_11(cartridge_11);

    bool flag=1;

    for(int i=0;i<1000000;i++){
        gameboy_11.update();
        if(gameboy_11.read(SC)==0x81){
            printf("%C",gameboy_11.read(SB));
            gameboy_11.write(SC,0);
        }
    }
}

//passed