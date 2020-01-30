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

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif

#include "headers/Frontier.h"
#include "headers/ShaderParams.h"
#include "headers/Utils.h"
#include "headers/Options.h"

Frontier::Frontier(ShaderParams *shaderParams, Options *options){
    mShaderParams = shaderParams;
    mOptions = options;
}

void Frontier::render(){
    mShaderParams->setParam(mShaderParams->colorOrTextureParam, COLOR_MODE);
    float color[4] = {1, 1, 1, 1};
    mShaderParams->setParam(mShaderParams->colorParam, color);
    int count = 100;
    float radius = 65;
    float angle_step = 2*M_PI / count;
    for (int i = 0; i < count; i++){
        float angle = (float) angle_step * i;
        glPushMatrix();
        glTranslatef(radius*cos(angle), 0, radius*sin(angle));
        Utils::drawSphere(0.2, 20, 20, mOptions->areWireframesEnabled());
        glPopMatrix();
    }
}

bool Frontier::isPointInBounds(float x, float y){
    return pow(x, 2) + pow(y, 2) < pow(FRONTIER_LIMIT, 2);
}