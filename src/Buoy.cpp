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

Buoy::Buoy(float x, float z){
    m_mesh = new Mesh((char *) "assets/sphere.obj");
    m_coord_x = x;
    m_coord_z = z;
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

void Buoy::render() {
    float scaleFactor[3] = {0.005, 0.005, 0.005};
    glScalef(scaleFactor[0], scaleFactor[1], scaleFactor[2]);
    glTranslatef(getCoordX(), 0, getCoordZ());
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
    glBegin(GL_TRIANGLES);
    // Bandierina
    int size = 40;
    glVertex3f(0, height, 0);
    glVertex3f(0, height-size, 0);
    glVertex3f(0, height-size/2, 50);

    // Boa
    m_mesh->render();
    m_mesh->ComputeBoundingBox(getCoordX()*scaleFactor[0], 0, getCoordZ()*scaleFactor[2], 0);
}