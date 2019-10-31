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

Texture::Texture(int textbind) {
    m_textbind = textbind;
    switch (textbind){
        case TEXTURE_BOTTOM_BODY_DARK_RED_METAL:
            m_filename = (char*) "assets/ship/bottom_body_dark_red_metal.tga";
            break;
        case TEXTURE_DECK_WOODEN_FLOOR:
            m_filename = (char*) "assets/ship/deck_wooden_floor.tga";
            break;
        case TEXTURE_SEA:
            m_filename = (char*) "assets/sea.jpg";
            break;
        case TEXTURE_SKY:
            m_filename = (char*) "assets/sky.jpg";
            break;
        case TEXTURE_TOP_BODY_BLACK_METAL:
            m_filename = (char*) "assets/ship/top_body_black_metal.tga";
            break;
        case TEXTURE_TOP_BODY_WHITE_METAL:
            m_filename = (char*) "assets/ship/top_body_white_metal.tga";
            break;
        default:
            break;
    }
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