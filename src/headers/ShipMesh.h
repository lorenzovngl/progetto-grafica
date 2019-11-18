#ifndef PROGETTOGRAFICA_SHIPMESH_H
#define PROGETTOGRAFICA_SHIPMESH_H

#include "Mesh.h"
#include "TextureManager.h"

class ShipMesh : public Mesh {
private:
    TextureManager *textureManager;
    Texture *m_texture[10];
    void initModel();
    void drawTriangles(int, int, int);
    void setupTexture(GLenum n_texture, Point3 min, Point3 max);
public:
    ShipMesh(char *filename, TextureManager *textureManager):Mesh(filename) {
        this->textureManager = textureManager;
    }
    void render();
};

#endif