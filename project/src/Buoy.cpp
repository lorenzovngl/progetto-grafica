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

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector> // la classe vector di SDL

#endif

#include "headers/Buoy.h"
#include "headers/Options.h"
#include "../lib/ShadowMapper/ShadowMapper.h"
#include "headers/Utils.h"

Buoy::Buoy(int id, float x, float z, TextureManager *textureManager, ShadowMapper *shadowMapper, ShaderParams* shaderParams, Options *options){
    m_mesh = new Mesh((char *) "assets/sphere.obj", shaderParams, options);
    this->active = true;
    m_coord_x = x;
    m_coord_z = z;
    this->id = id;
    this->textureManager = textureManager;
    this->shadowMapper = shadowMapper;
    this->mShaderParams = shaderParams;
    this->options = options;
    textureManager->loadTexture(TEXTURE_FLAG_ITALY);
}

Mesh* Buoy::getMesh(){
    return m_mesh;
}

float Buoy::getCoordX() {
    return m_coord_x;
}

float Buoy::getCoordZ() {
    return m_coord_z;
}

bool Buoy::isActive(){
    return this->active;
}

void Buoy::disable(){
    this->active = false;
}

void Buoy::render() {
    float scaleFactor = 0.005;
    float motion = (float) SDL_GetTicks()/500 + this->id;
    const float H = 0.05;
    glTranslatef(getCoordX(), sin(motion)*H - H/2, getCoordZ());
    glScalef(scaleFactor, scaleFactor, scaleFactor);

    // Asta della boa
    int base = 2;
    int height = 100;
    if (options->areWireframesEnabled()){
        glBegin(GL_LINE_LOOP);
    } else {
        glBegin(GL_QUADS);
    }
    float color[4] = {1, 0, 0, 1};
    mShaderParams->setParam(mShaderParams->colorOrTextureParam, COLOR_MODE);
    mShaderParams->setParam(mShaderParams->colorParam, color);
    glColor3f(1, 0, 0);
    // Parallelepipedo
    glVertex3f(-(base/2),0,(base/2));
    glVertex3f(-(base/2),height,(base/2));
    glVertex3f((base/2), height,(base/2));
    glVertex3f((base/2),0,(base/2));

    glVertex3f(-(base/2),0,-(base/2));
    glVertex3f(-(base/2),height,-(base/2));
    glVertex3f((base/2), height,-(base/2));
    glVertex3f((base/2),0,-(base/2));

    glVertex3f((base/2),0,-(base/2));
    glVertex3f((base/2),height,-(base/2));
    glVertex3f((base/2), height,(base/2));
    glVertex3f((base/2),0,(base/2));

    glVertex3f(-(base/2),0,-(base/2));
    glVertex3f(-(base/2),height,-(base/2));
    glVertex3f(-(base/2), height,(base/2));
    glVertex3f(-(base/2),0,(base/2));

    glVertex3f((base/2),height,(base/2));
    glVertex3f(-(base/2),height,(base/2));
    glVertex3f(-(base/2), height,-(base/2));
    glVertex3f((base/2),height,-(base/2));

    glEnd();

    // Boa
    Utils::drawSphere(20, 20, 20, options->areWireframesEnabled());
    m_mesh->ComputeBoundingBox(getCoordX(), 0, getCoordZ(), scaleFactor, 0);

    // Simulo direzione del vento
    glRotatef(45, 0, 1, 0);

    mShaderParams->setParam(mShaderParams->colorOrTextureParam, TEXTURE_MODE);
    textureManager->enableTexture(TEXTURE_FLAG_ITALY);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);

    // Bandierina
    mShaderParams->setParam(mShaderParams->genCoordsParam, EXPLICIT_COORDS);
    float flag_height = 40;
    float flag_width = 50;
    float pieces = 5;
    float speed = 2;
    float x0, y0, z0, x1, y1, z1;
    for (int i = 0; i < pieces; i++){
        x0 = flag_height / pieces * i;
        z0 = flag_width / pieces * i;
        x1 = flag_height / pieces * (i+1);
        z1 = flag_width / pieces * (i+1);
        for (int j = 0; j < pieces; j++){
            y0 = height - (flag_height / pieces * j);
            y1 = height - (flag_height / pieces * (j+1));
            if (options->areWireframesEnabled()){
                glBegin(GL_LINE_LOOP);
            } else {
                glBegin(GL_QUADS);
            }
            if (i == 0){
                glTexCoord2f(1/pieces*i, 1/pieces*i);
                glVertex3f(x0, y0, z0);
                glTexCoord2f(1/pieces*i, 1/pieces*(i+1));
                glVertex3f(x0, y1, z0);
            } else {
                glTexCoord2f(1/pieces*i, 1/pieces*i);
                glVertex3f(sin(x0+motion)*speed, y0, z0);
                glTexCoord2f(1/pieces*i, 1/pieces*(i+1));
                glVertex3f(sin(x0+motion)*speed, y1, z0);
            }
            glTexCoord2f(1/pieces*(i+1), 1/pieces*(i+1));
            glVertex3f(sin(x1+motion)*speed, y1, z1);
            glTexCoord2f(1/pieces*(i+1), 1/pieces*i);
            glVertex3f(sin(x1+motion)*speed, y0, z1);
            glEnd();
        }
    }
    glDisable(GL_TEXTURE_2D);
}