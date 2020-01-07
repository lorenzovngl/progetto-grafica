#ifndef PROGETTOGRAFICA_BOUNDINGBOX_H
#define PROGETTOGRAFICA_BOUNDINGBOX_H

#include "point3.h"

class BoundingBox {
public:
    Point3 *vertex[8];

    BoundingBox(Point3 bbmin, Point3 bbmax);

    // Converte le coordinate locali in quelle del mondo per il collision detection
    void localToWorldCoords(Point3 worldCenter, float scale_factor, float angle);

    void display();

    static bool isCollision(BoundingBox *a, BoundingBox *b);
};

#endif