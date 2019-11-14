#ifndef PROGETTOGRAFICA_GAME_H
#define PROGETTOGRAFICA_GAME_H

#include "Ship.h"
#include "Buoy.h"
#include "Enviroment.h"

class Game {
private:
    int game_start_time;
    int game_end_time;
    int score;
    int score_limit;
    Ship *ship;
    Enviroment *enviroment;
public:
    Game(Ship*, Enviroment*);
    int getGameTime();
    int getScore();
    int getScoreLimit();
    void detectCollision();
    bool isFinished();
};

#endif