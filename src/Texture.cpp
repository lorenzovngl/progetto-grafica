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
#include <vector> // la classe vector di SDL

#endif

#include "headers/Texture.h"

Texture::Texture(int textbind, char *filename) {
    m_textbind = textbind;
    m_filename = filename;
}

bool Texture::loadTexture(){
    GLenum texture_format;

    SDL_Surface *s = IMG_Load(m_filename);
    if (!s) return false;

    if (s->format->BytesPerPixel == 4){     // contiene canale alpha
        if (s->format->Rmask == 0x000000ff){
            texture_format = GL_RGBA;
        }
        else{
            texture_format = GL_BGRA;
        }
    } else if (s->format->BytesPerPixel == 3){     // non contiene canale alpha
        if (s->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    } else {
        printf("[ERROR] the image is not truecolor\n");
        exit(1);
    }

    glBindTexture(GL_TEXTURE_2D, m_textbind);
    gluBuild2DMipmaps(
            GL_TEXTURE_2D,
            3,
            s->w, s->h,
            texture_format,
            GL_UNSIGNED_BYTE,
            s->pixels
    );
    glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR );
    glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR );
    return true;
}

int Texture::getBind() {
    return m_textbind;
}