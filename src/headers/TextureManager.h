#ifndef PROGETTOGRAFICA_TEXTUREMANAGER_H
#define PROGETTOGRAFICA_TEXTUREMANAGER_H

#include "Texture.h"

#define TEXTURE_SEA 0
#define TEXTURE_SKY 1

// Ship textures
#define TEXTURE_ANTENNE_METAL 2
#define TEXTURE_BOTTOM_BODY_DARK_STRIPE 3
#define TEXTURE_BOTTOM_BODY_DARK_RED_METAL 4
#define TEXTURE_DECK_WOODEN_FLOOR 5
#define TEXTURE_SHIP_CATWALK 6
#define TEXTURE_SUPERSTRUCTURE_BASE_STRIPE 7
#define TEXTURE_SUPERSTRUCTURE_GLASS 8
#define TEXTURE_SUPERSTRUCTURE_WHITE_METAL 9
#define TEXTURE_SUPERSTRUCTURE_WIRES 10
#define TEXTURE_TOP_BODY_BLACK_METAL 11
#define TEXTURE_TOP_BODY_WHITE_METAL 12

#define TEXTURE_FLAG_ITALY 13
#define TEXTURE_MY_PHOTO 15

#define COUNT_TEXTURES 20

class TextureManager {
private:
    Texture *textures[COUNT_TEXTURES];
public:
    bool loadTexture(int id);

    void enableTexture(int id);

    void printActiveTexture();
};


#endif
