#ifndef PROGETTOGRAFICA_TEXTURE_H
#define PROGETTOGRAFICA_TEXTURE_H

#define TEXTURE_SEA 0
#define TEXTURE_SKY 1
#define TEXTURE_DECK_WOODEN_FLOOR 2
#define TEXTURE_TOP_BODY_BLACK_METAL 3

class Texture {
private:
    int m_textbind;
    char *m_filename;
public:
    Texture(int);
    bool loadTexture();
    int getBind();
};


#endif
