#ifndef PROGETTOGRAFICA_SHIP_H
#define PROGETTOGRAFICA_SHIP_H

#include "Controller.h"

class Ship {
private:
    void DrawHeadlight(float x, float y, float z, int lightN) const;
    void enableTilt();
public:
    // Metodi
    void Init(); // inizializza variabili
    void render(); // disegna a schermo
    void DoStep(); // computa un passo del motore fisico
    Ship() { Init(); } // costruttore

    Controller controller;

    // STATO DELLA MACCHINA
    // (DoStep fa evolvere queste variabili nel tempo)
    float px, py, pz, facing; // posizione e orientamento
    float mozzoA, mozzoP, sterzo; // stato interno
    float vx, vy, vz; // velocita' attuale

    float tilt_angle_x, tilt_angle_z;       //Angoli di inclinazione

    // STATS DELLA MACCHINA
    // (di solito rimangono costanti)
    float velSterzo, velRitornoSterzo, accMax, attrito,
            raggioRuotaA, raggioRuotaP, grip,
            attritoX, attritoY, attritoZ; // attriti
};

#endif
