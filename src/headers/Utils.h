#ifndef PROGETTOGRAFICA_UTILS_H
#define PROGETTOGRAFICA_UTILS_H

#include "point3.h"

class Utils {
public:
    static void drawAxis();
    static void drawSphere(double, int, int);
    static float floatMod(float, float);
    static void setCoordToPixel();
    // Converte le coordinate locali in quelle del mondo per il collision detection
    static Point3 localToWorldCoords(Point3 localCoords, Point3 center, float angle);
};

#endif