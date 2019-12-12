#ifndef PROGETTOGRAFICA_SHIPMESH_H
#define PROGETTOGRAFICA_SHIPMESH_H

#include "Mesh.h"
#include "TextureManager.h"
#include "../../lib/ShadowMapper/ShadowMapper.h"

class ShipMesh : public Mesh {
private:
    TextureManager *textureManager;
    ShadowMapper *shadowMapper;
    Texture *m_texture[10];
    void initModel();
    void drawTriangles(bool texture_enabled, int, int, int);
    void drawHelixes(bool texture_enabled);
    void drawRudders(bool texture_enabled);
    void setupTexture(GLenum n_texture, Point3 min, Point3 max);
public:
    ShipMesh(char *filename, TextureManager *textureManager, ShadowMapper *shadowMapper):Mesh(filename) {
        this->textureManager = textureManager;
        this->shadowMapper = shadowMapper;
    }
    void render(bool texture_enabled, float speed, float facing);
};

#endif