#ifndef PROGETTOGRAFICA_HUD_H
#define PROGETTOGRAFICA_HUD_H

#include <SDL2/SDL_ttf.h>
#include "Game.h"
#include "Leaderboard.h"
#include "../../lib/GLText.h"

#define TEXTURE_TEXT 20

class HUD {
private:
    GLText *mText18;
    GLText *mText25;
    GLText *mText40;
    GLText *mText100;
    Game *game;

    void displayLeaderboardColumn(int col, Leaderboard::LBItem **items, int v_width, int v_height);

public:
    bool isCommandsListVisibile;
    bool isLeaderboardVisible;
    bool bAskUsername;
    bool bAskBuoys;

    HUD(Game *);

    void display(int v_width, int v_height, float ship_cx, float ship_cy, float ship_facing, Enviroment *enviroment,
                 float fps);

    void askUsername(int v_width, int v_height, char *input);

    void displayStartGameMessage(int v_idth, int v_height);

    void displayEndGameMessage(int v_width, int v_height);

    void displayPauseMessage(int v_width, int v_height);

    void displayCommands(int v_width, int v_height);

    void displayLeaderboard(Leaderboard *leaderboard, int v_width, int v_height);

    void toggleCommandsList();

    void toggleLeaderboard();

    void askNumberOfBuoys(int v_width, int v_height, char *input);
};

#endif