#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif

#include "headers/ShaderParams.h"

ShaderParams::ShaderParams(ShadowMapper *shadowMapper) {
    mShadowMapper = shadowMapper;
    strcpy(colorParam, "u_color");
    strcpy(colorOrTextureParam, "u_colorOrTexture");
    strcpy(genCoordsParam, "u_genCoords");
    strcpy(overlay, "u_overlay");
}

void ShaderParams::setParam(char *param, int value) {
    GLint paramLoc = glGetUniformLocation(mShadowMapper->defaultPass.program, param);
    glUniform1i(paramLoc, value);
}

void ShaderParams::setParam(char *param, float value[]) {
    GLint paramLoc = glGetUniformLocation(mShadowMapper->defaultPass.program, param);
    glUniform4fv(paramLoc, 1, value);
}