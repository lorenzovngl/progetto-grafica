// https://github.com/Flix01/Tiny-OpenGL-Shadow-Mapping-Examples
/** License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

// DEPENDENCIES:
/*
-> glut or freeglut (the latter is recommended)
-> glew (Windows only)
*/

// HOW TO COMPILE:
/*
// LINUX:
gcc -O2 -std=gnu89 -no-pie my_shadow_mapping.c -o my_shadow_mapping -I"../" -lglut -lGL -lX11 -lm
// WINDOWS (here we use the static version of glew, and glut32.lib, that can be replaced by freeglut.lib):
cl /O2 /MT /Tc shadow_mapping.c /D"GLEW_STATIC" /I"../" /link /out:shadow_mapping.exe glut32.lib glew32s.lib opengl32.lib gdi32.lib Shell32.lib comdlg32.lib user32.lib kernel32.lib


// IN ADDITION:
By default the source file assumes that every OpenGL-related header is in "GL/".
But you can define in the command-line the correct paths you use in your system
for glut.h, glew.h, etc. with something like:
-DGLUT_PATH=\"Glut/glut.h\"
-DGLEW_PATH=\"Glew/glew.h\"
(this syntax works on Linux, don't know about Windows)
*/

//#define USE_GLEW  // By default it's only defined for Windows builds (but can be defined in Linux/Mac builds too)


#define PROGRAM_NAME "shadow_mapping"
#define VISUALIZE_DEPTH_TEXTURE
#define SHADOW_MAP_CLAMP_MODE GL_CLAMP_TO_EDGE // GL_CLAMP or GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER
    //          GL_CLAMP;               // sampling outside of the shadow map gives always shadowed pixels
    //          GL_CLAMP_TO_EDGE;       // sampling outside of the shadow map can give shadowed or unshadowed pixels (it depends on the edge of the shadow map)
    //          GL_CLAMP_TO_BORDER;     // sampling outside of the shadow map gives always non-shadowed pixels (if we set the border color correctly)
#define SHADOW_MAP_FILTER GL_LINEAR // GL_LINEAR or GL_NEAREST (GL_LINEAR is more useful with a sampler2DShadow, that cannot be used with esponential shadow mapping)

//#define USE_UNSTABLE_SHADOW_MAPPING_TECHNIQUE // Better resolution, but shadow-swimming as the camera rotates (on static objects). Please see README.md about it.
                                                // in camera ortho3d mode [F1], the resolution is further better, but shadow-swimming appears when zooming in-out too.

// These path definitions can be passed to the compiler command-line
#ifndef GLUT_PATH
#   define GLUT_PATH "GL/glut.h"    // Mandatory
#endif //GLEW_PATH
#ifndef FREEGLUT_EXT_PATH
#   define FREEGLUT_EXT_PATH "GL/freeglut_ext.h"    // Optional (used only if glut.h comes from the freeglut library)
#endif //GLEW_PATH
#ifndef GLEW_PATH
#   define GLEW_PATH "GL/glew.h"    // Mandatory for Windows only
#endif //GLEW_PATH

#ifdef _WIN32
#	include "windows.h"
#	define USE_GLEW
#endif //_WIN32

#ifdef USE_GLEW
#	include GLEW_PATH
#else //USE_GLEW
#	define GL_GLEXT_PROTOTYPES
#endif //USE_GLEW

#include GLUT_PATH
#ifdef __FREEGLUT_STD_H__
#   include FREEGLUT_EXT_PATH
#endif //__FREEGLUT_STD_H__

#define STR_MACRO(s) #s
#define XSTR_MACRO(s) STR_MACRO(s)


extern "C" {
    #include "helper_functions.h"
}
#include "ShadowMapper.h"
#include "../../src/headers/Utils.h"
// only very few of its functions are used.

#include <stdio.h>
#include <math.h>
#include <string.h>


// Config file handling: basically there's an .ini file next to the
// exe that you can tweak. (it's just an extra)
const char* ConfigFileName = PROGRAM_NAME".ini";
typedef struct {
    int fullscreen_width,fullscreen_height;
    int windowed_width,windowed_height;
    int fullscreen_enabled;
    int show_fps;
    int use_camera_ortho3d_projection_matrix;
} Config;
void Config_Init(Config* c) {
    c->fullscreen_width=c->fullscreen_height=0;
    c->windowed_width=960;c->windowed_height=540;
    c->fullscreen_enabled=0;
    c->show_fps = 0;
    c->use_camera_ortho3d_projection_matrix = 0;
}
int Config_Load(Config* c,const char* filePath)  {
    FILE* f = fopen(filePath, "rt");
    char ch='\0';char buf[256]="";
    size_t nread=0;
    int numParsedItem=0;
    if (!f)  return -1;
    while ((ch = fgetc(f)) !=EOF)    {
        buf[nread]=ch;
        nread++;
        if (nread>255) {
            nread=0;
            continue;
        }
        if (ch=='\n') {
           buf[nread]='\0';
           if (nread<2 || buf[0]=='[' || buf[0]=='#') {nread = 0;continue;}
           if (nread>2 && buf[0]=='/' && buf[1]=='/') {nread = 0;continue;}
           // Parse
           switch (numParsedItem)    {
               case 0:
               sscanf(buf, "%d %d", &c->fullscreen_width,&c->fullscreen_height);
               break;
               case 1:
               sscanf(buf, "%d %d", &c->windowed_width,&c->windowed_height);
               break;
               case 2:
               sscanf(buf, "%d", &c->fullscreen_enabled);
               break;
               case 3:
               sscanf(buf, "%d", &c->show_fps);
               break;
               case 4:
               sscanf(buf, "%d", &c->use_camera_ortho3d_projection_matrix);
               break;
           }
           nread=0;
           ++numParsedItem;
        }
    }
    fclose(f);
    if (c->windowed_width<=0) c->windowed_width=720;
    if (c->windowed_height<=0) c->windowed_height=405;
    return 0;
}
int Config_Save(Config* c,const char* filePath)  {
    FILE* f = fopen(filePath, "wt");
    if (!f)  return -1;
    fprintf(f, "[Size In Fullscreen Mode (zero means desktop size)]\n%d %d\n",c->fullscreen_width,c->fullscreen_height);
    fprintf(f, "[Size In Windowed Mode]\n%d %d\n",c->windowed_width,c->windowed_height);
    fprintf(f, "[Fullscreen Enabled (0 or 1) (CTRL+RETURN)]\n%d\n", c->fullscreen_enabled);
    fprintf(f, "[Show FPS (0 or 1) (F2)]\n%d\n", c->show_fps);
    fprintf(f, "[Use camera ortho3d projection matrix (0 or 1) (F1)]\n%d\n", c->use_camera_ortho3d_projection_matrix);
    fprintf(f,"\n");
    fclose(f);
    return 0;
}

Config config;

// glut has a special fullscreen GameMode that you can toggle with CTRL+RETURN (not in WebGL)
int windowId = 0; 			// window Id when not in fullscreen mode
int gameModeWindowId = 0;	// window Id when in fullscreen mode

// Now we can start with our program

float instantFrameTime = 16.2f;

char ShadowPassFragmentShader[5000];
char ShadowPassVertexShader[5000];

void ShadowMapper::InitShadowPass()	{
    Utils::loadFileIntoCharArray(ShadowPassFragmentShader, (char*) "lib/ShadowMapper/shaders/shadowPassFragShader.glsl");
    Utils::loadFileIntoCharArray(ShadowPassVertexShader, (char*) "lib/ShadowMapper/shaders/shadowPassVertShader.glsl");
    shadowPass.program = Helper_LoadShaderProgramFromSource(ShadowPassVertexShader, ShadowPassFragmentShader);

    // create depth texture
    glGenTextures(1, &shadowPass.textureId);
    glBindTexture(GL_TEXTURE_2D, shadowPass.textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, SHADOW_MAP_FILTER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, SHADOW_MAP_FILTER);
#   ifndef __EMSCRIPTEN__
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
#   else //__EMSCRIPTEN__
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0);
#   undef SHADOW_MAP_CLAMP_MODE
#   define SHADOW_MAP_CLAMP_MODE GL_CLAMP_TO_EDGE
#   endif //__EMSCRIPTEN__
    if (SHADOW_MAP_CLAMP_MODE==GL_CLAMP_TO_BORDER)  {
        const GLfloat border[] = {1.0f,1.0f,1.0f,0.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, SHADOW_MAP_CLAMP_MODE );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, SHADOW_MAP_CLAMP_MODE );
    glBindTexture(GL_TEXTURE_2D, 0);

    // create depth fbo
    glGenFramebuffers(1, &shadowPass.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowPass.fbo);
#   ifndef __EMSCRIPTEN__
    glDrawBuffer(GL_NONE); // Instruct openGL that we won't bind a color texture with the currently bound FBO
    glReadBuffer(GL_NONE);
#   endif //__EMSCRIPTEN__
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,shadowPass.textureId, 0);
    {
        //Does the GPU support current FBO configuration?
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status!=GL_FRAMEBUFFER_COMPLETE) printf("glCheckFramebufferStatus(...) FAILED for shadowPass.fbo.\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void ShadowMapper::DestroyShadowPass()	{
    if (shadowPass.program) {glDeleteProgram(shadowPass.program);shadowPass.program=0;}
    if (shadowPass.fbo) {glDeleteBuffers(1,&shadowPass.fbo);shadowPass.fbo=0;}
    if (shadowPass.textureId) {glDeleteTextures(1,&shadowPass.textureId);}
}

char DefaultPassFragmentShader[5000];
char DefaultPassVertexShader[5000];

void ShadowMapper::InitDefaultPass()	{
    Utils::loadFileIntoCharArray(DefaultPassFragmentShader, (char*) "lib/ShadowMapper/shaders/defaultPassFragShader.glsl");
    Utils::loadFileIntoCharArray(DefaultPassVertexShader, (char*) "lib/ShadowMapper/shaders/defaultPassVertShader.glsl");
	defaultPass.program = Helper_LoadShaderProgramFromSource(DefaultPassVertexShader, DefaultPassFragmentShader);
    defaultPass.uniform_location_biasedShadowMvpMatrix = glGetUniformLocation(defaultPass.program,"u_biasedShadowMvpMatrix");
    defaultPass.uniform_location_shadowMap = glGetUniformLocation(defaultPass.program,"u_shadowMap");
    defaultPass.uniform_location_shadowDarkening = glGetUniformLocation(defaultPass.program,"u_shadowDarkening");

    glUseProgram(defaultPass.program);
    glUniform1i(defaultPass.uniform_location_shadowMap,0);
    glUniform2f(defaultPass.uniform_location_shadowDarkening,80.0,0.45);	// Default values are (40.0f,0.75f) in [0-80] and [0-1]
    //glUniformMatrix4fv(defaultPass.uniform_location_biasedShadowMvpMatrix, 1 /*only setting 1 matrix*/, GL_FALSE /*transpose?*/, Matrix);
	glUseProgram(0);
}
void ShadowMapper::DestroyDefaultPass()	{
    printf("DestroyDefaulyPass() called\n");
	if (defaultPass.program) {glDeleteProgram(defaultPass.program);defaultPass.program=0;}
}

float current_width=0,current_height=0,current_aspect_ratio=1;  // Not sure when I've used these...
void ShadowMapper::ResizeGL(int w, int h) {
    current_width = (float) w;
    current_height = (float) h;
    if (current_height!=0) current_aspect_ratio = current_width/current_height;
    if (h>0)	{
        // We set our pMatrix
        if (!config.use_camera_ortho3d_projection_matrix)
            Helper_Perspective(pMatrix,pMatrixFovyDeg,(float)w/(float)h,pMatrixNearPlane,pMatrixFarPlane);
        else
            Helper_Ortho3D(pMatrix,cameraDistance,pMatrixFovyDeg,(float)w/(float)h,pMatrixNearPlane,pMatrixFarPlane);

        glMatrixMode(GL_PROJECTION);glLoadMatrixf(pMatrix);glMatrixMode(GL_MODELVIEW);

#       ifdef USE_UNSTABLE_SHADOW_MAPPING_TECHNIQUE
        Helper_InvertMatrix(pMatrixInverse,pMatrix);    // pMatrixInverse is used only when USE_UNSTABLE_SHADOW_MAPPING_TECHNIQUE is defined
#       endif //USE_UNSTABLE_SHADOW_MAPPING_TECHNIQUE
	}


    if (w>0 && h>0 && !config.fullscreen_enabled) {
        // On exiting we'll like to save these data back
        config.windowed_width=w;
        config.windowed_height=h;
    }

    glViewport(0,0,w,h);    // This is what people often call in ResizeGL()

}


void ShadowMapper::InitGL() {

    // These are important, but often overlooked OpenGL calls
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Otherwise transparent objects are not displayed correctly
    glClearColor(0.3f, 0.6f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);    // Only needed for ffp, when VISUALIZE_DEPTH_TEXTURE is defined


	// ffp stuff
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

	// New
    InitShadowPass();
    InitDefaultPass();

    // Please note that after InitGL(), this implementation calls ResizeGL(...,...).
    // If you copy/paste this code you can call it explicitly...
}

void ShadowMapper::DestroyGL() {
	// New
    DestroyShadowPass();
    DestroyDefaultPass();
    // 40 display lists are generated by Helper_GlutDrawGeometry(...) if pgDisplayListBase!=0
    if (pgDisplayListBase && *pgDisplayListBase) {glDeleteLists(*pgDisplayListBase,40);*pgDisplayListBase=0;}
}

static void GlutDestroyWindow();
static void GlutCreateWindow();

void GlutCloseWindow()  {Config_Save(&config,ConfigFileName);}

void ShadowMapper::updateCameraPos() {
    const float distanceY = sin(cameraPitch)*cameraDistance;
    const float distanceXZ = cos(cameraPitch)*cameraDistance;
    cameraPos[0] = targetPos[0] + sin(cameraYaw)*distanceXZ;
    cameraPos[1] = targetPos[1] + distanceY;
    cameraPos[2] = targetPos[2] + cos(cameraYaw)*distanceXZ;
}

void ShadowMapper::updateDirectionalLight() {
    const float distanceY = sin(lightPitch);
    const float distanceXZ = cos(lightPitch);
    lightDirection[0] = sin(lightYaw)*distanceXZ;
    lightDirection[1] = distanceY;
    lightDirection[2] = cos(lightYaw)*distanceXZ;
    Helper_Vector3Normalize(lightDirection);
    lightDirection[3]=0.f;
}

void ShadowMapper::resetCamera(float eyeDist, float viewBeta, float viewAlpha) {
    // You can set the initial camera position here through:
    targetPos[0]=0; targetPos[1]=0; targetPos[2]=0; // The camera target point
    //cameraYaw = 2*M_PI;                             // The camera rotation around the Y axis
    cameraYaw = viewAlpha;
    //cameraPitch = M_PI*0.125f;                      // The camera rotation around the XZ plane
    cameraPitch = viewBeta;
    //cameraDistance = 5;                             // The distance between the camera position and the camera target point
    cameraDistance = eyeDist;

    updateCameraPos();
    if (config.use_camera_ortho3d_projection_matrix)    ShadowMapper::ResizeGL(current_width, current_height); // Needed because in Helper_Orho3D(...) cameraTargetDistance changes
}

void ShadowMapper::resetLight() {
    lightYaw = M_PI*0.425f;
    lightPitch = M_PI*0.235f;
    updateDirectionalLight();
}

void ShadowMapper::showShadowMask(int vpHeight, int vpWidth){

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
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, shadowPass.textureId);
        glColor4f(1,1,1,0.9f);
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);glVertex2f(-1, -1);
        glTexCoord2f(1,0);glVertex2f(-0.25*(vpWidth/vpHeight), -1);
        glTexCoord2f(1,1);glVertex2f(-0.25*(vpWidth/vpHeight), -0.25/(vpWidth/vpHeight));
        glTexCoord2f(0,1);glVertex2f(-1, -0.25/(vpWidth/vpHeight));
        glEnd();
        glBindTexture(GL_TEXTURE_2D,0);
        glDisable(GL_TEXTURE_2D);
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