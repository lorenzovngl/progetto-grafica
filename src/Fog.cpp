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

#include "headers/Fog.h"

Fog::Fog() {
    color[0] = 0.7;
    color[1] = 0.7;
    color[2] = 0.7;
    color[3] = 1.0;
}

void Fog::render(){
    glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, color);
    glFogf(GL_FOG_START, 0.5);
    glFogf(GL_FOG_END, 2.0);
    glFogi(GL_FOG_MODE, GL_LINEAR);
}