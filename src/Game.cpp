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
    score = 0;
    score_limit = 10;
    this->ship = ship;
    this->enviroment = enviroment;
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

bool isCollision(Point3 a_min, Point3 a_max, Point3 b_min, Point3 b_max) {
    return (a_min.X() <= b_max.X() && a_max.X() >= b_min.X()) &&
           (a_min.Y() <= b_max.Y() && a_max.Y() >= b_min.Y()) &&
           (a_min.Z() <= b_max.Z() && a_max.Z() >= b_min.Z());
}

void Game::detectCollision(){
    ShipMesh* shipMesh = ship->getMesh();
    Mesh* buoyMesh;
    for (int i = 0; i < 1; i++){
        buoyMesh = enviroment->getBuoy(i)->getMesh();
        printf("Ship: %f %f, Buoy: %f %f\n",
               shipMesh->w_bbmin->X(), shipMesh->w_bbmin->Z(),
               buoyMesh->w_bbmin->X(), buoyMesh->w_bbmin->Z());
        if (isCollision(*shipMesh->w_bbmin, *shipMesh->w_bbmin, *buoyMesh->w_bbmin, *buoyMesh->w_bbmax)){
            printf("Collision with %d\n", i);
        }
    }
}