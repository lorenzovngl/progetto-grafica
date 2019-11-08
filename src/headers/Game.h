#ifndef PROGETTOGRAFICA_GAME_H
#define PROGETTOGRAFICA_GAME_H

class Game {
private:
    int game_start_time;
    int score;
    int score_limit;
public:
    Game();
    int getGameTime();
    int getScore();
    int getScoreLimit();
};

#endif