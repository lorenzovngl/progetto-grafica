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
    void display();
};

#endif