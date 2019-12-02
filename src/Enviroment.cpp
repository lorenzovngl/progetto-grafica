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
#include "headers/TextureManager.h"

Enviroment::Enviroment(TextureManager *textureManager) {
    srand(time(NULL));
    for (int i = 0; i < BUOYS_COUNT; i++) {
        buoy[i] = new Buoy(i, rand() % 20 - 10, rand() % 20 - 10, textureManager);
    }
    this->textureManager = textureManager;
    textureManager->loadTexture(TEXTURE_SEA);
    textureManager->loadTexture(TEXTURE_SKY);
}

Buoy* Enviroment::getBuoy(int i) {
    return buoy[i];
}

int Enviroment::getBuoysCount() {
    return BUOYS_COUNT;
}

void Enviroment::drawFarSea(float ship_x, float ship_y, float ship_z) {
    const float S = 500; // size
    const float H = 0;   // altezza
    const int K = 100; //disegna K x K quads
    float scale_factor = 0.05;

    float s[4] = {(float) 0.05/scale_factor, 0, 0, 0};
    float t[4] = {0, 0, (float) 0.05/scale_factor, 0};
    glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, t);

    // disegna KxK quads
    glBegin(GL_QUADS);
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
    float scale_factor = 0.05;
    const float S = 400; // size
    const float H = 0.2;   // altezza
    const int K = 100; //disegna K x K quads

    glPushMatrix();
    glTranslatef(ship_x, ship_y, ship_z);

    float s[4] = {0.05, 0, 0, 0};
    float t[4] = {0, 0, 0.05, 0};
    glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, t);

    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // disegna KxK quads
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 1, 0);       // normale verticale uguale x tutti
    for (int x = 0; x < K; x++) {
        for (int z = 0; z < K; z++) {
            float x0 = (-S + 2 * (x + 0) * S / K)*scale_factor;
            float x1 = (-S + 2 * (x + 1) * S / K)*scale_factor;
            float z0 = (-S + 2 * (z + 0) * S / K)*scale_factor;
            float z1 = (-S + 2 * (z + 1) * S / K)*scale_factor;
            float motion = (float) SDL_GetTicks()/500;
            float y00 = (cos(x0 + motion) + sin(z0 + motion))*H*scale_factor;
            float y01 = (cos(x0 + motion) + sin(z1 + motion))*H*scale_factor;
            float y11 = (cos(x1 + motion) + sin(z1 + motion))*H*scale_factor;
            float y10 = (cos(x1 + motion) + sin(z0 + motion))*H*scale_factor;

            // Blocco il movimento dei vertici del perimetro
            if (x == 0){
                y00 = 0;
                y01 = 0;
            } else if (x == K - 1){
                y10 = 0;
                y11 = 0;
            }
            if (z == 0){
                y00 = 0;
                y10 = 0;
            } else if (z == K - 1){
                y01 = 0;
                y11 = 0;
            }

            glVertex3d(x0, y00, z0);
            glVertex3d(x1, y10, z0);
            glVertex3d(x1, y11, z1);

            glVertex3d(x0, y00, z0);
            glVertex3d(x1, y11, z1);
            glVertex3d(x0, y01, z1);

        }
    }
    glEnd();
    glPopMatrix();
}

void Enviroment::drawSky() {
    textureManager->enableTexture(TEXTURE_SKY);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); // Env map
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glDisable(GL_LIGHTING);

    Utils::drawSphere(100.0, 20, 20);

    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}

void Enviroment::renderBuoys(){
    for (int i = 0; i < BUOYS_COUNT; i++) {
        if (buoy[i]->isActive()){
            glPushMatrix();
            buoy[i]->render();
            glPopMatrix();
        }
    }
}

void Enviroment::render(float ship_x, float ship_y, float ship_z, bool texture_enabled) {
    if (texture_enabled){
        textureManager->enableTexture(TEXTURE_SEA);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f( 0.0f, 0.0f, 1.0f, 0.7f );
    }
    drawNearSea(ship_x, ship_y, ship_z);
    //drawFarSea(ship_x, ship_y, ship_z);
    if (texture_enabled){
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);
    }
    //drawSky();
}