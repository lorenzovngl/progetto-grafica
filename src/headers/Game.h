#ifndef PROGETTOGRAFICA_GAME_H
#define PROGETTOGRAFICA_GAME_H

#include "Ship.h"

class Game {
private:
    int game_start_time;
    int score;
    int score_limit;
    Ship ship;
public:
    Game();
    int getGameTime();
    int getScore();
    int getScoreLimit();
};

#endif