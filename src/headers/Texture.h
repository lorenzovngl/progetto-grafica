#ifndef PROGETTOGRAFICA_TEXTURE_H
#define PROGETTOGRAFICA_TEXTURE_H

class Texture {
private:
    int m_textbind;
public:
    char *filename;

    Texture(int);

    bool loadTexture();

    int getBind();

    static void printActiveTexture();
};


#endif
