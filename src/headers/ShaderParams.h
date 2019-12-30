#ifndef PROGETTOGRAFICA_SHADERPARAMS_H
#define PROGETTOGRAFICA_SHADERPARAMS_H

#include <GL/gl.h>
#include "../../lib/ShadowMapper/ShadowMapper.h"

// GenCoords
#define EXPLICIT_COORDS 0
#define OBJECT_LINEAR 1
#define SPHERE_MAP 2

// ColorOrTexture
#define COLOR_MODE 0
#define TEXTURE_MODE 1

#define TRUE 1
#define FALSE 0

class ShaderParams {
private:
    ShadowMapper* mShadowMapper;
public:
    char colorParam[20];
    char colorOrTextureParam[20];
    char genCoordsParam[20];
    char overlay[20];
    ShaderParams(ShadowMapper* shadowMapper);
    void setParam(char* param, int value);
    void setParam(char* param, float value[]);
};


#endif
