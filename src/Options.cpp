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
    wireframesEnabled = false;
    shadowsEnabled = true;
    fogEnabled = true;
    shadersEnabled = true;
}

bool Options::areWireframesEnabled(){
    return wireframesEnabled;
}

bool Options::areShadowsEnabled(){
    return shadowsEnabled;
}

bool Options::isFogEnabled(){
    return fogEnabled;
}

bool Options::areShadersEnabled(){
    return shadersEnabled;
}

void Options::toggleWireframes(){
    wireframesEnabled = !wireframesEnabled;
}

void Options::toggleShadows(){
    shadowsEnabled = !shadowsEnabled;
}

void Options::toggleFog(){
    fogEnabled = !fogEnabled;
}

void Options::toggleShaders(){
    shadersEnabled = !shadersEnabled;
}

void Options::printMenu(){
    printf("F1 - Change camera\n"
           "F2 - Toggle wireframes\n"
           "F3 - Toggle shadows\n"
           "F4 - Toggle fog\n"
           "F5 - Toggle advance rendering (shaders)\n");
}