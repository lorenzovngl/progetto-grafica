#ifndef PROGETTOGRAFICA_ENVIROMENT_H
#define PROGETTOGRAFICA_ENVIROMENT_H

#include "Buoy.h"
#include "TextureManager.h"
#include "../../lib/ShadowMapper/ShadowMapper.h"
#include "Options.h"

#define BUOYS_COUNT 20

class Enviroment {
private:
    TextureManager *textureManager;
    ShadowMapper *shadowMapper;
    Options *options;
    Buoy *buoy[BUOYS_COUNT];
    float scale_factor;
public:
    Enviroment(TextureManager *textureManager, ShadowMapper *shadowMapper, Options *options);
    Buoy* getBuoy(int);
    int getBuoysCount();
    void renderBuoys();
    void drawNearSea(float, float, float);
    void drawFarSea(float, float, float);
    void drawSky();
    void render(float, float, float, bool texture_enabled);
};


#endif
