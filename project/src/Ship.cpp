// car.cpp
// implementazione dei metodi definiti in car.h

#include <stdio.h>
#include <math.h>
#include <chrono>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/gl.h>
#include <GL/glu.h>

#endif

#include <vector> // la classe vector di SDL
#include <SDL2/SDL_timer.h>
#include <GL/freeglut_std.h>

#include "headers/Mesh.h"
#include "headers/Ship.h"
#include "headers/Utils.h"
#include "headers/ShipMesh.h"
#include "../lib/ShadowMapper/ShadowMapper.h"
#include "headers/Frontier.h"

// var globale di tipo meshF
ShipMesh *carlinga;

Ship::Ship(TextureManager *textureManager, ShadowMapper* shadowMapper, ShaderParams* shaderParams, Options* options, Frontier *frontier) {
    this->textureManager = textureManager;
    this->shadowMapper = shadowMapper;
    this->mShaderParams = shaderParams;
    this->options = options;
    mFrontier = frontier;
    Init();
}

ShipMesh* Ship::getMesh(){
    return carlinga;
}

// DoStep: facciamo un passo di fisica (a delta-t costante)
//
// Indipendente dal rendering.
//
// ricordiamoci che possiamo LEGGERE ma mai SCRIVERE
// la struttura controller da DoStep
void Ship::DoStep() {
    // computiamo l'evolversi della macchina

    float vxm, vym, vzm; // velocita' in spazio macchina

    // da vel frame mondo a vel frame macchina
    float cosf = cos(facing * M_PI / 180.0);
    float sinf = sin(facing * M_PI / 180.0);

    vxm = +cosf * vx - sinf * vz;
    vym = vy;
    vzm = +sinf * vx + cosf * vz;

    // gestione dello sterzo
    if (controller.key[Controller::LEFT]) sterzo += velSterzo;
    if (controller.key[Controller::RIGHT]) sterzo -= velSterzo;
    float oldSterzo = sterzo;
    sterzo *= velRitornoSterzo; // ritorno a volante fermo

    if (controller.key[Controller::ACC]) vzm -= accMax;// accelerazione in avanti
    if (controller.key[Controller::DEC]) vzm += accMax;// accelerazione indietro

    // attriti (semplificando)
    vxm *= attritoX;
    vym *= attritoY;
    vzm *= attritoZ;

    // l'orientamento segue quello dello sterzo
    // (a seconda della velocita' sulla z)
    facing = facing - (vzm * grip) * sterzo;

    // ritorno a vel coord mondo
    vx = +cosf * vxm + sinf * vzm;
    vy = vym;
    vz = -sinf * vxm + cosf * vzm;

    if(mFrontier->isPointInBounds(px + vx, pz + vz)){
        // posizione = posizione + velocita * delta t (ma e' delta t costante)
        px += vx;
        py += vy;
        pz += vz;
    } else {
       vx = vy = vz = 0;
    }
}

void Ship::Init() {
    carlinga = new ShipMesh((char *) "assets/ship/fishing_ship.obj", this->textureManager,
            this->shadowMapper, this->mShaderParams, options);
    // inizializzo lo stato
    px = pz = facing = 0; // posizione e orientamento
    py = 0.0;

    sterzo = 0;   // stato
    vx = vy = vz = 0;      // velocita' attuale
    // inizializzo la struttura di controllo
    controller.Init();

    velSterzo = 3.4;         // A
    velRitornoSterzo = 0.93; // B, sterzo massimo = A*B / (1-B)

    accMax = 0.0011;

    // attriti: percentuale di velocita' che viene mantenuta
    // 1 = no attrito
    // <<1 = attrito grande
    attritoZ = 0.991;  // piccolo attrito sulla Z
    attritoX = 0.8;  // grande attrito sulla X
    attritoY = 1.0;  // attrito sulla y nullo

    // Nota: vel max = accMax*attritoZ / (1-attritoZ)

    grip = 0.45; // quanto il facing si adegua velocemente allo sterzo
}

void Ship::reset(){
    facing = 0;
    px = py = pz = 0;
    vx = vy = vz = 0;
}

void Ship::enableTilt(){
    // Meccanismi di oscillazione
    float ms = (float) SDL_GetTicks()/1000;

    float maxTiltAngle = 3;
    ms = Utils::floatMod(ms, maxTiltAngle*2);
    if (ms >= maxTiltAngle){
        tilt_angle_x = Utils::floatMod(ms, maxTiltAngle) - maxTiltAngle / 2;
    } else {
        tilt_angle_x = maxTiltAngle / 2 - Utils::floatMod(ms, maxTiltAngle);
    }
    if (tilt_angle_x > 0){
        tilt_angle_x = log(tilt_angle_x+1);
    } else if (tilt_angle_x < 0){
        tilt_angle_x = - log(abs(tilt_angle_x)+1);
    }
    glRotatef(tilt_angle_x, 1, 0, 0);

    ms = (float) SDL_GetTicks()/1000;

    maxTiltAngle = 2;
    ms = Utils::floatMod(ms, maxTiltAngle*2);
    if (ms >= maxTiltAngle){
        tilt_angle_z = Utils::floatMod(ms, maxTiltAngle) - maxTiltAngle / 2;
    } else {
        tilt_angle_z = maxTiltAngle / 2 - Utils::floatMod(ms, maxTiltAngle);
    }
    if (tilt_angle_z > 0){
        tilt_angle_z = log(tilt_angle_z+1);
    } else if (tilt_angle_z < 0){
        tilt_angle_z = - log(abs(tilt_angle_z)+1);
    }
    glRotatef(tilt_angle_z, 0, 0, 1);
}

// disegna a schermo
void Ship::render(bool texture_enabled) {
    // sono nello spazio mondo
    glPushMatrix();

    mShaderParams->setParam(mShaderParams->colorOrTextureParam, TEXTURE_MODE);

    glTranslatef(px, py, pz);
    glRotatef(facing, 0, 1, 0);

    // sono nello spazio del modello
    glScalef(0.05, 0.05, 0.05); // patch: riscaliamo la mesh di 1/10

    // TODO: Cambiano colore alle boe, gestire
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glRotatef(-90, 0, 1, 0);
    enableTilt();
    // Controlla l'inclinazione della barca basandosi sull'angolo di virata e sulla velocità
    glRotatef(-sterzo/3*((abs(vx)+abs(vz))*10), 1, 0, 0);
    glColor3f(1.0, 1.0, 1.0);
    carlinga->render(texture_enabled, abs(vx)+abs(vy)+abs(vz), -sterzo);
    carlinga->ComputeBoundingBox(px, py, pz, 0.05, -90 + facing);
    //glColor3f(.4, .4, .4);

    glPopMatrix();
}
