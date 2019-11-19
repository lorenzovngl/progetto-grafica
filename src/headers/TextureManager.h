#ifndef PROGETTOGRAFICA_TEXTUREMANAGER_H
#define PROGETTOGRAFICA_TEXTUREMANAGER_H

#include "Texture.h"

#define COUNT_TEXTURES 20

#define TEXTURE_SEA 0
#define TEXTURE_SKY 1
#define TEXTURE_DECK_WOODEN_FLOOR 2
#define TEXTURE_TOP_BODY_BLACK_METAL 3
#define TEXTURE_BOTTOM_BODY_DARK_RED_METAL 4
#define TEXTURE_TOP_BODY_WHITE_METAL 5
#define TEXTURE_FLAG_ITALY 6

class TextureManager {
private:
    Texture *textures[COUNT_TEXTURES];
public:
    bool loadTexture(int id);
    void enableTexture(int id);
    void printActiveTexture();
};


#endif
