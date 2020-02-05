#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../lib/GCGraLib2/GCGraLib2.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector> // la classe vector di SDL

#endif

#include "headers/HUD.h"
#include "../lib/GLText.h"
#include "headers/Utils.h"
#include "headers/Texture.h"

HUD::HUD(Game *p_game) {
    game = p_game;
    if (TTF_Init() < 0) {
        fprintf(stderr, "Couldn't initialize TTF: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }
    //inizializzo anche la libreria ttf
    mText18 = new GLText(TEXTURE_TEXT, TTF_OpenFont("assets/fonts/FreeSans.ttf", 18));
    mText25 = new GLText(TEXTURE_TEXT, TTF_OpenFont("assets/fonts/FreeSans.ttf", 25));
    mText40 = new GLText(TEXTURE_TEXT, TTF_OpenFont("assets/fonts/FreeSans.ttf", 40));
    mText100 = new GLText(TEXTURE_TEXT, TTF_OpenFont("assets/fonts/FreeSans.ttf", 100));
    if (mText18 == NULL || mText25 == NULL || mText40 == NULL || mText100 == NULL) {
        fprintf(stderr, "Couldn't load font\n");
    }
    isCommandsListVisibile = false;
    isLeaderboardVisible = false;
    bAskBuoys = false;
    bAskUsername = true;
}

void drawRegularPolygon(float cx, float cy, float radius, int slices, float facing){
    float angle_step = 2*M_PI / slices;
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < slices; i++){
        float angle = angle_step * i + facing;
        glVertex2f(cx, cy);
        glVertex2f(cx + radius*cos(angle), cy + radius*sin(angle));
        angle = angle_step * (i+1) + facing;
        glVertex2f(cx + radius*cos(angle), cy + radius*sin(angle));
    }
    glEnd();
}

void drawNavigator(float cx, float cy, float radius, float facing){
    float angle_step = 2*M_PI / 3;
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3; i++){
        if (i != 1){
            float angle = angle_step * i + facing;
            glVertex2f(cx, cy);
            glVertex2f(cx + radius*cos(angle), cy + radius*sin(angle));
            angle = angle_step * (i+1) + facing;
            glVertex2f(cx + radius*cos(angle), cy + radius*sin(angle));
        }
    }
    glEnd();
}

void HUD::display(int v_width, int v_height, float ship_cx, float ship_cy, float ship_facing, Enviroment* enviroment, float fps){
    char string[56];

    glPushMatrix();
    glLoadIdentity();
    Utils::setCoordToPixel(v_width, v_height);

    sprintf(string, "P - pause    C - Commands list    L - Leaderboard");
    mText18->setText(string, 255,255,255);
    mText18->setPosition(20,v_height-10);
    mText18->setAlignment(ALIGN_LEFT);
    mText18->render();

    sprintf(string, "FPS: %i", (int) fps);
    mText18->setText(string, 255,255,255);
    mText18->setPosition(v_width-100,v_height-10);
    mText18->setAlignment(ALIGN_LEFT);
    mText18->render();

    int millis = game->getGameTime();
    int sec = millis/1000;
    int min = sec/60;
    sprintf(string, "%s    %02d:%02d:%03d", game->getLeaderboard()->getUsename(), min, sec-min*60, millis-sec*1000-min*60);
    mText25->setText(string, 255,255,255);
    mText25->setPosition(v_width/2,v_height/8*7);
    mText25->setAlignment(ALIGN_CENTER);
    mText25->render();

    sprintf(string, "Catched: %d/%d", game->getScore(), game->getScoreLimit());
    mText25->setText(string, 255,255,255);
    mText25->setPosition(20,50);
    mText25->setAlignment(ALIGN_LEFT);
    mText25->render();

    // Map
    glDisable(GL_LIGHTING);
    int padding = 70;
    float mapWidth = 100;
    float mapCenterX = v_width - padding;
    float mapCenterY = padding;

    // Ship
    glColor3f(1, 0, 0);
    float facing_radiants = (float) (ship_facing * M_PI/180);
    drawNavigator(mapCenterX + ship_cx, mapCenterY + ship_cy, 7, facing_radiants + M_PI/2);
    // Buoys
    glColor3f(0, 1, 0);
    for (int i = 0; i < enviroment->getBuoysCount(); i++){
        Buoy* buoy = enviroment->getBuoy(i);
        if (buoy->isActive()){
            drawRegularPolygon(mapCenterX + buoy->getCoordX(), mapCenterY - buoy->getCoordZ(), 3, 50, 0);
        }
    }
    // World
    glColor3f(0, 0, 1);
    drawRegularPolygon(mapCenterX, mapCenterY, 65, 50, 0);

    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void HUD::askUsername(int v_width, int v_height, char *input) {
    char buffer[3][100];

    glPushMatrix();
    glLoadIdentity();
    Utils::setCoordToPixel(v_width, v_height);

    mText100->setText((char*) "Open Sailing", 255, 255, 255);
    mText100->setPosition(v_width/2,v_height/1.1);
    mText100->setAlignment(ALIGN_CENTER);
    mText100->render();

    mText25->setText((char*) "A Ship Game in OpenGL", 255, 255, 255);
    mText25->setPosition(v_width/2,v_height/1.1 - 130);
    mText25->setAlignment(ALIGN_CENTER);
    mText25->render();

    sprintf(buffer[0], "Author: Lorenzo Vainigli");
    sprintf(buffer[1], "Corso di Grafica, Laurea Magistrale in Informatica");
    sprintf(buffer[2], "Universita' di Bologna, A.A. 2017/18");
    for (int i = 0; i < 3; i++){
        mText18->setText(buffer[i], 255, 255, 255);
        mText18->setPosition(v_width/2,v_height/1.1 - 200 - 25 * i);
        mText18->setAlignment(ALIGN_CENTER);
        mText18->render();
    }

    sprintf(buffer[0], "Please insert your name,");
    sprintf(buffer[1], "then press Enter.");
    sprintf(buffer[2], "%s_", input);
    for (int i = 0; i < 3; i++){
        mText40->setText(buffer[i], 255, 255, 255);
        mText40->setPosition(v_width/2,v_height/2.5 - 50 * i);
        mText40->setAlignment(ALIGN_CENTER);
        mText40->render();
    }

    glPopMatrix();
}

void HUD::displayStartGameMessage(int v_width, int v_height){
    char buffer[5][100];

    glPushMatrix();
    glLoadIdentity();
    Utils::setCoordToPixel(v_width, v_height);

    sprintf(buffer[0], "Welcome to the game! Your goal is to catch all the buoys");
    sprintf(buffer[1], "placed around the ship in the shortest possible time.");
    for (int i = 0; i < 2; i++){
        mText25->setText(buffer[i], 255, 255, 255);
        mText25->setPosition(v_width/2,v_height/1.5 - 30 * i);
        mText25->setAlignment(ALIGN_CENTER);
        mText25->render();
    }

    sprintf(buffer[0], "Controls:");
    sprintf(buffer[1], "ARROW UP - Accelerate / Go forward");
    sprintf(buffer[2], "ARROW DOWN - Brake / Reverse / Go backword");
    sprintf(buffer[3], "ARROW LEFT - Turn left");
    sprintf(buffer[4], "ARROW RIGHT - Turn right");
    for (int i = 0; i < 5; i++){
        mText25->setText(buffer[i], 255, 255, 255);
        mText25->setPosition(v_width/4,v_height/1.5 - 30 * (i+3));
        mText25->setAlignment(ALIGN_LEFT);
        mText25->render();
    }
    
    mText18->setText((char*) "Press B to change the number of buoys to catch.", 255, 255, 255);
    mText18->setPosition(v_width/2,v_height/1.5 - 270);
    mText18->setAlignment(ALIGN_CENTER);
    mText18->render();

    mText40->setText((char*) "Time starts when you move the ship.", 255,255,255);
    mText40->setPosition(v_width/2,v_height/4);
    mText40->setAlignment(ALIGN_CENTER);
    mText40->render();

    glPopMatrix();
}

void HUD::displayEndGameMessage(int v_width, int v_height){
    char buffer[100];

    glPushMatrix();
    glLoadIdentity();
    Utils::setCoordToPixel(v_width, v_height);

    int millis = game->getGameTime();
    int sec = millis/1000;
    int min = sec/60;
    sprintf(buffer, "Game finished, your time is: %02d:%02d:%03d.", min, sec - min * 60, millis - sec * 1000 - min * 60);
    mText40->setText(buffer, 255, 255, 255);
    mText40->setPosition(v_width/2,v_height/1.5);
    mText40->setAlignment(ALIGN_CENTER);
    mText40->render();
    
    mText25->setText((char*) "Press N to start a new game, Q to quit.", 255, 255, 255);
    mText25->setPosition(v_width/2,v_height/1.5 - 50);
    mText25->setAlignment(ALIGN_CENTER);
    mText25->render();

    glPopMatrix();
}

void HUD::displayPauseMessage(int v_width, int v_height){
    glPushMatrix();
    glLoadIdentity();
    Utils::setCoordToPixel(v_width, v_height);

    mText40->setText((char*) "Game paused", 255, 255, 255);
    mText40->setPosition(v_width/2,v_height/1.5);
    mText40->setAlignment(ALIGN_CENTER);
    mText40->render();
    
    mText25->setText((char*) "Press P to resume, N to start a new game.", 255, 255, 255);
    mText25->setPosition(v_width/2,v_height/1.5 - 50);
    mText25->setAlignment(ALIGN_CENTER);
    mText25->render();

    glPopMatrix();
}

void HUD::displayCommands(int v_width, int v_height){
    char buffer[9][50];

    glPushMatrix();
    glLoadIdentity();
    Utils::setCoordToPixel(v_width, v_height);

    sprintf(buffer[0], "ARROW UP - Accelerate / Go forward");
    sprintf(buffer[1], "ARROW DOWN - Brake / Reverse / Go backword");
    sprintf(buffer[2], "ARROW LEFT - Turn left");
    sprintf(buffer[3], "ARROW RIGHT - Turn right");
    sprintf(buffer[4], "F1 - Change camera");
    sprintf(buffer[5], "F2 - Toggle wireframes");
    sprintf(buffer[6], "F3 - Toggle shadows");
    sprintf(buffer[7], "F4 - Toggle fog");
    sprintf(buffer[8], "F5 - Toggle advanced rendering (shaders)");
    for (int i = 0; i < 9; i++){
        mText25->setText(buffer[i], 255, 255, 255);
        mText25->setPosition(v_width/4,v_height/1.5 - 30 * i);
        mText25->setAlignment(ALIGN_LEFT);
        mText25->render();
    }

    glPopMatrix();
}

void HUD::displayLeaderboardColumn(int col, Leaderboard::LBItem** items, int v_width, int v_height){
    char buffer[200];
    int i = 0;
    int padding;
    while (i < LEADERBOARD_LENGHT){
        if (items[i] != nullptr) {
            int millis, sec, min;
            switch (col){
                case 0:         // Posizione
                    sprintf(buffer, "%d.", i+1);
                    padding = 0;
                    break;
                case 1:         // Nome
                    sprintf(buffer, "%s", items[i]->name);
                    padding = 40;
                    break;
                case 2:         // Numero di boe
                    sprintf(buffer, "%d", items[i]->numBuoys);
                    padding = 150;
                    break;
                case 3:         // Tempo
                    millis = items[i]->time;
                    sec = millis / 1000;
                    min = sec / 60;
                    sprintf(buffer, "%02d:%02d:%03d", min, sec - min * 60, millis - sec * 1000 - min * 60);
                    padding = 220;
                    break;
                case 4:         // Tempo / boe
                    millis = items[i]->time / items[i]->numBuoys;
                    sec = millis / 1000;
                    min = sec / 60;
                    sprintf(buffer, "%02d:%02d:%03d", min, sec - min * 60, millis - sec * 1000 - min * 60);
                    padding = 340;
                    break;
                default:
                    break;
            }
        } else {
            sprintf(buffer, "--");
        }
        mText18->setText(buffer, 255, 255, 255);
        if (i < floor(LEADERBOARD_LENGHT/2)){
            mText18->setPosition(v_width/2 - 450 + padding,v_height/1.5 - 30 * i);
        } else {
            mText18->setPosition(v_width/2 + 50 + padding,v_height/1.5 - 30 * (i - floor(LEADERBOARD_LENGHT/2)));
        }
        mText18->setAlignment(ALIGN_LEFT);
        mText18->render();
        i++;
    }
}

void HUD::displayLeaderboard(Leaderboard *leaderboard, int v_width, int v_height) {
    Leaderboard::LBItem** items;

    glPushMatrix();
    glLoadIdentity();
    Utils::setCoordToPixel(v_width, v_height);

    items = leaderboard->read();
    // Posizione
    mText18->setText((char*) "Pos", 255, 255, 255);
    mText18->setPosition(v_width/2 - 450, v_height/1.5 + 40);
    mText18->setAlignment(ALIGN_LEFT);
    mText18->render();
    mText18->setPosition(v_width/2 + 50, v_height/1.5 + 40);
    mText18->render();
    displayLeaderboardColumn(0, items, v_width, v_height);
    // Nome
    mText18->setText((char*) "Name", 255, 255, 255);
    mText18->setPosition(v_width/2 - 410, v_height/1.5 + 40);
    mText18->setAlignment(ALIGN_LEFT);
    mText18->render();
    mText18->setPosition(v_width/2 + 90, v_height/1.5 + 40);
    mText18->render();
    displayLeaderboardColumn(1, items, v_width, v_height);
    // Numero di boe
    mText18->setText((char*) "Buoys", 255, 255, 255);
    mText18->setPosition(v_width/2 - 300, v_height/1.5 + 40);
    mText18->setAlignment(ALIGN_LEFT);
    mText18->render();
    mText18->setPosition(v_width/2 + 200, v_height/1.5 + 40);
    mText18->render();
    displayLeaderboardColumn(2, items, v_width, v_height);
    // Tempo
    mText18->setText((char*) "Time", 255, 255, 255);
    mText18->setPosition(v_width/2 - 230, v_height/1.5 + 40);
    mText18->setAlignment(ALIGN_LEFT);
    mText18->render();
    mText18->setPosition(v_width/2 + 270, v_height/1.5 + 40);
    mText18->render();
    displayLeaderboardColumn(3, items, v_width, v_height);
    // Tempo / boe
    mText18->setText((char*) "Time/buoys", 255, 255, 255);
    mText18->setPosition(v_width/2 - 110, v_height/1.5 + 40);
    mText18->setAlignment(ALIGN_LEFT);
    mText18->render();
    mText18->setPosition(v_width/2 + 390, v_height/1.5 + 40);
    mText18->render();
    displayLeaderboardColumn(4, items, v_width, v_height);

    glPopMatrix();
}

void HUD::toggleCommandsList(){
    if (isCommandsListVisibile){
        game->resume();
    } else {
        game->pause();
    }
    isCommandsListVisibile = !isCommandsListVisibile;
}

void HUD::toggleLeaderboard(){
    if (isLeaderboardVisible){
        game->resume();
    } else {
        game->pause();
    }
    isLeaderboardVisible = !isLeaderboardVisible;
}

void HUD::askNumberOfBuoys(int v_width, int v_height, char *input) {
    char buffer[3][100];

    glPushMatrix();
    glLoadIdentity();
    Utils::setCoordToPixel(v_width, v_height);

    sprintf(buffer[0], "Please insert the number of bouys you want");
    sprintf(buffer[1], "to play with (from 1 to %d), then press Enter.", BUOYS_COUNT_MAX);
    sprintf(buffer[2], "%s_", input);
    for (int i = 0; i < 3; i++){
        mText40->setText(buffer[i], 255, 255, 255);
        mText40->setPosition(v_width/2,v_height/1.5 - 50 * i);
        mText40->setAlignment(ALIGN_CENTER);
        mText40->render();
    }

    glPopMatrix();
}