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

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

#endif

extern "C" {
    #include "../lib/ShadowManager/helper_functions.h"
}
#include "../lib/ShadowManager/ShadowManager.h"
#include "headers/Ship.h"
#include "headers/Enviroment.h"
#include "headers/Camera.h"
#include "headers/HUD.h"
#include "headers/Game.h"
#include "headers/Utils.h"
#include "headers/TextureManager.h"

float viewAlpha = 20, viewBeta = 40; // angoli che definiscono la vista
float eyeDist = 3.0; // distanza dell'occhio dall'origine
int scrH = 700, scrW = 700; // altezza e larghezza viewport (in pixels)

Ship *ship;
int nstep = 0; // numero di passi di FISICA fatti fin'ora
const int PHYS_SAMPLING_STEP = 10; // numero di millisec che un passo di fisica simula

// Frames Per Seconds
const int fpsSampling = 3000; // lunghezza intervallo di calcolo fps
float fps = 0; // valore di fps dell'intervallo precedente
int fpsNow = 0; // quanti fotogrammi ho disegnato fin'ora nell'intervallo attuale
Uint32 timeLastInterval = 0; // quando e' cominciato l'ultimo intervallo

SDL_Renderer *renderer;
TextureManager *textureManager;
Enviroment *enviroment;
Camera *camera;
HUD *hud;
Game *game;
ShadowManager *shadowManager;

#define SHADOW_MAP_RESOLUTION 1024

void DrawGL(void)
{
    printf("DrawGL() called\n");
    // All the things about time are just used to display FPS (F2)
    // or to move objects around (NOT for shadow)
    static unsigned begin = 0;
    static unsigned cur_time = 0;
    unsigned elapsed_time,delta_time;
    float elapsedMs;float cosAlpha,sinAlpha;    // used to move objects around

    // These two instead are necessary for shadow mapping
    static float vMatrixInverse[16];            // view Matrix inverse (it's the camera matrix).
    static float lvpMatrix[16];                 // = light_pMatrix*light_vMatrix

    // Just some time stuff here
    if (begin==0) begin = glutGet(GLUT_ELAPSED_TIME);
    elapsed_time = glutGet(GLUT_ELAPSED_TIME) - begin;
    delta_time = elapsed_time - cur_time;
    float instantFrameTime = (float)delta_time*0.001f;
    cur_time = elapsed_time;

    elapsedMs = (float)elapsed_time;
    cosAlpha = cos(elapsedMs*0.0005f);
    sinAlpha = sin(elapsedMs*0.00075f);


    // view Matrix
    Helper_LookAt(shadowManager->vMatrix,shadowManager->cameraPos[0],shadowManager->cameraPos[1],shadowManager->cameraPos[2],
                  shadowManager->targetPos[0],shadowManager->targetPos[1],shadowManager->targetPos[2],0,1,0);
    glLoadMatrixf(shadowManager->vMatrix);
    glLightfv(GL_LIGHT0,GL_POSITION,shadowManager->lightDirection);    // Important: the ffp must recalculate internally lightDirectionEyeSpace based on vMatrix [=> every frame]

    // view Matrix inverse (it's the camera matrix). Used twice below (and very important to keep in any case).
    Helper_InvertMatrixFast(vMatrixInverse,shadowManager->vMatrix);    // We can use Helper_InvertMatrixFast(...) instead of Helper_InvertMatrix(...) here [No scaling inside and no projection matrix]


    // Draw to Shadow Map------------------------------------------------------------------------------------------
    {
#       ifndef USE_UNSTABLE_SHADOW_MAPPING_TECHNIQUE
        Helper_GetLightViewProjectionMatrix(lvpMatrix,
                                            vMatrixInverse,shadowManager->pMatrixNearPlane,shadowManager->pMatrixFarPlane,
                                            shadowManager->pMatrixFovyDeg,(scrW/scrH),shadowManager->lightDirection,1.0f/(float)SHADOW_MAP_RESOLUTION);
#       else //USE_UNSTABLE_SHADOW_MAPPING_TECHNIQUE
        Helper_GetLightViewProjectionMatrixExtra(0,
                                             vMatrixInverse,pMatrixNearPlane,pMatrixFarPlane,pMatrixFovyDeg,current_aspect_ratio,config.use_camera_ortho3d_projection_matrix?cameraDistance:0,  // in camera ortho3d mode, we can still pass zero as last arg here to avoid shadow-flickering when zooming in-out, at the expense of the further boost in shadow resolution that ortho mode can give us
                                             lightDirection,1.0f/(float)SHADOW_MAP_RESOLUTION,
                                             0,0,
                                             pMatrixInverse,    // Mandatory when we need to retrieve arguments that follow it
                                             0,0,
                                             lvpMatrix);        // Technically this was provided as an 'lvpMatrix for optimal frustum culling usage' in the 'Stable Shadow Mapping' case (but can be used to replace it too)
#       endif //USE_UNSTABLE_SHADOW_MAPPING_TECHNIQUE

        // Draw to shadow map texture
        glMatrixMode(GL_PROJECTION);glPushMatrix();glLoadIdentity();glMatrixMode(GL_MODELVIEW);        // We'll set the combined light view-projection matrix in GL_MODELVIEW (do you know that it's the same?)
        glBindFramebuffer(GL_FRAMEBUFFER,  shadowManager->shadowPass.fbo);
        glViewport(0, 0, SHADOW_MAP_RESOLUTION,SHADOW_MAP_RESOLUTION);
        glClear(GL_DEPTH_BUFFER_BIT);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glCullFace(GL_FRONT);
        glEnable(GL_DEPTH_CLAMP);
        glUseProgram( shadowManager->shadowPass.program);            // we can just use glUseProgram(0) here
        glPushMatrix();glLoadMatrixf(lvpMatrix); // we load both (light) projection and view matrices here (it's the same after all)
        Helper_GlutDrawGeometry(elapsedMs,cosAlpha,sinAlpha,shadowManager->targetPos,shadowManager->pgDisplayListBase);  // Done SHADOW_MAP_NUM_CASCADES times!
        glPopMatrix();
        glUseProgram(0);
        glDisable(GL_DEPTH_CLAMP);
        glCullFace(GL_BACK);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glMatrixMode(GL_PROJECTION);glPopMatrix();glMatrixMode(GL_MODELVIEW);

    }

    // Draw world
    {
        // biasedShadowMvpMatrix is used only in the DefaultPass:
        static float bias[16] = {0.5,0,0,0, 0,0.5,0,0,  0,0,0.5,0,    0.5,0.5,0.5,1}; // Moving from unit cube in NDC [-1,1][-1,1][-1,1] to [0,1][0,1][0,1] (x and y are texCoords; z is the depth range, [0,1] by default in window coordinates)
        static float biasedShadowMvpMatrix[16];     // multiplied per vMatrixInverse
        Helper_MultMatrix(biasedShadowMvpMatrix,bias,lvpMatrix);
        Helper_MultMatrix(biasedShadowMvpMatrix,biasedShadowMvpMatrix,vMatrixInverse);  // We do this, so that when in the vertex shader we multiply it with the camera mvMatrix, we get: biasedShadowMvpMatrix * mMatrix (using mMatrices directly in the shaders prevents the usage of double precision matrices: mvMatrices are good when converted to float to feed the shader, mMatrices are bad)

        // Draw to world
        glViewport(0, 0, scrW, scrH);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, shadowManager->shadowPass.textureId);
        glUseProgram( shadowManager->defaultPass.program);
        glUniformMatrix4fv( shadowManager->defaultPass.uniform_location_biasedShadowMvpMatrix, 1 /*only setting 1 matrix*/, GL_FALSE /*transpose?*/,biasedShadowMvpMatrix);
        Helper_GlutDrawGeometry(elapsedMs,cosAlpha,sinAlpha,shadowManager->targetPos,shadowManager->pgDisplayListBase);  // Done SHADOW_MAP_NUM_CASCADES times!
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D,0);
    }


    {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glColor3f(1,1,1);
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, shadowManager->shadowPass.textureId);
        glColor4f(1,1,1,0.9f);
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);glVertex2f(-1,    -1);
        glTexCoord2f(1,0);glVertex2f(-0.25*(scrW/scrH), -1);
        glTexCoord2f(1,1);glVertex2f(-0.25*(scrW/scrH), -0.25/(scrW/scrH));
        glTexCoord2f(0,1);glVertex2f(-1,    -0.25/(scrW/scrH));
        glEnd();
        glBindTexture(GL_TEXTURE_2D,0);
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDepthMask(GL_TRUE);
    }
}

/* Esegue il Rendering della scena */
void rendering(SDL_Window *window) {
    // un frame in piu'!!!
    /*fpsNow++;

    glLineWidth(3); // linee larghe

    // settiamo il viewport
    glViewport(0, 0, scrW, scrH);

    glClearColor(0,0,0,0);

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

    camera->set(*ship, eyeDist, viewBeta, viewAlpha);*/

    // setto la posizione luce
    /*float ambient[4] = {1, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    float diffuse[4] = {1, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    float specular[4] = {1, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    float position[4] = {30, 50, 0, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    static float tmpcol[4] = {1, 1, 1, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tmpcol);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 127);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);*/

    // settiamo matrice di modellazione

    //Utils::drawTranslatedAxis(30, 50, 0);
    //drawCubeWire();

    //glColor3f(0.4,0.4,0.4);

    // Shadow rendering
    /*glPushMatrix();
    float l[] = { 30.0, 50.0, 0.0 }; // Coordinates of the light source
    float n[] = { 0.0,  -1.0, 0.0 }; // Normal vector for the plane
    float e[] = { 0.0, 0.0, 0.0 }; // Point of the plane
    glTranslatef(0, -0.02, 0);
    Utils::glShadowProjection(l,e,n);
    glDisable(GL_LIGHTING);
    glColor3f(0.4,0.4,0.4);
    ship->render(false);
    glEnable(GL_LIGHTING);
    glPopMatrix();*/

    shadowManager->DrawGL();
    //ship->render(false);
    //enviroment->render(ship->px, ship->py, ship->pz); // disegna il mare

    //game->detectCollision();
    //hud->display(scrW, scrH);

    // attendiamo la fine della rasterizzazione di
    // tutte le primitive mandate

    //glFinish();
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
        printf("Failed to init SDL_IMG\n");
        return 1;
    }

    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // facciamo una finestra di scrW x scrH pixels
    window = SDL_CreateWindow(argv[0], 0, 0, scrW, scrH, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    glutInitWindowSize(scrW, scrH);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);

    if (!glfwInit()){
        printf("Failed to init GLFW\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    int res = glewInit();
    if (res != GLEW_OK){
        printf("Failed to init GLEW: %s\n", glewGetErrorString(res));
        return 1;
    }

    //Create our opengl context and attach it to our window
    mainContext = SDL_GL_CreateContext(window);

   /* glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); // rinormalizza le normali prima di usarle
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW); // consideriamo Front Facing le facce ClockWise*/
    //glEnable(GL_COLOR_MATERIAL);
    //glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Otherwise transparent objects are not displayed correctly

    textureManager = new TextureManager();

    enviroment = new Enviroment(textureManager);
    ship = new Ship(textureManager);
    game = new Game(ship, enviroment);
    camera = new Camera();
    hud = new HUD(game);
    shadowManager = new ShadowManager();

    shadowManager->InitGL();
    shadowManager->ResizeGL(scrW, scrH);
    shadowManager->resetCamera();
    shadowManager->resetLight();


    bool done = 0;
    while (!done) {

        SDL_Event e;

        // guardo se c'e' un evento:
        if (SDL_PollEvent(&e)) {
            // se si: processa evento
            switch (e.type) {
                case SDL_KEYDOWN:
                    ship->controller.EatKey(e.key.keysym.sym, keymap, true);
                    if (e.key.keysym.sym == SDLK_F1){
                        camera->change(*ship, eyeDist, viewBeta, viewAlpha);
                        printf("Change to %d\n", camera->getType());
                    }
                    break;
                case SDL_KEYUP:
                    ship->controller.EatKey(e.key.keysym.sym, keymap, false);
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
                    if (e.motion.state & SDL_BUTTON(1) &&
                    (camera->getType() == CAMERA_MOUSE || camera->getType() == CAMERA_MOUSE_SHIP)) {
                        viewAlpha += e.motion.xrel;
                        viewBeta += e.motion.yrel;
                        //if (viewBeta<-90) viewBeta=-90;
                        //if (viewBeta < +5) viewBeta = +5; //per non andare sotto la macchina
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
                            ship->controller.Joy(0, true);
                            ship->controller.Joy(1, false);
                        }
                        if (e.jaxis.value > 3200) {
                            ship->controller.Joy(0, false);
                            ship->controller.Joy(1, true);
                        }
                        if (e.jaxis.value >= -3200 && e.jaxis.value <= 3200) {
                            ship->controller.Joy(0, false);
                            ship->controller.Joy(1, false);
                        }
                        rendering(window);
                        //redraw();
                    }
                    break;
                case SDL_JOYBUTTONDOWN: /* Handle Joystick Button Presses */
                    if (e.jbutton.button == 0) {
//         done=1;
                        ship->controller.Joy(2, true);
                        ship->controller.Joy(3, false);
                    }
                    if (e.jbutton.button == 1) {
//         done=1;
                        ship->controller.Joy(2, false);
                        ship->controller.Joy(3, false);
                    }
                    if (e.jbutton.button == 2) {
//         done=1;
                        ship->controller.Joy(2, false);
                        ship->controller.Joy(3, true);
                    }
                    if (e.jbutton.button == 3) {
//         done=1;
                        ship->controller.Joy(2, false);
                        ship->controller.Joy(3, false);
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
                ship->DoStep();
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