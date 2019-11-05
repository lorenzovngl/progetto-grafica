#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
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
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        buoy[i] = new Buoy(rand() % 20000 - 10000, rand() % 20000 - 10000);
    }
}

void Enviroment::drawFarSea(float ship_x, float ship_y, float ship_z) {
    const float S = 100; // size
    const float H = 0;   // altezza
    const int K = 100; //disegna K x K quads
    float scale_factor = 0.05;

    float s[4] = {(float) 0.05/scale_factor, 0, 0, 0};
    float t[4] = {0, 0, (float) 0.05/scale_factor, 0};
    glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, t);

    // disegna KxK quads
    glBegin(GL_QUADS);
    glColor3f(0.2, 0.2, 1); // colore uguale x tutti i quads
    glNormal3f(0, 1, 0);       // normale verticale uguale x tutti
    for (int x = 0; x < K; x++) {
        for (int z = 0; z < K - 52; z++) {
            float x0 = -S + 2 * (x + 0) * S / K + ship_x;
            float x1 = -S + 2 * (x + 1) * S / K + ship_x;
            float z0 = -S + 2 * (z + 0) * S / K + ship_z;
            float z1 = -S + 2 * (z + 1) * S / K + ship_z;
            glTexCoord2f(x0, z0);
            glVertex3d(x0, H, z0);
            glTexCoord2f(x1, z0);
            glVertex3d(x1, H, z0);
            glTexCoord2f(x1, z1);
            glVertex3d(x1, H, z1);
            glTexCoord2f(x0, z1);
            glVertex3d(x0, H, z1);
        }
        for (int z = K - 48; z < K; z++) {
            float x0 = -S + 2 * (x + 0) * S / K + ship_x;
            float x1 = -S + 2 * (x + 1) * S / K + ship_x;
            float z0 = -S + 2 * (z + 0) * S / K + ship_z;
            float z1 = -S + 2 * (z + 1) * S / K + ship_z;
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
    for (int x = 0; x < K - 52; x++) {
        for (int z = 0; z < K; z++) {
            float x0 = -S + 2 * (x + 0) * S / K + ship_x;
            float x1 = -S + 2 * (x + 1) * S / K + ship_x;
            float z0 = -S + 2 * (z + 0) * S / K + ship_z;
            float z1 = -S + 2 * (z + 1) * S / K + ship_z;
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
    for (int x = K - 48; x < K; x++) {
        for (int z = 0; z < K; z++) {
            float x0 = -S + 2 * (x + 0) * S / K + ship_x;
            float x1 = -S + 2 * (x + 1) * S / K + ship_x;
            float z0 = -S + 2 * (z + 0) * S / K + ship_z;
            float z1 = -S + 2 * (z + 1) * S / K + ship_z;
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
}

void Enviroment::drawNearSea(float ship_x, float ship_y, float ship_z) {
    const float S = 81; // size
    const float H = 0.2;   // altezza
    const int K = 50; //disegna K x K quads
    float scale_factor = 0.05;

    glPushMatrix();
    //glTranslatef(ship_x, ship_y, ship_z);
    glScalef(scale_factor, scale_factor, scale_factor);         // Cambio la scala per disegnare il mare

    float s[4] = {0.05, 0, 0, 0};
    float t[4] = {0, 0, 0.05, 0};
    glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, t);

    // disegna KxK quads
    glBegin(GL_TRIANGLES);
    glColor3f(0.2, 0.2, 1); // colore uguale x tutti i quads
    glNormal3f(0, 1, 0);       // normale verticale uguale x tutti
    for (int x = 0; x < K; x++) {
        for (int z = 0; z < K; z++) {
            float x0 = -S + 2 * (x + 0) * S / K + ship_x*1/scale_factor;
            float x1 = -S + 2 * (x + 1) * S / K + ship_x*1/scale_factor;
            float z0 = -S + 2 * (z + 0) * S / K + ship_z*1/scale_factor;
            float z1 = -S + 2 * (z + 1) * S / K + ship_z*1/scale_factor;
            float motion = (float) SDL_GetTicks()/500;

            glVertex3d(x0, (cos(x0 + motion) + sin(z0 + motion))*H, z0);
            glVertex3d(x1, (cos(x1 + motion) + sin(z0 + motion))*H, z0);
            glVertex3d(x1, (cos(x1 + motion) + sin(z1 + motion))*H, z1);

            glVertex3d(x0, (cos(x0 + motion) + sin(z0 + motion))*H, z0);
            glVertex3d(x1, (cos(x1 + motion) + sin(z1 + motion))*H, z1);
            glVertex3d(x0, (cos(x0 + motion) + sin(z1 + motion))*H, z1);

        }
    }
    glEnd();
    glPopMatrix();
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
    glDisable(GL_LIGHTING);

    Utils::drawSphere(100.0, 20, 20);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

void Enviroment::render(float ship_x, float ship_y, float ship_z) {
    glActiveTexture(GL_TEXTURE0);
    Texture *texture = new Texture(TEXTURE_SEA);
    texture->loadTexture();
    glBindTexture(GL_TEXTURE_2D, texture->getBind());
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    drawNearSea(ship_x, ship_y, ship_z);
    drawFarSea(ship_x, ship_y, ship_z);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
    drawSky();
    for (int i = 0; i < 10; i++) {
        glPushMatrix();
        buoy[i]->render();
        glPopMatrix();
    }
}