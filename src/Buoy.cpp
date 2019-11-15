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

    glActiveTexture(GL_TEXTURE7);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    // Bandierina
    int size = 40;
    glTexCoord2f(0, 0);
    glVertex3f(0, height, 0);
    glTexCoord2f(0, 1);
    glVertex3f(0, height-size, 0);
    glTexCoord2f(1, 1);
    glVertex3f(sin(motion)*50, height-size, cos(motion)*50);
    glTexCoord2f(1, 0);
    glVertex3f(sin(motion)*50, height, cos(motion)*50);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // Boa
    m_mesh->render();
    m_mesh->ComputeBoundingBox(getCoordX(), 0, getCoordZ(), scaleFactor, 0);
}