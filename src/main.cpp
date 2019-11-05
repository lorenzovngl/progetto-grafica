#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif

#include "headers/Ship.h"
#include "headers/Enviroment.h"
#include "headers/Camera.h"

float viewAlpha = 20, viewBeta = 40; // angoli che definiscono la vista
float eyeDist = 3.0; // distanza dell'occhio dall'origine
int scrH = 700, scrW = 700; // altezza e larghezza viewport (in pixels)

Ship ship;
int nstep = 0; // numero di passi di FISICA fatti fin'ora
const int PHYS_SAMPLING_STEP = 10; // numero di millisec che un passo di fisica simula

// Frames Per Seconds
const int fpsSampling = 3000; // lunghezza intervallo di calcolo fps
float fps = 0; // valore di fps dell'intervallo precedente
int fpsNow = 0; // quanti fotogrammi ho disegnato fin'ora nell'intervallo attuale
Uint32 timeLastInterval = 0; // quando e' cominciato l'ultimo intervallo

Enviroment *enviroment = new Enviroment();
Camera *camera = new Camera();

// setta le matrici di trasformazione in modo
// che le coordinate in spazio oggetto siano le coord
// del pixel sullo schemo
void SetCoordToPixel() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-1, -1, 0);
    glScalef(2.0 / scrW, 2.0 / scrH, 1);
}

/* Esegue il Rendering della scena */
void rendering(SDL_Window *window) {
    // un frame in piu'!!!
    fpsNow++;

    glLineWidth(3); // linee larghe

    // settiamo il viewport
    glViewport(0, 0, scrW, scrH);

    // colore sfondo = bianco
    glClearColor(1, 1, 1, 1);

    // settiamo matrice di vista
    glTranslatef(0, 0, -eyeDist);
    glRotatef(viewBeta, 1, 0, 0);
    glRotatef(viewAlpha, 0, 1, 0);


    // settiamo la matrice di proiezione
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 70, //fovy,
                    ((float)scrW) / scrH,//aspect Y/X,
                    0.2,//distanza del NEAR CLIPPING PLANE in coordinate vista
                    1000  //distanza del FAR CLIPPING PLANE in coordinate vista
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // riempe tutto lo screen buffer di pixel color sfondo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //drawAxis(); // disegna assi frame VISTA

    // setto la posizione luce
    float tmpv[4] = {0, 1, 2, 0}; // ultima comp=0 => luce direzionale
    glLightfv(GL_LIGHT0, GL_POSITION, tmpv);


    camera->set(ship, eyeDist, viewBeta, viewAlpha);


    //drawAxis(); // disegna assi frame MONDO
    static float tmpcol[4] = {1, 1, 1, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tmpcol);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 127);

    // settiamo matrice di modellazione

    //drawAxis(); // disegna assi frame OGGETTO
    //drawCubeWire();

    enviroment->render(ship.px, ship.py, ship.pz); // disegna il mare

    ship.render(); // disegna la barca

    // attendiamo la fine della rasterizzazione di
    // tutte le primitive mandate

    glFinish();
    // ho finito: buffer di lavoro diventa visibile
    SDL_GL_SwapWindow(window);
}

void redraw(){
    // ci automandiamo un messaggio che (s.o. permettendo)
    // ci fara' ridisegnare la finestra
    SDL_Event e;
    e.type=SDL_WINDOWEVENT;
    e.window.event=SDL_WINDOWEVENT_EXPOSED;
    SDL_PushEvent(&e);
}

int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_GLContext mainContext;
    Uint32 windowID;
    SDL_Joystick *joystick;
    static int keymap[Controller::NKEYS] = {SDLK_a, SDLK_d, SDLK_w, SDLK_s};

    // inizializzazione di SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    glutInit(&argc, argv);

    int flags = IMG_INIT_JPG|IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if ((initted & flags) != flags){
        printf("Failed to init SDL_IMG");
    }

    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // facciamo una finestra di scrW x scrH pixels
    window = SDL_CreateWindow(argv[0], 0, 0, scrW, scrH, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    //glutInitWindowSize(scrW, scrH);
    //glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Create our opengl context and attach it to our window
    mainContext = SDL_GL_CreateContext(window);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); // rinormalizza le normali prima di usarle
    //glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW); // consideriamo Front Facing le facce ClockWise
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    bool done = 0;
    while (!done) {

        SDL_Event e;

        // guardo se c'e' un evento:
        if (SDL_PollEvent(&e)) {
            // se si: processa evento
            switch (e.type) {
                case SDL_KEYDOWN:
                    ship.controller.EatKey(e.key.keysym.sym, keymap, true);
                    if (e.key.keysym.sym == SDLK_F1){
                        camera->change(ship, eyeDist, viewBeta, viewAlpha);
                        printf("Change to %d\n", camera->getType());
                    }
                    break;
                case SDL_KEYUP:
                    ship.controller.EatKey(e.key.keysym.sym, keymap, false);
                    break;
                case SDL_QUIT:
                    done = 1;
                    break;
                case SDL_WINDOWEVENT:
                    // dobbiamo ridisegnare la finestra
                    if (e.window.event == SDL_WINDOWEVENT_EXPOSED)
                        rendering(window);
                    else {
                        windowID = SDL_GetWindowID(window);
                        if (e.window.windowID == windowID) {
                            switch (e.window.event) {
                                case SDL_WINDOWEVENT_SIZE_CHANGED: {
                                    scrW = e.window.data1;
                                    scrH = e.window.data2;
                                    glViewport(0, 0, scrW, scrH);
                                    rendering(window);
                                    //redraw(); // richiedi ridisegno
                                    break;
                                }
                            }
                        }
                    }
                    break;

                case SDL_MOUSEMOTION:
                    if (e.motion.state & SDL_BUTTON(1) & camera->getType() == CAMERA_MOUSE) {
                        viewAlpha += e.motion.xrel;
                        viewBeta += e.motion.yrel;
                        //if (viewBeta<-90) viewBeta=-90;
                        if (viewBeta < +5) viewBeta = +5; //per non andare sotto la macchina
                        if (viewBeta > +90) viewBeta = +90;
                        rendering(window);
                        //redraw(); // richiedi un ridisego
                    }
                    break;

                case SDL_MOUSEWHEEL:
                    if (e.wheel.y < 0) {
                        // avvicino il punto di vista (zoom in)
                        eyeDist = eyeDist * 0.9;
                        if (eyeDist < 1) eyeDist = 1;
                    };
                    if (e.wheel.y > 0) {
                        // allontano il punto di vista (zoom out)
                        eyeDist = eyeDist / 0.9;
                    };
                    rendering(window);
                    break;

                case SDL_JOYAXISMOTION: /* Handle Joystick Motion */
                    if (e.jaxis.axis == 0) {
                        if (e.jaxis.value < -3200) {
                            ship.controller.Joy(0, true);
                            ship.controller.Joy(1, false);
                        }
                        if (e.jaxis.value > 3200) {
                            ship.controller.Joy(0, false);
                            ship.controller.Joy(1, true);
                        }
                        if (e.jaxis.value >= -3200 && e.jaxis.value <= 3200) {
                            ship.controller.Joy(0, false);
                            ship.controller.Joy(1, false);
                        }
                        rendering(window);
                        //redraw();
                    }
                    break;
                case SDL_JOYBUTTONDOWN: /* Handle Joystick Button Presses */
                    if (e.jbutton.button == 0) {
//         done=1;
                        ship.controller.Joy(2, true);
                        ship.controller.Joy(3, false);
                    }
                    if (e.jbutton.button == 1) {
//         done=1;
                        ship.controller.Joy(2, false);
                        ship.controller.Joy(3, false);
                    }
                    if (e.jbutton.button == 2) {
//         done=1;
                        ship.controller.Joy(2, false);
                        ship.controller.Joy(3, true);
                    }
                    if (e.jbutton.button == 3) {
//         done=1;
                        ship.controller.Joy(2, false);
                        ship.controller.Joy(3, false);
                    }
                    break;
            }
        } else {
            // nessun evento: siamo IDLE

            Uint32 timeNow = SDL_GetTicks(); // che ore sono?

            if (timeLastInterval + fpsSampling < timeNow) {
                // e' tempo di ricalcolare i Frame per Sec
                fps = 1000.0 * ((float) fpsNow) / (timeNow - timeLastInterval);
                fpsNow = 0;
                timeLastInterval = timeNow;
            }

            bool doneSomething = false;
            int guardia = 0; // sicurezza da loop infinito

            // finche' il tempo simulato e' rimasto indietro rispetto
            // al tempo reale...
            while (nstep * PHYS_SAMPLING_STEP < timeNow) {
                ship.DoStep();
                nstep++;
                doneSomething = true;
                timeNow = SDL_GetTicks();
                if (guardia++ > 1000) {
                    done = true;
                    break;
                } // siamo troppo lenti!
            }

            if (doneSomething)
                rendering(window);
                //redraw();
            else {
                // tempo libero!!!
            }
        }
    }
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return (0);
}