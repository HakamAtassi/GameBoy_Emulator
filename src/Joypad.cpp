#include "headers/Joypad.h"

Joypad::Joypad(){};

Joypad::~Joypad(){};


Joypad::Joypad(RAM * ram):ram(ram){

}



void Joypad::getInput(){
    SDL_Event event;
	SDL_PollEvent(&event);

    //controls:
    //W up
    //A left
    //S back
    //D right
    //K A
    //L B

	switch(event.type){
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){

				case SDLK_w:
					printf("w pressed\n");
					return;
					break;


				case SDLK_a:
					printf("a pressed\n");
					return;
					break;

				case SDLK_s:
					printf("s pressed\n");
					return;
					break;

				case SDLK_d:
					printf("d pressed\n");
					return;
					break;

				
				case SDLK_k:
					printf("k pressed\n");
					return;
					break;

				case SDLK_l:
					printf("l pressed\n");
					return;
					break;

				default:
					break;
			}

		case SDL_KEYUP:
			switch (event.key.keysym.sym){

				
				case SDLK_w:
					printf("w depressed\n");
					return;
					break;


				case SDLK_a:
					printf("a depressed\n");
					return;
					break;

				case SDLK_s:
					printf("s depressed\n");
					return;
					break;

				case SDLK_d:
					printf("d depressed\n");
					return;
					break;

				
				case SDLK_k:
					printf("k depressed\n");
					return;
					break;

				case SDLK_l:
					printf("l depressed\n");
					return;
					break;

				default:
					break;
			}
		}
}