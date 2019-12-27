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
#include "headers/GLText.h"
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
    font = TTF_OpenFont("assets/fonts/FreeSans.ttf", 25);

    if (font == NULL) {
        fprintf(stderr, "Couldn't load font\n");
    }
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

    GLText *text = new GLText(TEXTURE_TEXT_TIME,font);
    int millis = game->getGameTime();
    int sec = millis/1000;
    int min = sec/60;
    sprintf(string, "Time: %02d:%02d:%03d", min, sec-min*60, millis-sec*1000-min*60);
    text->setText(string, 255,255,255);
    text->setPosition(v_width/2,v_height/8*7);
    text->setAlignment(ALIGN_CENTER);
    text->render();

    text = new GLText(TEXTURE_TEXT_TIME,font);
    sprintf(string, "Catched: %d/%d", game->getScore(), game->getScoreLimit());
    text->setText(string, 255,255,255);
    text->setPosition(20,50);
    text->render();

    text = new GLText(TEXTURE_TEXT_TIME,font);
    sprintf(string, "FPS: %i", (int) fps);
    text->setText(string, 255,255,255);
    text->setPosition(v_width-100,v_height-10);
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

void HUD::displayStartGameMessage(int v_width, int v_height){
    glPushMatrix();
    glLoadIdentity();
    Utils::setCoordToPixel(v_width, v_height);

    GLText *text = new GLText(TEXTURE_TEXT_TIME,font);
    text->setText((char*) "Time starts when you move the ship.", 255,255,255);
    text->setPosition(v_width/2,v_height/2);
    text->setAlignment(ALIGN_CENTER);
    text->render();

    glPopMatrix();
}

void HUD::displayEndGameMessage(int v_width, int v_height){
    char string[56];

    glPushMatrix();
    glLoadIdentity();
    Utils::setCoordToPixel(v_width, v_height);

    GLText *text = new GLText(TEXTURE_TEXT_TIME,font);
    int millis = game->getGameTime();
    int sec = millis/1000;
    int min = sec/60;
    sprintf(string, "Game finished, your time is: %02d:%02d:%03d", min, sec-min*60, millis-sec*1000-min*60);
    text->setText(string, 255,255,255);
    text->setPosition(v_width/2,v_height/2);
    text->setAlignment(ALIGN_CENTER);
    text->render();

    glPopMatrix();
}