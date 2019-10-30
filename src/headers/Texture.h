#ifndef PROGETTOGRAFICA_TEXTURE_H
#define PROGETTOGRAFICA_TEXTURE_H

#define TEXTURE_SEA 0
#define TEXTURE_SKY 1
#define TEXTURE_SHIP 2

class Texture {
private:
    int m_textbind;
    char *m_filename;
public:
    Texture(int, char*);
    bool loadTexture();
    int getBind();
};


#endif
