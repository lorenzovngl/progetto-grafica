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
    font = TTF_OpenFont("assets/fonts/FreeSans.ttf", 20);

    if (font == NULL) {
        fprintf(stderr, "Couldn't load font\n");
    }
}

void HUD::displayTime() {
    glPushMatrix();
    glDisable(GL_DEPTH_TEST);

    //glClearColor(0,0,0,0);
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //Utils::setCoordToPixel(); //per avere mapping 1-1 con i pixel schermo

    glDisable(GL_LIGHTING);

    glColor3f(1,1,1);
    //Utils::setCoordToPixel(); //per avere mapping 1-1 con i pixel schermo
    char string[56];
    int millis = SDL_GetTicks();
    int sec = millis/1000;
    int min = sec/60;
    sprintf(string, "%02d:%02d:%03d", min, sec-min*60, millis-sec*1000-min*60);
    glRasterPos2f(0, 0);
    glColor3f(1., 0., 0.);
    glScalef(5, 2, 5);
    for (int i = 0; i < strlen(string); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}

void HUD::display(int v_width, int v_height){
    char string[56];


    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    Utils::setCoordToPixel(v_width, v_height);
    GLText *text = new GLText(TEXTURE_TEXT_TIME,font);
    int millis = game->getGameTime();
    int sec = millis/1000;
    int min = sec/60;
    sprintf(string, "Time: %02d:%02d:%03d", min, sec-min*60, millis-sec*1000-min*60);
    text->setText(string, 255,255,255);
    text->setPosition(50,50);
    text->render();

    text = new GLText(TEXTURE_TEXT_TIME,font);
    sprintf(string, "Catched: %d/%d", game->getScore(), game->getScoreLimit());
    text->setText(string, 255,255,255);
    text->setPosition(v_width/1.8,50);
    text->render();

    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}