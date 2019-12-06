#ifndef PROGETTOGRAFICA_CAMERA_H
#define PROGETTOGRAFICA_CAMERA_H

#include "Ship.h"
#include "../../lib/ShadowMapper/ShadowMapper.h"

#define CAMERA_BACK_SHIP 0
#define CAMERA_PILOT 1
#define CAMERA_MOUSE 2
#define CAMERA_MOUSE_SHIP 3
#define CAMERA_TYPE_MAX 4

class Camera {
private:
    int m_type;
    ShadowMapper *shadowMapper;
public:
    float cameraPos[3];
    float targetPos[3];
    Camera();
    void setShadowMapper(ShadowMapper *shadowMapper);
    int getType();
    void change(Ship, float, float, float);
    void set(Ship, float, float, float);
};


#endif
