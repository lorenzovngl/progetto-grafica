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

#endif

#include "headers/Utils.h"

void Utils::drawAxis(){
    const float K=0.10;
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);

    glColor3f(0,0,1);   // Asse X
    glVertex3f( -1,0,0 );
    glVertex3f( +1,0,0 );

    glColor3f(1,0,0);   // Asse Y
    glVertex3f( 0,-1,0 );
    glVertex3f( 0,+1,0 );

    glColor3f(0,1,0);   // Asse Z
    glVertex3f( 0,0,-1 );
    glVertex3f( 0,0,+1 );
    glEnd();

    glBegin(GL_TRIANGLES);

    glColor3f(0,0,1);   // Asse X
    glVertex3f( +1,   0, 0 );
    glVertex3f( +1-K,+K, 0 );
    glVertex3f( +1-K,-K, 0 );

    glColor3f(1,0,0);   // Asse Y
    glVertex3f( 0,+1  ,0 );
    glVertex3f( K,+1-K,0 );
    glVertex3f(-K,+1-K,0 );

    glColor3f(0,1,0);   // Asse Z
    glVertex3f( 0, 0,+1 );
    glVertex3f( 0,+K,+1-K );
    glVertex3f( 0,-K,+1-K );
    glEnd();
    glEnable(GL_LIGHTING);
}

void Utils::drawSphere(double r, int lats, int longs) {
    int i, j;
    for(i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0  = sin(lat0);
        double zr0 =  cos(lat0);

        double lat1 = M_PI * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            //le normali servono per l'EnvMap
            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(r * x * zr0, r * y * zr0, r * z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(r * x * zr1, r * y * zr1, r * z1);
        }
        glEnd();
    }
}

float Utils::floatMod(float number, float divisor){
    while (number >= divisor){
        number -= divisor;
    }
}

void Utils::setCoordToPixel(int v_width, int v_height){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-1,-1,0);
    glScalef(2.5/v_width, 2.5/v_height, 1);
}

Point3 Utils::rotate_point(float cx, float cz, float angle, Point3 p){
    float s = sin(angle);
    float c = cos(angle);

    // translate point back to origin:
    p.coord[0] -= cx;
    p.coord[2] -= cz;

    // rotate point
    float xnew = p.X() * c - p.Z() * s;
    float ynew = p.X() * s + p.Z() * c;

    // translate point back:
    p.coord[0] = xnew + cx;
    p.coord[2] = ynew + cz;
    return p;
}