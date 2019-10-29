#ifndef PROGETTOGRAFICA_CAMERA_H
#define PROGETTOGRAFICA_CAMERA_H

#include "Ship.h"

#define CAMERA_BACK_SHIP 0
#define CAMERA_PILOT 1
#define CAMERA_MOUSE 2
#define CAMERA_TYPE_MAX 3

class Camera {
private:
    int m_type;
public:
    Camera();
    int getType();
    void change(Ship, float, float, float);
    void set(Ship, float, float, float);
};


#endif
