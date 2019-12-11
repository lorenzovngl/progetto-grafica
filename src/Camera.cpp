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

#include "headers/Camera.h"
#include "headers/Ship.h"

Camera::Camera(){
    m_type = CAMERA_MOUSE_SHIP;
}

int Camera::getType(){
    return m_type;
}

void Camera::change(Ship ship, float eyeDist, float viewBeta, float viewAlpha) {
    m_type = (m_type+1) % CAMERA_TYPE_MAX;
    set(ship, eyeDist, viewBeta, viewAlpha);
}

void Camera::set(Ship ship, float eyeDist, float viewBeta, float viewAlpha) {
    double px = ship.px;
    double py = ship.py;
    double pz = ship.pz;
    double angle = ship.facing;
    double cosf = cos(angle*M_PI/180.0);
    double sinf = sin(angle*M_PI/180.0);
    double camd, camh, ex, ey, ez, cx, cy, cz;
    double cosff, sinff;

    ex = cx = 0;
    ey = cy = 0;
    ez = cz = 0;

    glPushMatrix();
    glLoadIdentity();

    switch (m_type) {
        case CAMERA_BACK_SHIP:
            camd = 4.0;
            camh = 1.5;
            ex = px + camd*sinf;
            ey = py + camh;
            ez = pz + camd*cosf;
            cx = px - camd*sinf;
            cy = py + camh;
            cz = pz - camd*cosf;
            gluLookAt(ex,ey,ez,cx,cy,cz,0.0,1.0,0.0);
            break;
        case CAMERA_PILOT:
            glTranslatef(0.0, 0.0, 0.5);
            camd = 0.2;
            camh = 0.40;
            ex = px + camd*sinf;
            ey = py + camh;
            ez = pz + camd*cosf;
            cx = px - camd*sinf;
            cy = py + camh;
            cz = pz - camd*cosf;
            gluLookAt(ex,ey,ez,cx,cy,cz,0.0,1.0,0.0);
            break;
        case CAMERA_MOUSE:
            glTranslatef(0, 0, -eyeDist);
            glRotatef(viewBeta,  1,0,0);
            glRotatef(viewAlpha, 0,-1,0);
            break;
        case CAMERA_MOUSE_SHIP:
            glTranslatef(0, 0, -eyeDist);
            glRotatef(viewBeta,1,0,0);
            glRotatef(viewAlpha, 0,1,0);
            camd = 0.2;
            camh = 0.40;
            ex = px + camd*sinf;
            ey = py + camh;
            ez = pz + camd*cosf;
            cx = px - camd*sinf;
            cy = py + camh;
            cz = pz - camd*cosf;
            gluLookAt(ex,ey,ez,cx,cy,cz,0.0,1.0,0.0);
            break;
    }
    GLfloat m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    for (int i = 0; i < 4; i++){
        viewMatrix[i] = m[i];
        viewMatrix[4+i] = m[4+i];
        viewMatrix[8+i] = m[8+i];
        viewMatrix[12+i] = m[12+i];
    }
    cameraPos[0] = (float) ex;
    cameraPos[1] = (float) ey;
    cameraPos[2] = (float) ez;
    targetPos[0] = (float) cx;
    targetPos[1] = (float) cy;
    targetPos[2] = (float) cz;
    glPopMatrix();
}