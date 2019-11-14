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
    game_start_time = SDL_GetTicks();
    game_end_time = 0;
    score = 0;
    score_limit = 10;
    this->ship = ship;
    this->enviroment = enviroment;
}

int Game::getGameTime() {
    if (game_end_time != 0){
        return game_end_time;
    } else {
        return SDL_GetTicks() - game_start_time;
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
    for (int i = 0; i < BUOYS_COUNT && !isFinished(); i++){
        buoyMesh = enviroment->getBuoy(i)->getMesh();
        /*printf("Center: %f %f, BB: %f %f, WBB: %f %f\n",
               ship->px, ship->pz,
               shipMesh->bbmin.X(), shipMesh->bbmin.Z(),
               shipMesh->w_bounding_box->vertex[0]->X(), shipMesh->w_bounding_box->vertex[0]->Z());*/
        //buoyMesh->w_bounding_box->display();
        //shipMesh->w_bounding_box->display();
        if (enviroment->getBuoy(i)->isActive() &&
            BoundingBox::isCollision(shipMesh->w_bounding_box, buoyMesh->w_bounding_box)){
            enviroment->getBuoy(i)->disable();
            score++;
        }
    }
}

bool Game::isFinished(){
    bool isFinished = true;
    for (int i = 0; i < BUOYS_COUNT; i++){
        if (enviroment->getBuoy(i)->isActive()){
            isFinished = false;
        }
    }
    if (isFinished){
        game_end_time = getGameTime();
    }
    return isFinished;
}