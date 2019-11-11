#ifndef PROGETTOGRAFICA_BUOY_H
#define PROGETTOGRAFICA_BUOY_H

#include "Mesh.h"

class Buoy {
private:
    Mesh *m_mesh;
    float m_coord_x;
    float m_coord_z;
public:
    Buoy(float, float);
    Mesh* getMesh();
    float getCoordX();
    float getCoordZ();
    void render();
};

#endif