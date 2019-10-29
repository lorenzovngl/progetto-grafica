#ifndef PROGETTOGRAFICA_ENVIROMENT_H
#define PROGETTOGRAFICA_ENVIROMENT_H

#include "Buoy.h"

class Enviroment {
private:
    Buoy *buoy[10];
    void drawFloor();
public:
    Enviroment();
    void render();
};


#endif
