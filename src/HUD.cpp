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

#include "headers/HUD.h"
#include "headers/GLText.h"
#include "headers/Utils.h"

HUD::HUD() {
    if (TTF_Init() < 0) {
        fprintf(stderr, "Couldn't initialize TTF: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }
    //inizializzo anche la libreria ttf
    font = TTF_OpenFont("assets/fonts/neuropol.ttf", 12);
    // font = TTF_OpenFont ("/Library/Fonts/Arial.ttf", DEFAULT_PTSIZE);

    if (font == NULL) {
        fprintf(stderr, "Couldn't load font\n");
    }
}

void HUD::display() {
    glDisable(GL_DEPTH_TEST);

    //glClearColor(0,0,0,0);
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    Utils::setCoordToPixel(); //per avere mapping 1-1 con i pixel schermo

    glDisable(GL_LIGHTING);

    /*glBegin(GL_QUADS); //creo un quad dove ci scrivero del testo
    glColor3f(1,1,1);

    glVertex2i(0, 0);
    glVertex2i(200, 0);
    glVertex2i(200,  200);
    glVertex2i(0,  200);
    glEnd();*/

    Utils::setCoordToPixel();
    GLText *text = new GLText(20, font);
    glActiveTexture(GL_TEXTURE20);
    text->setText((char*) "Welcome", 0,0,255);
    text->setPosition(100,100);
    text->render();
    free(text);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}