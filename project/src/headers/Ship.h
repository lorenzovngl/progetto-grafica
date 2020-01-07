#ifndef PROGETTOGRAFICA_SHIP_H
#define PROGETTOGRAFICA_SHIP_H

#include "Controller.h"
#include "ShipMesh.h"
#include "Options.h"
#include "ShaderParams.h"
#include "Frontier.h"

class Ship {
private:
    TextureManager *textureManager;
    ShadowMapper *shadowMapper;
    ShaderParams *mShaderParams;
    Options *options;
    Frontier *mFrontier;

    void enableTilt();

public:
    Controller controller;

    Ship(TextureManager *textureManager, ShadowMapper *shadowMapper, ShaderParams *shaderParams, Options *options,
         Frontier *frontier);

    // Metodi
    void Init(); // inizializza variabili
    void reset();

    void render(bool texture_enabled); // disegna a schermo
    void DoStep(); // computa un passo del motore fisico
    ShipMesh *getMesh();

    // STATO DELLA BARCA
    // (DoStep fa evolvere queste variabili nel tempo)
    float px, py, pz, facing; // posizione e orientamento
    float sterzo; // stato interno
    float vx, vy, vz; // velocita' attuale

    float tilt_angle_x, tilt_angle_z;       //Angoli di inclinazione

    // STATISTICHE DELLA BARCA
    // (di solito rimangono costanti)
    float velSterzo, velRitornoSterzo, accMax,
            grip, attritoX, attritoY, attritoZ;
};

#endif
