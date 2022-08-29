#include "SDL2/SDL.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

#define WIDTH 160
#define HEIGHT 144

int main(){

    uint8_t pixelBuffer[WIDTH*HEIGHT*3]={};
	SDL_Window *window;
	SDL_Event event;
	SDL_Renderer *renderer;
	SDL_Texture *texture;

    window = SDL_CreateWindow("GameBoy",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    WIDTH, HEIGHT,SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, -1, 0);

    texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET,WIDTH,HEIGHT);
    SDL_SetRenderTarget(renderer, NULL);

//    SDL_RenderClear();
    SDL_UpdateTexture(texture,
					  NULL,
					  pixelBuffer, 
					  WIDTH*3);

	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_Event eventMain;

	while(1){
		SDL_PollEvent(&eventMain);
		if(eventMain.type == SDL_QUIT)
				break;
	}

}