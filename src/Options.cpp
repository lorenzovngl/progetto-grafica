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

#include "headers/Options.h"

Options::Options() {
    wireframesEnable = false;
    shadowsEnable = true;
    shadersEnable = true;
}

bool Options::areWireframesEnabled(){
    return wireframesEnable;
}

bool Options::areShadowsEnabled(){
    return shadowsEnable;
}

bool Options::areShadersEnabled(){
    return shadersEnable;
}

void Options::toggleWireframes(){
    wireframesEnable = !wireframesEnable;
}

void Options::toggleShadows(){
    shadowsEnable = !shadowsEnable;
}

void Options::toggleShaders(){
    shadersEnable = !shadersEnable;
}

void Options::printMenu(){
    printf("F1 - Change camera\n"
           "F2 - Toggle wireframes\n"
           "F3 - Toggle shadows\n"
           "F4 - Toggle advance rendering (shaders)\n");
}