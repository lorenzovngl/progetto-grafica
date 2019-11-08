#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector> // la classe vector di SDL

#endif

#include "headers/Game.h"

Game::Game() {
    game_start_time = SDL_GetTicks();
    score = 0;
    score_limit = 10;
}

int Game::getGameTime() {
    return SDL_GetTicks() - game_start_time;
}

int Game::getScore() {
    return score;
}

int Game::getScoreLimit(){
    return score_limit;
}