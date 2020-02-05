#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "../lib/ShadowMapper/helper_functions.h"
}

#include "../lib/ShadowMapper/ShadowMapper.h"
#include "headers/Ship.h"
#include "headers/Enviroment.h"
#include "headers/Camera.h"
#include "headers/HUD.h"
#include "headers/Game.h"
#include "headers/Utils.h"
#include "headers/TextureManager.h"
#include "headers/Options.h"
#include "headers/Fog.h"
#include "headers/ShaderParams.h"
#include "headers/Frontier.h"
#include "headers/Leaderboard.h"


int viewportWidth = 1024;
int viewportHeight = 768;

float viewAlpha = 20, viewBeta = 40; // angoli che definiscono la vista
float eyeDist = 3.0; // distanza dell'occhio dall'origine

Ship *ship;
int nstep = 0; // numero di passi di FISICA fatti fin'ora
const int PHYS_SAMPLING_STEP = 10; // numero di millisec che un passo di fisica simula

// Frames Per Seconds
const int fpsSampling = 500; // lunghezza intervallo di calcolo fps (ms)
float fps = 0; // valore di fps dell'intervallo precedente
int fpsNow = 0; // quanti fotogrammi ho disegnato fin'ora nell'intervallo attuale
Uint32 timeLastInterval = 0; // quando e' cominciato l'ultimo intervallo

SDL_Renderer *renderer;
TextureManager *textureManager;
Enviroment *enviroment;
Frontier *frontier;
Fog *fog;
Camera *camera;
HUD *hud;
Game *game;
ShadowMapper *shadowMapper;
ShaderParams *shaderParams;
Options *options;
bool inputModeEnabled;
char usenameInputBuffer[50];
char buoysCountInputBuffer[10];

/* Esegue il Rendering della scena */
void rendering(SDL_Window *window) {
    // un frame in piu'!!!
    fpsNow++;

    glLineWidth(1);

    // settiamo il viewport
    glViewport(0, 0, viewportWidth, viewportHeight);

    glClearColor(1, 1, 1, 1);

    // settiamo matrice di vista
    glTranslatef(0, 0, -eyeDist);
    glRotatef(viewBeta, 1, 0, 0);
    glRotatef(viewAlpha, 0, 1, 0);


    // settiamo la matrice di proiezione
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, //fovy,
                   (float) viewportWidth / (float) viewportHeight,//aspect Y/X,
                   0.2,//distanza del NEAR CLIPPING PLANE in coordinate vista
                   1000  //distanza del FAR CLIPPING PLANE in coordinate vista
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // riempe tutto lo screen buffer di pixel color sfondo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float ambient[4] = {0.2, 0.2, 0.2, 1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    float diffuse[4] = {0.8, 0.8, 0.8, 1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    float specular[4] = {0, 0, 0, 0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    static float tmpcol[4] = {1, 1, 1, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tmpcol);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    camera->set(*ship, eyeDist, viewBeta, viewAlpha);

    // These two instead are necessary for shadow mapping
    static float vMatrixInverse[16];            // view Matrix inverse (it's the camera matrix).
    static float lvpMatrix[16];                 // = light_pMatrix*light_vMatrix
    static float biasedShadowMvpMatrix[16];

    glLoadMatrixf(camera->viewMatrix);
    // Important: the ffp must recalculate internally lightDirectionEyeSpace based on vMatrix [=> every frame]
    glLightfv(GL_LIGHT0, GL_POSITION, shadowMapper->lightDirection);

    if (options->areShadersEnabled()) {

        // view Matrix inverse (it's the camera matrix). Used twice below (and very important to keep in any case).
        // We can use Helper_InvertMatrixFast(...) instead of Helper_InvertMatrix(...) here [No scaling inside and no projection matrix]
        Helper_InvertMatrixFast(vMatrixInverse, camera->viewMatrix);

        // Draw to Shadow Map
        Helper_GetLightViewProjectionMatrix(lvpMatrix,
                                            vMatrixInverse, shadowMapper->pMatrixNearPlane,
                                            shadowMapper->pMatrixFarPlane,
                                            shadowMapper->pMatrixFovyDeg,
                                            (float) viewportWidth / (float) viewportHeight,
                                            shadowMapper->lightDirection,
                                            1.0f / (float) SHADOW_MAP_RESOLUTION);

        // Draw to shadow map texture
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glMatrixMode(
                GL_MODELVIEW);        // We'll set the combined light view-projection matrix in GL_MODELVIEW (do you know that it's the same?)
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapper->shadowPass.fbo);
        glViewport(0, 0, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION);
        glClear(GL_DEPTH_BUFFER_BIT);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glCullFace(GL_FRONT);
        glEnable(GL_DEPTH_CLAMP);
        glPushMatrix();
        glLoadMatrixf(lvpMatrix); // we load both (light) projection and view matrices here (it's the same after all)
        ship->render(false);
        enviroment->renderBuoys();
        glPopMatrix();
        glDisable(GL_DEPTH_CLAMP);
        glCullFace(GL_BACK);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        // Draw world
        // biasedShadowMvpMatrix is used only in the DefaultPass:
        static float bias[16] = {0.5, 0, 0, 0,
                                 0, 0.5, 0, 0,
                                 0, 0, 0.5, 0,
                                 0.5, 0.5, 0.5, 1};
        // Moving from unit cube in NDC [-1,1][-1,1][-1,1] to [0,1][0,1][0,1] (x and y are texCoords; z is the depth range,
        // [0,1] by default in window coordinates)
        // multiplied per vMatrixInverse
        Helper_MultMatrix(biasedShadowMvpMatrix, bias, lvpMatrix);
        // We do this, so that when in the vertex shader we multiply it with the camera mvMatrix,
        // we get: biasedShadowMvpMatrix * mMatrix (using mMatrices directly in the shaders prevents the usage of double
        // precision matrices: mvMatrices are good when converted to float to feed the shader, mMatrices are bad)
        Helper_MultMatrix(biasedShadowMvpMatrix, biasedShadowMvpMatrix, vMatrixInverse);
    }

    glViewport(0, 0, viewportWidth, viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (options->areShadersEnabled()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowMapper->shadowPass.textureId);
        glActiveTexture(GL_TEXTURE1);
        glUseProgram(shadowMapper->defaultPass.program);
        glUniform1i(glGetUniformLocation(shadowMapper->defaultPass.program, "u_texture"), 1);
        glUniform1i(glGetUniformLocation(shadowMapper->defaultPass.program, "u_fogEnabled"),
                    (options->isFogEnabled()) ? 1 : 0);
        glUniform4fv(glGetUniformLocation(shadowMapper->defaultPass.program, "u_cameraEye"), 1, camera->viewMatrix);
        glUniformMatrix4fv(shadowMapper->defaultPass.uniform_location_biasedShadowMvpMatrix, 1, GL_FALSE,
                           biasedShadowMvpMatrix);
        glDisable(GL_CULL_FACE);
        if (game->isPaused || game->getGameTime() == 0 || game->isFinished()){
            shaderParams->setParam(shaderParams->overlay, 1);
        } else {
            shaderParams->setParam(shaderParams->overlay, 0);
        }
        ship->render(true);
        enviroment->renderSea(ship->px, ship->py, ship->pz, true);
        enviroment->renderBuoys();
        enviroment->drawSky();
        glPopMatrix();
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
    } else {
        ship->render(true);
        enviroment->renderSea(ship->px, ship->py, ship->pz, true);
        enviroment->renderBuoys();
        enviroment->drawSky();
        glPopMatrix();
    }

    frontier->render();
    game->detectCollision();
    if (!hud->bAskUsername) {
        hud->display(viewportWidth, viewportHeight, ship->px, -ship->pz, ship->facing, enviroment, fps);
    }
    if (hud->bAskUsername) {
        hud->askUsername(viewportWidth, viewportHeight, usenameInputBuffer);
    } else if (hud->bAskBuoys) {
        hud->askNumberOfBuoys(viewportWidth, viewportHeight, buoysCountInputBuffer);
    } else if (hud->isCommandsListVisibile) {
        hud->displayCommands(viewportWidth, viewportHeight);
    } else if (hud->isLeaderboardVisible) {
        hud->displayLeaderboard(game->getLeaderboard(), viewportWidth, viewportHeight);
    } else if (game->getGameTime() == 0) {
        hud->displayStartGameMessage(viewportWidth, viewportHeight);
    } else if (game->isFinished()) {
        hud->displayEndGameMessage(viewportWidth, viewportHeight);
    } else if (game->isPaused) {
        hud->displayPauseMessage(viewportWidth, viewportHeight);
    }

    glFinish();
    SDL_GL_SwapWindow(window);
}

int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_GLContext mainContext;
    Uint32 windowID;
    SDL_Joystick *joystick;
    static int keymap[Controller::NKEYS] = {SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN};

    // inizializzazione di SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    glutInit(&argc, argv);


    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if ((initted & flags) != flags) {
        printf("Failed to init SDL_IMG\n");
        return 1;
    }

    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // facciamo una finestra di scrW x scrH pixels
    window = SDL_CreateWindow(argv[0], 0, 0, viewportWidth, viewportHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_SetWindowTitle(window, (char *) "Open Sailing");
    glutInitWindowSize(viewportWidth, viewportHeight);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    if (!glfwInit()) {
        printf("Failed to init GLFW\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    int res = glewInit();
    if (res != GLEW_OK) {
        printf("Failed to init GLEW: %s\n", glewGetErrorString(res));
        return 1;
    }

    //Create our opengl context and attach it to our window
    mainContext = SDL_GL_CreateContext(window);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); // rinormalizza le normali prima di usarle
    glFrontFace(GL_CW); // consideriamo Front Facing le facce ClockWise
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Otherwise transparent objects are not displayed correctly

    textureManager = new TextureManager();
    shadowMapper = new ShadowMapper();
    shaderParams = new ShaderParams(shadowMapper);
    options = new Options();

    enviroment = new Enviroment(textureManager, shadowMapper, shaderParams, options);
    frontier = new Frontier(shaderParams, options);
    ship = new Ship(textureManager, shadowMapper, shaderParams, options, frontier);
    fog = new Fog();
    game = new Game(ship, enviroment);
    camera = new Camera();
    hud = new HUD(game);

    shadowMapper->InitGL();
    shadowMapper->ResizeGL(viewportWidth, viewportHeight);
    shadowMapper->resetCamera(eyeDist, viewBeta, viewAlpha);
    camera->set(*ship, eyeDist, viewBeta, viewAlpha);
    shadowMapper->resetLight();

    strcpy(usenameInputBuffer, "Lorenzo");
    sprintf(buoysCountInputBuffer, "%d", DEFAULT_BUOYS_COUNT);

    // Appena avviato il programma chiede l'inserimento dell'username
    inputModeEnabled = true;
    bool shiftButtonActive = true;           // Per gestire le maiuscole durante l'inserimento dell'username

    bool done = false;
    while (!done) {

        SDL_Event e;

        // guardo se c'e' un evento:
        if (SDL_PollEvent(&e)) {
            // se si: processa evento
            switch (e.type) {
                case SDL_KEYDOWN:
                    if (!inputModeEnabled) {
                        ship->controller.EatKey(e.key.keysym.sym, keymap, true);
                        if (e.key.keysym.sym == SDLK_F1) {
                            camera->change(*ship, eyeDist, viewBeta, viewAlpha);
                        } else if (e.key.keysym.sym == SDLK_F2) {
                            options->toggleWireframes();
                        } else if (e.key.keysym.sym == SDLK_F3) {
                            options->toggleShadows();
                        } else if (e.key.keysym.sym == SDLK_F4) {
                            options->toggleFog();
                        } else if (e.key.keysym.sym == SDLK_F5) {
                            options->toggleShaders();
                        } else if (e.key.keysym.sym == SDLK_b) {
                            hud->bAskBuoys = true;
                            inputModeEnabled = true;
                        } else if (e.key.keysym.sym == SDLK_c) {
                            hud->toggleCommandsList();
                        } else if (e.key.keysym.sym == SDLK_l) {
                            hud->toggleLeaderboard();
                        } else if (e.key.keysym.sym == SDLK_p && !hud->isCommandsListVisibile) {
                            game->togglePause();
                        } else if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN) {
                            game->go();
                        } else if (e.key.keysym.sym == SDLK_n) {
                            game->reset();
                        } else if (e.key.keysym.sym == SDLK_q){
                            done = true;
                        }
                    } else {
                        char *pEnd;
                        if (hud->bAskUsername && (e.key.keysym.sym == SDLK_LSHIFT || e.key.keysym.sym == SDLK_RSHIFT)) {
                            // Gestisce inserimento maiuscole per il nome utente
                            shiftButtonActive = true;
                        }
                        if (e.key.keysym.sym != SDLK_RETURN) {
                            if (hud->bAskUsername) {
                                const char keyboardM[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                                const char keyboard[] = "abcdefghijklmnopqrstuvwxyz";
                                if (e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_z) {
                                    if (shiftButtonActive) {
                                        strncat(usenameInputBuffer, &keyboardM[e.key.keysym.sym - SDLK_a], 1);
                                    } else {
                                        strncat(usenameInputBuffer, &keyboard[e.key.keysym.sym - SDLK_a], 1);
                                    }
                                } else if (e.key.keysym.sym == SDLK_BACKSPACE) {
                                    if (strlen(usenameInputBuffer) > 0) {
                                        usenameInputBuffer[strlen(usenameInputBuffer) - 1] = '\0';
                                    }
                                }
                            } else {
                                const char keyboard[] = "0123456789";
                                const char keypad[] = "1234567890";
                                if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) {
                                    strncat(buoysCountInputBuffer, &keyboard[e.key.keysym.sym - SDLK_0], 1);
                                } else if (e.key.keysym.sym >= SDLK_KP_1 && e.key.keysym.sym <= SDLK_KP_0) {
                                    strncat(buoysCountInputBuffer, &keypad[e.key.keysym.sym - SDLK_KP_1], 1);
                                } else if (e.key.keysym.sym == SDLK_BACKSPACE) {
                                    if (strlen(buoysCountInputBuffer) > 0) {
                                        buoysCountInputBuffer[strlen(buoysCountInputBuffer) - 1] = '\0';
                                    }
                                }
                            }
                        } else if (hud->bAskUsername) {
                            game->getLeaderboard()->setUsename(usenameInputBuffer);
                            hud->bAskUsername = false;
                            inputModeEnabled = false;
                        } else if (hud->bAskBuoys && strtol(buoysCountInputBuffer, &pEnd, 10) >= 1 &&
                                   strtol(buoysCountInputBuffer, &pEnd, 10) <= 100) {
                            enviroment->buoysCount = (int) strtol(buoysCountInputBuffer, &pEnd, 10);
                            hud->bAskBuoys = false;
                            inputModeEnabled = false;
                            game->reset();
                        }
                    }
                    break;
                case SDL_KEYUP:
                    if (hud->bAskUsername && (e.key.keysym.sym == SDLK_LSHIFT || e.key.keysym.sym == SDLK_RSHIFT)) {
                        // Gestisce inserimento maiuscole per il nome utente
                        shiftButtonActive = false;
                    }
                    ship->controller.EatKey(e.key.keysym.sym, keymap, false);
                    break;
                case SDL_QUIT:
                    done = true;
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
                                    viewportWidth = e.window.data1;
                                    viewportHeight = e.window.data2;
                                    rendering(window);
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
                        if (viewBeta > +90) viewBeta = +90;
                        shadowMapper->resetCamera(eyeDist, viewBeta * 0.005, -viewAlpha * 0.005);
                        rendering(window);
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
                    shadowMapper->resetCamera(eyeDist, viewBeta * 0.005, -viewAlpha * 0.005);
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
                if (!game->isPaused) {
                    ship->DoStep();
                }
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
            else {
                // tempo libero!!!
            }
        }
    }
    shadowMapper->DestroyGL();
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return (0);
}