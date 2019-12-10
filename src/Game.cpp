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

Game::Game(Ship *ship, Enviroment *enviroment) {
    game_start_time = 0;
    game_end_time = 0;
    score = 0;
    score_limit = enviroment->getBuoysCount();
    this->ship = ship;
    this->enviroment = enviroment;
}

int Game::getGameTime() {
    if (game_end_time != 0){
        return game_end_time;
    } else if (game_start_time != 0){
        return SDL_GetTicks() - game_start_time;
    } else {
        return 0;
    }
}

int Game::getScore() {
    return score;
}

int Game::getScoreLimit(){
    return score_limit;
}

void Game::detectCollision(){
    ShipMesh* shipMesh = ship->getMesh();
    Mesh* buoyMesh;
    for (int i = 0; i < enviroment->getBuoysCount() && !isFinished(); i++){
        buoyMesh = enviroment->getBuoy(i)->getMesh();
        if (enviroment->getBuoy(i)->isActive() &&
            BoundingBox::isCollision(shipMesh->w_bounding_box, buoyMesh->w_bounding_box)){
            enviroment->getBuoy(i)->disable();
            score++;
        }
    }
}

void Game::go(){
    if (game_start_time == 0){
        game_start_time = SDL_GetTicks();
    }
}

bool Game::isFinished(){
    bool isFinished = true;
    for (int i = 0; i < enviroment->getBuoysCount(); i++){
        if (enviroment->getBuoy(i)->isActive()){
            isFinished = false;
        }
    }
    if (isFinished){
        game_end_time = getGameTime();
    }
    return isFinished;
}