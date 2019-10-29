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

#include "headers/Mesh.h"
#include "headers/Ship.h"

// var globale di tipo meshF
Mesh carlinga((char *) "assets/ship/fishing_ship.obj"); // chiama il costruttore

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
    sterzo *= velRitornoSterzo; // ritorno a volante fermo

    if (controller.key[Controller::ACC]) vzm -= accMax;// accelerazione in avanti
    if (controller.key[Controller::DEC]) vzm += accMax;// accelerazione indietro

    // attriti (semplificando)
    vxm *= attritoX;
    vym *= attritoY;
    vzm *= attritoZ;

    // l'orientamento della macchina segue quello dello sterzo
    // (a seconda della velocita' sulla z)
    facing = facing - (vzm * grip) * sterzo;

    // rotazione mozzo ruote (a seconda della velocita' sulla z)
    float da; //delta angolo
    da = (360.0 * vzm) / (2.0 * M_PI * raggioRuotaA);
    mozzoA += da;
    da = (360.0 * vzm) / (2.0 * M_PI * raggioRuotaP);
    mozzoP += da;

    // ritorno a vel coord mondo
    vx = +cosf * vxm + sinf * vzm;
    vy = vym;
    vz = -sinf * vxm + cosf * vzm;

    // posizione = posizione + velocita * delta t (ma e' delta t costante)
    px += vx;
    py += vy;
    pz += vz;
}

//void drawCube(); // questa e' definita altrove (quick hack)
void drawAxis(); // anche questa

void Ship::Init() {
    // inizializzo lo stato della macchina
    px = pz = facing = 0; // posizione e orientamento
    py = 0.0;

    mozzoA = mozzoP = sterzo = 0;   // stato
    vx = vy = vz = 0;      // velocita' attuale
    // inizializzo la struttura di controllo
    controller.Init();

    velSterzo = 3.4;         // A
    velRitornoSterzo = 0.93; // B, sterzo massimo = A*B / (1-B)

    accMax = 0.0011;

    // attriti: percentuale di velocita' che viene mantenuta
    // 1 = no attrito
    // <<1 = attrito grande
    attritoZ = 0.991;  // piccolo attrito sulla Z (nel senso di rotolamento delle ruote)
    attritoX = 0.8;  // grande attrito sulla X (per non fare slittare la macchina)
    attritoY = 1.0;  // attrito sulla y nullo

    // Nota: vel max = accMax*attritoZ / (1-attritoZ)

    raggioRuotaA = 0.25;
    raggioRuotaP = 0.35;

    grip = 0.45; // quanto il facing macchina si adegua velocemente allo sterzo
}

// attiva una luce di openGL per simulare un faro della macchina
void Ship::DrawHeadlight(float x, float y, float z, int lightN) const {
    int usedLight = GL_LIGHT1 + lightN;

    glEnable(usedLight);

    float col0[4] = {0.8, 0.8, 0.0, 1};
    glLightfv(usedLight, GL_DIFFUSE, col0);

    float col1[4] = {0.5, 0.5, 0.0, 1};
    glLightfv(usedLight, GL_AMBIENT, col1);

    float tmpPos[4] = {x, y, z, 1}; // ultima comp=1 => luce posizionale
    glLightfv(usedLight, GL_POSITION, tmpPos);

    float tmpDir[4] = {0, 0, -1, 0}; // ultima comp=1 => luce posizionale
    glLightfv(usedLight, GL_SPOT_DIRECTION, tmpDir);

    glLightf(usedLight, GL_SPOT_CUTOFF, 30);
    glLightf(usedLight, GL_SPOT_EXPONENT, 5);

    glLightf(usedLight, GL_CONSTANT_ATTENUATION, 0);
    glLightf(usedLight, GL_LINEAR_ATTENUATION, 1);
}

// disegna a schermo
void Ship::Render() const {
    // sono nello spazio mondo

    //drawAxis(); // disegno assi spazio mondo
    glPushMatrix();

    glTranslatef(px, py, pz);
    glRotatef(facing, 0, 1, 0);

    // sono nello spazio MACCHINA
    //drawAxis(); // disegno assi spazio macchina

//  DrawHeadlight(-0.3,0,-1, 0); // accendi faro sinistro
//  DrawHeadlight(+0.3,0,-1, 1); // accendi faro destro

    // drawCarlinga(); // disegna la carliga con pochi parallelepidedi

    // disegna la carliga con una mesh
    glPushMatrix();
    glScalef(-0.05, 0.05, -0.05); // patch: riscaliamo la mesh di 1/10
    glColor3f(1, 1, 1); // colore bianco

    // Meccanismi di oscillazione
    /*using namespace std::chrono;
    milliseconds ms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );

    int maxTiltAngle = 3;
    float tiltAngle = 0;
    long timeUnits = ms.count() / 1000;
    if ((timeUnits % (maxTiltAngle*2)) >= maxTiltAngle){
        tiltAngle = (timeUnits % maxTiltAngle) - maxTiltAngle/2;
    } else {
        tiltAngle = maxTiltAngle/2 - (timeUnits % maxTiltAngle);
    }
    glRotatef(tiltAngle, 1, 0, 0);

    maxTiltAngle = 2;
    if ((timeUnits % (maxTiltAngle*2)) >= maxTiltAngle){
        tiltAngle = (timeUnits % maxTiltAngle) - maxTiltAngle/2;
    } else {
        tiltAngle = maxTiltAngle/2 - (timeUnits % maxTiltAngle);
    }
    glRotatef(tiltAngle, 0, 0, 1);*/

    //carlinga.Render();
    //carlinga.Texturize();
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//		     glEnable(GL_TEXTURE_2D);

    carlinga.Render();
    glColor3f(.4, .4, .4);

    glPopMatrix();
}
