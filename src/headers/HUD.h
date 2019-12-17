#ifndef PROGETTOGRAFICA_HUD_H
#define PROGETTOGRAFICA_HUD_H

#include <SDL2/SDL_ttf.h>
#include "Game.h"

#define TEXTURE_TEXT_TIME 20

class HUD {
private:
    TTF_Font *font;
    Game *game;
public:
    HUD(Game*);
    void displayTime();
    void display(int v_width, int v_height, float ship_cx, float ship_cy, float ship_facing, Enviroment* enviroment, float fps);
};

#endif