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

#include "headers/BoundingBox.h"
#include "headers/point3.h"
#include "headers/Utils.h"

BoundingBox::BoundingBox(Point3 bbmin, Point3 bbmax){
    for(int i = 0; i < 8; i++){
        this->vertex[i] = new Point3();
    }
    this->vertex[0]->coord[0] = bbmax.X();
    this->vertex[0]->coord[1] = bbmax.Y();
    this->vertex[0]->coord[2] = bbmax.Z();

    this->vertex[1]->coord[0] = bbmax.X();
    this->vertex[1]->coord[1] = bbmax.Y();
    this->vertex[1]->coord[2] = bbmin.Z();

    this->vertex[2]->coord[0] = bbmax.X();
    this->vertex[2]->coord[1] = bbmin.Y();
    this->vertex[2]->coord[2] = bbmax.Z();

    this->vertex[3]->coord[0] = bbmax.X();
    this->vertex[3]->coord[1] = bbmin.Y();
    this->vertex[3]->coord[2] = bbmin.Z();

    this->vertex[4]->coord[0] = bbmin.X();
    this->vertex[4]->coord[1] = bbmax.Y();
    this->vertex[4]->coord[2] = bbmax.Z();

    this->vertex[5]->coord[0] = bbmin.X();
    this->vertex[5]->coord[1] = bbmax.Y();
    this->vertex[5]->coord[2] = bbmin.Z();

    this->vertex[6]->coord[0] = bbmin.X();
    this->vertex[6]->coord[1] = bbmin.Y();
    this->vertex[6]->coord[2] = bbmax.Z();

    this->vertex[7]->coord[0] = bbmin.X();
    this->vertex[7]->coord[1] = bbmin.Y();
    this->vertex[7]->coord[2] = bbmin.Z();
}

void BoundingBox::localToWorldCoords(Point3 worldCenter, float scale_factor, float angle) {
    for (int i = 0; i < 8; i++){
        *(this->vertex[i]) = *(this->vertex[i]) * scale_factor + worldCenter;
        *this->vertex[i] = Utils::rotate_point(worldCenter.X(), worldCenter.Z(), -angle*scale_factor*0.35, *this->vertex[i]);
        // Non ho la minima idea del perché 0.35 aggiusti le cose...
    }
}

void BoundingBox::display(){
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);

    glVertex3fv(this->vertex[0]->coord);
    glVertex3fv(this->vertex[1]->coord);

    glVertex3fv(this->vertex[1]->coord);
    glVertex3fv(this->vertex[3]->coord);

    glVertex3fv(this->vertex[3]->coord);
    glVertex3fv(this->vertex[2]->coord);

    glVertex3fv(this->vertex[2]->coord);
    glVertex3fv(this->vertex[0]->coord);


    glVertex3fv(this->vertex[4]->coord);
    glVertex3fv(this->vertex[5]->coord);

    glVertex3fv(this->vertex[5]->coord);
    glVertex3fv(this->vertex[7]->coord);

    glVertex3fv(this->vertex[7]->coord);
    glVertex3fv(this->vertex[6]->coord);

    glVertex3fv(this->vertex[6]->coord);
    glVertex3fv(this->vertex[4]->coord);


    glVertex3fv(this->vertex[0]->coord);
    glVertex3fv(this->vertex[4]->coord);

    glVertex3fv(this->vertex[1]->coord);
    glVertex3fv(this->vertex[5]->coord);

    glVertex3fv(this->vertex[2]->coord);
    glVertex3fv(this->vertex[6]->coord);

    glVertex3fv(this->vertex[3]->coord);
    glVertex3fv(this->vertex[7]->coord);

    glEnd();
}