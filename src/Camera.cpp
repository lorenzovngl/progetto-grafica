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
#include "headers/Utils.h"

Camera::Camera(){
    m_type = CAMERA_MOUSE;
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

// controllo la posizione della camera a seconda dell'opzione selezionata
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
            glTranslatef(0,0,-eyeDist);
            glRotatef(viewBeta,  1,0,0);
            glRotatef(viewAlpha, 0,1,0);
/*
printf("%f %f %f\n",viewAlpha,viewBeta,eyeDist);
                ex=eyeDist*cos(viewAlpha)*sin(viewBeta);
                ey=eyeDist*sin(viewAlpha)*sin(viewBeta);
                ez=eyeDist*cos(viewBeta);
                cx = px - camd*sinf;
                cy = py + camh;
                cz = pz - camd*cosf;
                gluLookAt(ex,ey,ez,cx,cy,cz,0.0,1.0,0.0);
*/
            break;
    }
}