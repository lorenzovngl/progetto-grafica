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

#include "headers/TextureManager.h"

bool TextureManager::loadTexture(int id){
    textures[id] = new Texture(id);
    switch (id){
        case TEXTURE_SEA:
            textures[id]->filename = (char*) "assets/sea.jpg";
            break;
        case TEXTURE_SKY:
            textures[id]->filename = (char*) "assets/sky.jpg";
            break;
        case TEXTURE_ANTENNE_METAL:
            textures[id]->filename = (char*) "assets/ship/antenne_metal.tga";
            break;
        case TEXTURE_BOTTOM_BODY_DARK_STRIPE:
            textures[id]->filename = (char*) "assets/ship/bottom_body_dark_stripe.tga";
            break;
        case TEXTURE_BOTTOM_BODY_DARK_RED_METAL:
            textures[id]->filename = (char*) "assets/ship/bottom_body_dark_red_metal.tga";
            break;
        case TEXTURE_DECK_WOODEN_FLOOR:
            textures[id]->filename = (char*) "assets/ship/deck_wooden_floor.tga";
            break;
        case TEXTURE_SHIP_CATWALK:
            textures[id]->filename = (char*) "assets/ship/ship_catwalk.tga";
            break;
        case TEXTURE_SUPERSTRUCTURE_BASE_STRIPE:
            textures[id]->filename = (char*) "assets/ship/superstructure_base_stripe.tga";
            break;
        case TEXTURE_SUPERSTRUCTURE_GLASS:
            textures[id]->filename = (char*) "assets/ship/superstructure_glass.tga";
            break;
        case TEXTURE_SUPERSTRUCTURE_WHITE_METAL:
            textures[id]->filename = (char*) "assets/ship/superstructure_white_metal.tga";
            break;
        case TEXTURE_SUPERSTRUCTURE_WIRES:
            textures[id]->filename = (char*) "assets/ship/superstructure_wires.tga";
            break;
        case TEXTURE_TOP_BODY_BLACK_METAL:
            textures[id]->filename = (char*) "assets/ship/top_body_black_metal.tga";
            break;
        case TEXTURE_TOP_BODY_WHITE_METAL:
            textures[id]->filename = (char*) "assets/ship/top_body_white_metal.tga";
            break;
        case TEXTURE_FLAG_ITALY:
            textures[id]->filename = (char*) "assets/flags/italy.png";
            break;
        case TEXTURE_MY_PHOTO:
            textures[id]->filename = (char*) "assets/my_photo.jpg";
            break;
        default:
            break;
    }
    GLenum texture_format;
    SDL_Surface *s = IMG_Load(textures[id]->filename);
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
    glBindTexture(GL_TEXTURE_2D, id);
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

void TextureManager::enableTexture(int id) {
    glBindTexture(GL_TEXTURE_2D, id);
}

void TextureManager::printActiveTexture(){
    GLint units[7] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5, GL_TEXTURE6};
    char *str_units[7] = {(char*) "GL_TEXTURE0", (char*) "GL_TEXTURE1", (char*) "GL_TEXTURE2", (char*) "GL_TEXTURE3",
                          (char*) "GL_TEXTURE4", (char*) "GL_TEXTURE5", (char*) "GL_TEXTURE6"};
    GLint param;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &param);
    int i = 0;
    while (param != units[i]){
        i++;
    }
    printf("Active texture: %s\n", (char*) str_units[i]);
}