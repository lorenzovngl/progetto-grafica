#ifndef PROGETTOGRAFICA_HUD_H
#define PROGETTOGRAFICA_HUD_H

#include <SDL2/SDL_ttf.h>

class HUD {
private:
    TTF_Font *font;
    int game_time;
public:
    HUD();
    void display();
};

#endif