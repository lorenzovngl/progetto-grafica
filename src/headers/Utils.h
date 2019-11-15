#ifndef PROGETTOGRAFICA_UTILS_H
#define PROGETTOGRAFICA_UTILS_H

#include "point3.h"

class Utils {
public:
    static void drawAxis();
    static void drawSphere(double, int, int);
    static float floatMod(float, float);
    static void setCoordToPixel(int, int);
    static Point3 rotate_point(float cx, float cz, float angle, Point3 p);
};

#endif