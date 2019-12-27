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

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector> // la classe vector di SDL

#endif

#include "headers/Utils.h"
#include "headers/Enviroment.h"
#include "headers/Texture.h"
#include "headers/TextureManager.h"
#include "../lib/ShadowMapper/ShadowMapper.h"
#include "headers/Frontier.h"

Enviroment::Enviroment(TextureManager *textureManager, ShadowMapper *shadowMapper, ShaderParams *shaderParams, Options *options) {
    scale_factor = 0.05;
    srand(time(NULL));
    for (int i = 0; i < BUOYS_COUNT; i++) {
        float cx = (float) (rand() % FRONTIER_LIMIT) - FRONTIER_LIMIT/2;
        float cy = (float) (rand() % FRONTIER_LIMIT) - FRONTIER_LIMIT/2;
        buoy[i] = new Buoy(i, cx, cy, textureManager, shadowMapper, shaderParams, options);
    }
    this->textureManager = textureManager;
    this->shadowMapper = shadowMapper;
    mShaderParams = shaderParams;
    this->options = options;
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
    const float S = 200; // size
    const float H = 0;   // altezza
    const int K = 100; //disegna K x K quads

    float s[4] = {(float) 0.45, 0, 0, 0};
    float t[4] = {0, 0, (float) 0.45, 0};
    glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, t);
    GLint genCoords = glGetUniformLocation(shadowMapper->defaultPass.program, "u_genCoords");
    GLint sPlane = glGetUniformLocation(shadowMapper->defaultPass.program, "u_sPlane");
    GLint tPlane = glGetUniformLocation(shadowMapper->defaultPass.program, "u_tPlane");
    glUniform1i(genCoords, 1);
    glUniform4fv(sPlane, 1, s);
    glUniform4fv(tPlane, 1, t);

    // disegna KxK quads
    glNormal3f(0, 1, 0);       // normale verticale uguale x tutti
    for (int x = 0; x < K; x++) {
        for (int z = 0; z < K - 55; z++) {
            float x0 = -S + 2 * (x + 0) * S / K + ship_x;
            float x1 = -S + 2 * (x + 1) * S / K + ship_x;
            float z0 = -S + 2 * (z + 0) * S / K + ship_z;
            float z1 = -S + 2 * (z + 1) * S / K + ship_z;
            if (options->areWireframesEnabled()){
                glBegin(GL_LINE_LOOP);
            } else {
                glBegin(GL_QUADS);
            }
            glTexCoord2f(x0, z0);
            glVertex3d(x0, H, z0);
            glTexCoord2f(x1, z0);
            glVertex3d(x1, H, z0);
            glTexCoord2f(x1, z1);
            glVertex3d(x1, H, z1);
            glTexCoord2f(x0, z1);
            glVertex3d(x0, H, z1);
            glEnd();
        }
        for (int z = K - 45; z < K; z++) {
            float x0 = -S + 2 * (x + 0) * S / K + ship_x;
            float x1 = -S + 2 * (x + 1) * S / K + ship_x;
            float z0 = -S + 2 * (z + 0) * S / K + ship_z;
            float z1 = -S + 2 * (z + 1) * S / K + ship_z;
            if (options->areWireframesEnabled()){
                glBegin(GL_LINE_LOOP);
            } else {
                glBegin(GL_QUADS);
            }
            glTexCoord2f(x0, z0);
            glVertex3d(x0, H, z0);
            glTexCoord2f(x1, z0);
            glVertex3d(x1, H, z0);
            glTexCoord2f(x1, z1);
            glVertex3d(x1, H, z1);
            glTexCoord2f(x0, z1);
            glVertex3d(x0, H, z1);
            glEnd();
        }
    }
    for (int x = 0; x < K - 55; x++) {
        for (int z = 0; z < K; z++) {
            float x0 = -S + 2 * (x + 0) * S / K + ship_x;
            float x1 = -S + 2 * (x + 1) * S / K + ship_x;
            float z0 = -S + 2 * (z + 0) * S / K + ship_z;
            float z1 = -S + 2 * (z + 1) * S / K + ship_z;
            if (options->areWireframesEnabled()){
                glBegin(GL_LINE_LOOP);
            } else {
                glBegin(GL_QUADS);
            }
            glTexCoord2f(x0, z0);
            glVertex3d(x0, H, z0);
            glTexCoord2f(x1, z0);
            glVertex3d(x1, H, z0);
            glTexCoord2f(x1, z1);
            glVertex3d(x1, H, z1);
            glTexCoord2f(x0, z1);
            glVertex3d(x0, H, z1);
            glEnd();
        }
    }
    for (int x = K - 45; x < K; x++) {
        for (int z = 0; z < K; z++) {
            float x0 = -S + 2 * (x + 0) * S / K + ship_x;
            float x1 = -S + 2 * (x + 1) * S / K + ship_x;
            float z0 = -S + 2 * (z + 0) * S / K + ship_z;
            float z1 = -S + 2 * (z + 1) * S / K + ship_z;
            if (options->areWireframesEnabled()){
                glBegin(GL_LINE_LOOP);
            } else {
                glBegin(GL_QUADS);
            }
            glTexCoord2f(x0, z0);
            glVertex3d(x0, H, z0);
            glTexCoord2f(x1, z0);
            glVertex3d(x1, H, z0);
            glTexCoord2f(x1, z1);
            glVertex3d(x1, H, z1);
            glTexCoord2f(x0, z1);
            glVertex3d(x0, H, z1);
            glEnd();
        }
    }
}

void Enviroment::drawNearSea(float ship_x, float ship_y, float ship_z) {
    const float S = 400; // size
    const float H = 0.2;   // altezza
    const int K = 100; //disegna K x K quads

    glPushMatrix();
    glScalef(scale_factor, scale_factor, scale_factor);
    //glTranslatef(ship_x/scale_factor, ship_y/scale_factor, ship_z/scale_factor);

    float s[4] = {0.1, 0, 0, 0};
    float t[4] = {0, 0, 0.1, 0};
    glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, t);
    GLint genCoords = glGetUniformLocation(shadowMapper->defaultPass.program, "u_genCoords");
    GLint sPlane = glGetUniformLocation(shadowMapper->defaultPass.program, "u_sPlane");
    GLint tPlane = glGetUniformLocation(shadowMapper->defaultPass.program, "u_tPlane");
    glUniform1i(genCoords, 1);
    glUniform4fv(sPlane, 1, s);
    glUniform4fv(tPlane, 1, t);

    // disegna KxK quads
    glNormal3f(0, 1, 0);       // normale verticale uguale x tutti
    for (int x = 0; x < K; x++) {
        for (int z = 0; z < K; z++) {
            float x0 = (-S + 2 * (x + 0) * S / K) + ship_x/scale_factor;
            float x1 = (-S + 2 * (x + 1) * S / K) + ship_x/scale_factor;
            float z0 = (-S + 2 * (z + 0) * S / K) + ship_z/scale_factor;
            float z1 = (-S + 2 * (z + 1) * S / K) + ship_z/scale_factor;
            float motion = (float) SDL_GetTicks()/500;
            float y00 = (cos(x0 + motion) + sin(z0 + motion))*H;
            float y01 = (cos(x0 + motion) + sin(z1 + motion))*H;
            float y11 = (cos(x1 + motion) + sin(z1 + motion))*H;
            float y10 = (cos(x1 + motion) + sin(z0 + motion))*H;

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

            if (options->areWireframesEnabled()){
                glBegin(GL_LINE_LOOP);
            } else {
                glBegin(GL_TRIANGLES);
            }
            glVertex3d(x0, y00, z0);
            glVertex3d(x1, y10, z0);
            glVertex3d(x1, y11, z1);
            glEnd();

            if (options->areWireframesEnabled()){
                glBegin(GL_LINE_LOOP);
            } else {
                glBegin(GL_TRIANGLES);
            }
            glVertex3d(x1, y11, z1);
            glVertex3d(x0, y00, z0);
            glVertex3d(x0, y01, z1);
            glEnd();

        }
    }
    glPopMatrix();
}

void Enviroment::drawSky() {
    textureManager->enableTexture(TEXTURE_SKY);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); // Env map
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    GLint genCoords = glGetUniformLocation(shadowMapper->defaultPass.program, "u_genCoords");
    glUniform1i(genCoords, 2);
    glDisable(GL_LIGHTING);

    Utils::drawSphere(100.0, 20, 20, options->areWireframesEnabled());

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
    GLint colorOrTexture = glGetUniformLocation(shadowMapper->defaultPass.program, "u_colorOrTexture");
    GLint u_color = glGetUniformLocation(shadowMapper->defaultPass.program, "u_color");
    glUniform1i(colorOrTexture, 1);
    textureManager->enableTexture(TEXTURE_SEA);
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    // ulilizzo le coordinate OGGETTO
    // cioe' le coordnate originali, PRIMA della moltiplicazione per la ModelView
    // in modo che la texture sia "attaccata" all'oggetto, e non "proiettata" su esso
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float color[4] = {0, 0, 1, 0.5};
    glUniform4fv(u_color, 1, color);
    drawNearSea(ship_x, ship_y, ship_z);
    drawFarSea(ship_x, ship_y, ship_z);
    glUniform1i(colorOrTexture, 1);
    glDisable(GL_BLEND);
}