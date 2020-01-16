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
    mStartTime = 0;
    mEndTime = 0;
    mScore = 0;
    mScoreLimit = enviroment->getBuoysCount();
    mShip = ship;
    mEnviroment = enviroment;
    mLeaderboard = new Leaderboard((char*) "Lorenzo");
    mLeaderboard->setNumBuoys(enviroment->getBuoysCount());
    insertedInLeaderboard = false;
    isPaused = false;
    mStartPauseTime = 0;
}

Leaderboard *Game::getLeaderboard() {
    return mLeaderboard;
}

int Game::getGameTime() {
    if (mEndTime != 0) {
        return mEndTime;
    } else if (isPaused) {
        return mStartPauseTime - mStartTime;
    } else if (mStartTime != 0) {
        return SDL_GetTicks() - mStartTime;
    } else {
        return 0;
    }
}

int Game::getScore() {
    return mScore;
}

int Game::getScoreLimit() {
    return mScoreLimit;
}

void Game::detectCollision() {
    ShipMesh *shipMesh = mShip->getMesh();
    Mesh *buoyMesh;
    for (int i = 0; i < mEnviroment->getBuoysCount() && !isFinished(); i++) {
        buoyMesh = mEnviroment->getBuoy(i)->getMesh();
        if (mEnviroment->getBuoy(i)->isActive() &&
            BoundingBox::isCollision(shipMesh->w_bounding_box, buoyMesh->w_bounding_box)) {
            mEnviroment->getBuoy(i)->disable();
            mScore++;
        }
    }
}

void Game::go() {
    if (mStartTime == 0) {
        mStartTime = SDL_GetTicks();
    }
}

void Game::reset() {
    mStartTime = 0;
    mEndTime = 0;
    mScore = 0;
    mShip->reset();
    mEnviroment->reset();
    mScoreLimit = mEnviroment->getBuoysCount();
    mLeaderboard->setNumBuoys(mEnviroment->getBuoysCount());
    isPaused = false;
    insertedInLeaderboard = false;
}

bool Game::isFinished() {
    bool isFinished = true;
    for (int i = 0; i < mEnviroment->getBuoysCount(); i++) {
        if (mEnviroment->getBuoy(i)->isActive()) {
            isFinished = false;
        }
    }
    if (isFinished) {
        mEndTime = getGameTime();
        if (!insertedInLeaderboard){
            mLeaderboard->insert(mEndTime);
            insertedInLeaderboard = true;
        }
    }
    return isFinished;
}

void Game::togglePause() {
    if (!isPaused) {
        pause();
    } else {
        resume();
    }
}

void Game::pause() {
    if (!isPaused && mStartTime != 0 && !isFinished()) {
        isPaused = true;
        // Memorizzo il tempo di pausa
        mStartPauseTime = SDL_GetTicks();
    }
}

void Game::resume() {
    if (isPaused && mStartTime != 0 && !isFinished()) {
        isPaused = false;
        // Resetto il tempo di inizio ma ci sommo il tempo trascorso
        // tra l'ultimo startTime e l'ultimo startPauseTime
        mStartTime += SDL_GetTicks() - mStartPauseTime;
    }
}