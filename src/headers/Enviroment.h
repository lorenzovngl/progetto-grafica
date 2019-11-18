#ifndef PROGETTOGRAFICA_ENVIROMENT_H
#define PROGETTOGRAFICA_ENVIROMENT_H

#include "Buoy.h"
#include "TextureManager.h"

#define BUOYS_COUNT 10

class Enviroment {
private:
    TextureManager *textureManager;
    Buoy *buoy[BUOYS_COUNT];
    Texture *m_texture[2];
    void drawNearSea(float, float, float);
    void drawFarSea(float, float, float);
    void drawSky();
public:
    Enviroment(TextureManager *textureManager);
    Buoy* getBuoy(int);
    void render(float, float, float);
};


#endif
