#ifndef PROGETTOGRAFICA_GAME_H
#define PROGETTOGRAFICA_GAME_H

#include "Ship.h"
#include "Buoy.h"
#include "Enviroment.h"
#include "Leaderboard.h"

class Game {
private:
    int mStartTime;
    int mEndTime;
    int mStartPauseTime;
    int mScore;
    int mScoreLimit;
    Ship *mShip;
    Enviroment *mEnviroment;
    Leaderboard *mLeaderboard;
    bool insertedInLeaderboard;
public:
    bool isPaused;

    Game(Ship *ship, Enviroment *enviroment);

    Leaderboard *getLeaderboard();

    int getGameTime();

    int getScore();

    int getScoreLimit();

    void detectCollision();

    void go();

    void reset();

    bool isFinished();

    void togglePause();

    void pause();

    void resume();
};

#endif