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

#include "headers/Enviroment.h"

Enviroment::Enviroment(){
    for (int i = 0; i < 10; i++) {
        buoy[i] = new Buoy(rand() % 1000, rand() % 1000);
    }
}

void Enviroment::drawFloor() {
    const float S = 20; // size
    const float H = 0;   // altezza
    const int K = 10; //disegna K x K quads

    //glEnable(GL_TEXTURE_2D);
    //SDL_Surface* surface = IMG_Load("assets/sea.jpg");
    //SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    // disegna KxK quads
    glBegin(GL_QUADS);
    glColor3f(0.2, 0.2, 1); // colore uguale x tutti i quads
    glNormal3f(0, 1, 0);       // normale verticale uguale x tutti
    for (int x = 0; x < K; x++) {
        for (int z = 0; z < K; z++) {
            float x0 = -S + 2 * (x + 0) * S / K;
            float x1 = -S + 2 * (x + 1) * S / K;
            float z0 = -S + 2 * (z + 0) * S / K;
            float z1 = -S + 2 * (z + 1) * S / K;
            glVertex3d(x0, H, z0);
            glVertex3d(x1, H, z0);
            glVertex3d(x1, H, z1);
            glVertex3d(x0, H, z1);
        }
    }
    glEnd();
    //SDL_FreeSurface(surface);
}

void Enviroment::render(){
    drawFloor();
    for (int i = 0; i < 10; i++){
        glPushMatrix();
        glScalef(-0.005, 0.005, -0.005);
        glColor3f(1, 0, 0);
        glTranslatef(buoy[i]->getCoordX(), 0, buoy[i]->getCoordZ());
        buoy[i]->render();
        glPopMatrix();
    }
}