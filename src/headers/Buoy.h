#ifndef PROGETTOGRAFICA_BUOY_H
#define PROGETTOGRAFICA_BUOY_H

#include "Mesh.h"
#include "TextureManager.h"
#include "Options.h"

class Buoy {
private:
    Mesh *m_mesh;
    TextureManager *textureManager;
    Options *options;
    bool active;
    float m_coord_x;
    float m_coord_z;
    int id;
public:
    Buoy(int, float, float, TextureManager *textureManager, Options *options);
    Mesh* getMesh();
    float getCoordX();
    float getCoordZ();
    void render();
    bool isActive();
    void disable();
};

#endif