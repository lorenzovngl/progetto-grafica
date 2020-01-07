#ifndef PROGETTOGRAFICA_HUD_H
#define PROGETTOGRAFICA_HUD_H

#include <SDL2/SDL_ttf.h>
#include "Game.h"

#define TEXTURE_TEXT 20

class HUD {
private:
    TTF_Font *mFont18;
    TTF_Font *mFont25;
    TTF_Font *mFont40;
    Game *game;
public:
    bool isCommandsListVisibile;

    HUD(Game *);

    void display(int v_width, int v_height, float ship_cx, float ship_cy, float ship_facing, Enviroment *enviroment,
                 float fps);

    void displayStartGameMessage(int v_width, int v_height);

    void displayEndGameMessage(int v_width, int v_height);

    void displayPauseMessage(int v_width, int v_height);

    void displayCommands(int v_width, int v_height);

    void toggleCommandsList();

    void askNumberOfBuoys(int v_width, int v_height, char *input);
};

#endif