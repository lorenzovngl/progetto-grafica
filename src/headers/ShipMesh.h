#ifndef PROGETTOGRAFICA_SHIPMESH_H
#define PROGETTOGRAFICA_SHIPMESH_H

#include "Mesh.h"

class ShipMesh : public Mesh {
private:
    Texture *m_texture[10];
    void initModel();
    void applyTexture(int, Point3*, float, float, float);
public:
    ShipMesh(char *filename):Mesh(filename) {
    }
    void render();
};

#endif