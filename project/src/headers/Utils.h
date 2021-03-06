#ifndef PROGETTOGRAFICA_UTILS_H
#define PROGETTOGRAFICA_UTILS_H

#include "point3.h"

class Utils {
public:
    static void drawAxis();

    static void drawTranslatedAxis(float x, float y, float z);

    static void drawSphere(double, int, int, bool enableWireframe);

    static float floatMod(float, float);

    static void setCoordToPixel(int, int);

    static Point3 rotate_point(float cx, float cz, float angle, Point3 p);

    static void glShadowProjection(float *l, float *e, float *n);

    static void loadFileIntoCharArray(char *dest, char *filepath);
};

#endif