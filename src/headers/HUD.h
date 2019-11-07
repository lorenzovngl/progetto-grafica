#ifndef PROGETTOGRAFICA_HUD_H
#define PROGETTOGRAFICA_HUD_H

#include <SDL2/SDL_ttf.h>

#define TEXTURE_TEXT_TIME 20

class HUD {
private:
    TTF_Font *font;
    int game_time;
public:
    HUD();
    void display(SDL_Renderer*);
};

#endif