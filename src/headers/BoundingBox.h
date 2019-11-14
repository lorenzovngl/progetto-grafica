#ifndef PROGETTOGRAFICA_BOUNDINGBOX_H
#define PROGETTOGRAFICA_BOUNDINGBOX_H

#include "point3.h"

#define VERTEX_X_Y_Z = 0            // Vertice (+X, +Y, +Z)
#define VERTEX_X_Y_mZ = 1           // Vertice (+X, +Y, -Z)
#define VERTEX_X_mY_Z = 2           // Vertice (+X, -Y, +Z)
#define VERTEX_X_mY_mZ = 3          // Vertice (+X, -Y, -Z)
#define VERTEX_mX_Y_Z = 4           // Vertice (-X, +Y, +Z)
#define VERTEX_mX_Y_mZ = 5          // Vertice (-X, +Y, -Z)
#define VERTEX_mX_mY_Z = 6          // Vertice (-X, -Y, +Z)
#define VERTEX_mX_mY_mZ = 7         // Vertice (-X, -Y, -Z)

class BoundingBox {
public:
    Point3 *vertex[8];
    BoundingBox(Point3 bbmin, Point3 bbmax);
    // Converte le coordinate locali in quelle del mondo per il collision detection
    void localToWorldCoords(Point3 worldCenter, float scale_factor, float angle);
    void display();
    static bool isCollision(BoundingBox* a, BoundingBox* b);
};

#endif