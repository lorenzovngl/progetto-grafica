#ifndef PROGETTOGRAFICA_ENVIROMENT_H
#define PROGETTOGRAFICA_ENVIROMENT_H

#include "Buoy.h"

class Enviroment {
private:
    Buoy *buoy[10];
    bool isActiveBuoy[10];
    Texture *m_texture[2];
    void drawNearSea(float, float, float);
    void drawFarSea(float, float, float);
    void drawSky();
public:
    Enviroment();
    Buoy* getBuoy(int);
    void disableBuoy(int);
    void render(float, float, float);
};


#endif
