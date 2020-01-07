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

bool BoundingBox::isCollision(BoundingBox* a, BoundingBox* b) {
    float a_bbminX = a->vertex[7]->coord[0];
    float a_bbminY = a->vertex[7]->coord[1];
    float a_bbminZ = a->vertex[7]->coord[2];
    float a_bbmaxX = a->vertex[0]->coord[0];
    float a_bbmaxY = a->vertex[0]->coord[1];
    float a_bbmaxZ = a->vertex[0]->coord[2];
    float b_bbminX = b->vertex[7]->coord[0];
    float b_bbminY = b->vertex[7]->coord[1];
    float b_bbminZ = b->vertex[7]->coord[2];
    float b_bbmaxX = b->vertex[0]->coord[0];
    float b_bbmaxY = b->vertex[0]->coord[1];
    float b_bbmaxZ = b->vertex[0]->coord[2];
    // Non è detto che quelli che sono indicati con min e max siano ordinati visti i movimenti che può fare la mesh
    float a_minX = (a_bbminX < a_bbmaxX) ? a_bbminX : a_bbmaxX;
    float a_minY = (a_bbminY < a_bbmaxY) ? a_bbminY : a_bbmaxY;
    float a_minZ = (a_bbminZ < a_bbmaxZ) ? a_bbminZ : a_bbmaxZ;
    float a_maxX = (a_bbminX < a_bbmaxX) ? a_bbmaxX : a_bbminX;
    float a_maxY = (a_bbminY < a_bbmaxY) ? a_bbmaxY : a_bbminY;
    float a_maxZ = (a_bbminZ < a_bbmaxZ) ? a_bbmaxZ : a_bbminZ;
    float b_minX = (b_bbminX < b_bbmaxX) ? b_bbminX : b_bbmaxX;
    float b_minY = (b_bbminY < b_bbmaxY) ? b_bbminY : b_bbmaxY;
    float b_minZ = (b_bbminZ < b_bbmaxZ) ? b_bbminZ : b_bbmaxZ;
    float b_maxX = (b_bbminX < b_bbmaxX) ? b_bbmaxX : b_bbminX;
    float b_maxY = (b_bbminY < b_bbmaxY) ? b_bbmaxY : b_bbminY;
    float b_maxZ = (b_bbminZ < b_bbmaxZ) ? b_bbmaxZ : b_bbminZ;
    /* Partiamo dal caso più semplice ad una dimensione:
     * Due segmenti A e B non si sovrappongono sse 
     * i due estremi di A sono entrambi minori dei due estremi di B o viceversa,
     * ovvero se il massimo di A è minore del massimo di B o viceversa */
    bool overlapX = !(a_maxX < b_minX || b_maxX < a_minX);
    bool overlapY = !(a_maxY < b_minY || b_maxY < a_minY);
    bool overlapZ = !(a_maxZ < b_minZ || b_maxZ < a_minZ);
    /*printf("\nA: (%f %f %d), (%f %f %d), (%f %f %d); B: (%f %f), (%f %f), (%f %f);",
           a_minX, a_maxX, overlapX, a_minY, a_maxY, overlapY, a_minZ, a_maxZ, overlapZ,
           b_minX, b_maxX, b_minY, b_maxY, b_minZ, b_maxZ);*/
    // Il caso a tre dimensioni è l'intersezione dei casi semplici per X, Y e Z.
    return overlapX && overlapY && overlapZ;
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