#ifndef PROGETTOGRAFICA_FRONTIER_H
#define PROGETTOGRAFICA_FRONTIER_H

#include "ShaderParams.h"
#include "Options.h"

#define FRONTIER_LIMIT 64

class Frontier {
private:
    ShaderParams *mShaderParams;
    Options *mOptions;
public:
    Frontier(ShaderParams *shaderParams, Options *options);
    void render();
    bool isPointInBounds(float x, float y);
};


#endif
