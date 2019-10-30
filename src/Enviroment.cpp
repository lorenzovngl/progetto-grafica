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

#include "headers/Utils.h"
#include "headers/Enviroment.h"
#include "headers/Texture.h"

Enviroment::Enviroment() {
    for (int i = 0; i < 10; i++) {
        buoy[i] = new Buoy(rand() % 1000, rand() % 1000);
    }
}

void Enviroment::drawSea() {
    const float S = 100; // size
    const float H = 0;   // altezza
    const int K = 10; //disegna K x K quads


    glActiveTexture(GL_TEXTURE0);
    Texture *texture = new Texture(TEXTURE_SEA);
    texture->loadTexture();
    glBindTexture(GL_TEXTURE_2D, texture->getBind());
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

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
            glTexCoord2f(x0, z0);
            glVertex3d(x0, H, z0);
            glTexCoord2f(x1, z0);
            glVertex3d(x1, H, z0);
            glTexCoord2f(x1, z1);
            glVertex3d(x1, H, z1);
            glTexCoord2f(x0, z1);
            glVertex3d(x0, H, z1);
        }
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    //SDL_FreeSurface(surface);
}

void Enviroment::drawSky() {
    glActiveTexture(GL_TEXTURE1);
    Texture *texture = new Texture(TEXTURE_SKY);
    texture->loadTexture();
    glBindTexture(GL_TEXTURE_2D, texture->getBind());
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); // Env map
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glColor3f(0.5, 0.5, 1);
    glDisable(GL_LIGHTING);

    Utils::drawSphere(100.0, 20, 20);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

void Enviroment::render() {
    drawSea();
    drawSky();
    for (int i = 0; i < 10; i++) {
        glPushMatrix();
        buoy[i]->render();
        glPopMatrix();
    }
}