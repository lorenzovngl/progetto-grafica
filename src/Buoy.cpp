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

#include "headers/Buoy.h"
#include "headers/Utils.h"

Buoy::Buoy(int id, float x, float z){
    m_mesh = new Mesh((char *) "assets/sphere.obj");
    this->active = true;
    m_coord_x = x;
    m_coord_z = z;
    this->id = id;
    glActiveTexture(GL_TEXTURE7);
    Texture *texture = new Texture(TEXTURE_FLAG_ITALY);
    texture->loadTexture();
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
    //Utils::drawAxis();
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    // Asta della boa
    int base = 2;
    int height = 100;
    glBegin(GL_QUADS);
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
    m_mesh->render();
    m_mesh->ComputeBoundingBox(getCoordX(), 0, getCoordZ(), scaleFactor, 0);

    // Simulo direzione del vento
    glRotatef(45, 0, 1, 0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    glBegin(GL_QUADS);
    // Bandierina
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
            if ((i+j) % 2 == 0){
                glColor3f(0, 0, 0);
            } else {
                glColor3f(1, 1, 1);
            }
            /*if (i != 0){
                x0 = sin(x0+motion)*speed;
            }*/
            y0 = height - (flag_height / pieces * j);
            y1 = height - (flag_height / pieces * (j+1));
            if (i == 0){
                glTexCoord2f(0, 0);
                glVertex3f(x0, y0, z0);
                glTexCoord2f(0, 1);
                glVertex3f(x0, y1, z0);
            } else {
                glTexCoord2f(0, 0);
                glVertex3f(sin(x0+motion)*speed, y0, z0);
                glTexCoord2f(0, 1);
                glVertex3f(sin(x0+motion)*speed, y1, z0);
            }
            glTexCoord2f(1, 1);
            glVertex3f(sin(x1+motion)*speed, y1, z1);
            glTexCoord2f(1, 0);
            glVertex3f(sin(x1+motion)*speed, y0, z1);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}