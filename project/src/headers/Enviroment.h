#ifndef PROGETTOGRAFICA_ENVIROMENT_H
#define PROGETTOGRAFICA_ENVIROMENT_H

#include "Buoy.h"
#include "TextureManager.h"
#include "../../lib/ShadowMapper/ShadowMapper.h"
#include "Options.h"

#define DEFAULT_BUOYS_COUNT 10
#define BUOYS_COUNT_MAX 100

class Enviroment {
private:
    TextureManager *textureManager;
    ShadowMapper *shadowMapper;
    ShaderParams *mShaderParams;
    Options *options;
    Buoy *buoy[BUOYS_COUNT_MAX];
    float scale_factor;
public:
    int buoysCount;

    Enviroment(TextureManager *textureManager, ShadowMapper *shadowMapper, ShaderParams *shaderParams,
               Options *options);

    Buoy *getBuoy(int);

    int getBuoysCount();

    void renderBuoys();

    void drawNearSea(float, float, float);

    void drawFarSea(float, float, float);

    void drawSky();

    void renderSea(float, float, float, bool texture_enabled);

    void reset();
};


#endif
