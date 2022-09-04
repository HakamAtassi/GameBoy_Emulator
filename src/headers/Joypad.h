#ifndef JOYPAD_H_
#define JOYPAD_H_

#include "RAM.h"
#include "SDL2/SDL.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

class Joypad
{
private:

    RAM * ram;



public:
    Joypad();
    Joypad(RAM * ram);
    ~Joypad();
    void getInput();

};




#endif 