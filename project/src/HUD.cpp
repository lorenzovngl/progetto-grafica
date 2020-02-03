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
    mFont18 = TTF_OpenFont("assets/fonts/FreeSans.ttf", 18);
    mFont25 = TTF_OpenFont("assets/fonts/FreeSans.ttf", 25);
    mFont40 = TTF_OpenFont("assets/fonts/FreeSans.ttf", 40);
    mFont100 = TTF_OpenFont("assets/fonts/FreeSans.ttf", 100);
    if (mFont18 == NULL || mFont25 == NULL || mFont40 == NULL) {
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

    GLText *text = new GLText(TEXTURE_TEXT, mFont18);
    sprintf(string, "P - pause    C - Commands list    L - Leaderboard");
    text->setText(string, 255,255,255);
    text->setPosition(20,v_height-10);
    text->setAlignment(ALIGN_LEFT);
    text->render();

    sprintf(string, "FPS: %i", (int) fps);
    text->setText(string, 255,255,255);
    text->setPosition(v_width-100,v_height-10);
    text->setAlignment(ALIGN_LEFT);
    text->render();

    text = new GLText(TEXTURE_TEXT, mFont25);
    int millis = game->getGameTime();
    int sec = millis/1000;
    int min = sec/60;
    sprintf(string, "%s    %02d:%02d:%03d", game->getLeaderboard()->getUsename(), min, sec-min*60, millis-sec*1000-min*60);
    text->setText(string, 255,255,255);
    text->setPosition(v_width/2,v_height/8*7);
    text->setAlignment(ALIGN_CENTER);
    text->render();

    sprintf(string, "Catched: %d/%d", game->getScore(), game->getScoreLimit());
    text->setText(string, 255,255,255);
    text->setPosition(20,50);
    text->setAlignment(ALIGN_LEFT);
    text->render();

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

    GLText *text = new GLText(TEXTURE_TEXT, mFont100);
    text->setText((char*) "Open Sailing", 255, 255, 255);
    text->setPosition(v_width/2,v_height/1.1);
    text->setAlignment(ALIGN_CENTER);
    text->render();

    text = new GLText(TEXTURE_TEXT, mFont25);
    text->setText((char*) "A Ship Game in OpenGL", 255, 255, 255);
    text->setPosition(v_width/2,v_height/1.1 - 130);
    text->setAlignment(ALIGN_CENTER);
    text->render();

    sprintf(buffer[0], "Author: Lorenzo Vainigli");
    sprintf(buffer[1], "Corso di Grafica, Laurea Magistrale in Informatica");
    sprintf(buffer[2], "Universita' di Bologna, A.A. 2017/18");
    text = new GLText(TEXTURE_TEXT, mFont18);
    for (int i = 0; i < 3; i++){
        text->setText(buffer[i], 255, 255, 255);
        text->setPosition(v_width/2,v_height/1.1 - 200 - 25 * i);
        text->setAlignment(ALIGN_CENTER);
        text->render();
    }

    sprintf(buffer[0], "Please insert your name,");
    sprintf(buffer[1], "then press Enter.");
    sprintf(buffer[2], "%s_", input);
    text = new GLText(TEXTURE_TEXT, mFont40);
    for (int i = 0; i < 3; i++){
        text->setText(buffer[i], 255, 255, 255);
        text->setPosition(v_width/2,v_height/2.5 - 50 * i);
        text->setAlignment(ALIGN_CENTER);
        text->render();
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
    GLText *text = new GLText(TEXTURE_TEXT, mFont25);
    for (int i = 0; i < 2; i++){
        text->setText(buffer[i], 255, 255, 255);
        text->setPosition(v_width/2,v_height/1.5 - 30 * i);
        text->setAlignment(ALIGN_CENTER);
        text->render();
    }

    sprintf(buffer[0], "Controls:");
    sprintf(buffer[1], "ARROW UP - Accelerate / Go forward");
    sprintf(buffer[2], "ARROW DOWN - Brake / Reverse / Go backword");
    sprintf(buffer[3], "ARROW LEFT - Turn left");
    sprintf(buffer[4], "ARROW RIGHT - Turn right");
    for (int i = 0; i < 5; i++){
        text->setText(buffer[i], 255, 255, 255);
        text->setPosition(v_width/4,v_height/1.5 - 30 * (i+3));
        text->setAlignment(ALIGN_LEFT);
        text->render();
    }

    text = new GLText(TEXTURE_TEXT, mFont18);
    text->setText((char*) "Press B to change the number of buoys to catch.", 255, 255, 255);
    text->setPosition(v_width/2,v_height/1.5 - 270);
    text->setAlignment(ALIGN_CENTER);
    text->render();

    text = new GLText(TEXTURE_TEXT, mFont40);
    text->setText((char*) "Time starts when you move the ship.", 255,255,255);
    text->setPosition(v_width/2,v_height/4);
    text->setAlignment(ALIGN_CENTER);
    text->render();

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
    GLText *text = new GLText(TEXTURE_TEXT, mFont40);
    text->setText(buffer, 255, 255, 255);
    text->setPosition(v_width/2,v_height/1.5);
    text->setAlignment(ALIGN_CENTER);
    text->render();

    text = new GLText(TEXTURE_TEXT, mFont25);
    text->setText((char*) "Press N to start a new game.", 255, 255, 255);
    text->setPosition(v_width/2,v_height/1.5 - 50);
    text->setAlignment(ALIGN_CENTER);
    text->render();

    glPopMatrix();
}

void HUD::displayPauseMessage(int v_width, int v_height){
    glPushMatrix();
    glLoadIdentity();
    Utils::setCoordToPixel(v_width, v_height);

    GLText *text = new GLText(TEXTURE_TEXT, mFont40);
    text->setText((char*) "Game paused", 255, 255, 255);
    text->setPosition(v_width/2,v_height/1.5);
    text->setAlignment(ALIGN_CENTER);
    text->render();

    text = new GLText(TEXTURE_TEXT, mFont25);
    text->setText((char*) "Press P to resume, N to start a new game.", 255, 255, 255);
    text->setPosition(v_width/2,v_height/1.5 - 50);
    text->setAlignment(ALIGN_CENTER);
    text->render();

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
    GLText *text = new GLText(TEXTURE_TEXT, mFont25);
    for (int i = 0; i < 9; i++){
        text->setText(buffer[i], 255, 255, 255);
        text->setPosition(v_width/4,v_height/1.5 - 30 * i);
        text->setAlignment(ALIGN_LEFT);
        text->render();
    }

    glPopMatrix();
}

void HUD::displayLeaderboardColumn(int col, Leaderboard::LBItem** items, int v_width, int v_height){
    char buffer[200];
    GLText *text = new GLText(TEXTURE_TEXT, mFont18);
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
        text->setText(buffer, 255, 255, 255);
        if (i < floor(LEADERBOARD_LENGHT/2)){
            text->setPosition(v_width/2 - 450 + padding,v_height/1.5 - 30 * i);
        } else {
            text->setPosition(v_width/2 + 50 + padding,v_height/1.5 - 30 * (i - floor(LEADERBOARD_LENGHT/2)));
        }
        text->setAlignment(ALIGN_LEFT);
        text->render();
        i++;
    }
}

void HUD::displayLeaderboard(Leaderboard *leaderboard, int v_width, int v_height) {
    Leaderboard::LBItem** items;

    glPushMatrix();
    glLoadIdentity();
    Utils::setCoordToPixel(v_width, v_height);

    items = leaderboard->read();
    GLText *text = new GLText(TEXTURE_TEXT, mFont18);
    // Posizione
    text->setText((char*) "Pos", 255, 255, 255);
    text->setPosition(v_width/2 - 450, v_height/1.5 + 40);
    text->setAlignment(ALIGN_LEFT);
    text->render();
    text->setPosition(v_width/2 + 50, v_height/1.5 + 40);
    text->render();
    displayLeaderboardColumn(0, items, v_width, v_height);
    // Nome
    text->setText((char*) "Name", 255, 255, 255);
    text->setPosition(v_width/2 - 410, v_height/1.5 + 40);
    text->setAlignment(ALIGN_LEFT);
    text->render();
    text->setPosition(v_width/2 + 90, v_height/1.5 + 40);
    text->render();
    displayLeaderboardColumn(1, items, v_width, v_height);
    // Numero di boe
    text->setText((char*) "Buoys", 255, 255, 255);
    text->setPosition(v_width/2 - 300, v_height/1.5 + 40);
    text->setAlignment(ALIGN_LEFT);
    text->render();
    text->setPosition(v_width/2 + 200, v_height/1.5 + 40);
    text->render();
    displayLeaderboardColumn(2, items, v_width, v_height);
    // Tempo
    text->setText((char*) "Time", 255, 255, 255);
    text->setPosition(v_width/2 - 230, v_height/1.5 + 40);
    text->setAlignment(ALIGN_LEFT);
    text->render();
    text->setPosition(v_width/2 + 270, v_height/1.5 + 40);
    text->render();
    displayLeaderboardColumn(3, items, v_width, v_height);
    // Tempo / boe
    text->setText((char*) "Time/buoys", 255, 255, 255);
    text->setPosition(v_width/2 - 110, v_height/1.5 + 40);
    text->setAlignment(ALIGN_LEFT);
    text->render();
    text->setPosition(v_width/2 + 390, v_height/1.5 + 40);
    text->render();
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
    GLText *text = new GLText(TEXTURE_TEXT, mFont40);
    for (int i = 0; i < 3; i++){
        text->setText(buffer[i], 255, 255, 255);
        text->setPosition(v_width/2,v_height/1.5 - 50 * i);
        text->setAlignment(ALIGN_CENTER);
        text->render();
    }

    glPopMatrix();
}