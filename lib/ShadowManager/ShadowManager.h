#ifndef PROGETTOGRAFICA_SHADOWMANAGER_H
#define PROGETTOGRAFICA_SHADOWMANAGER_H

#include <GL/gl.h>

typedef struct {
    GLuint fbo;
    GLuint textureId;
    GLuint program;
} ShadowPass;

typedef struct {
    GLuint program;
    GLint uniform_location_biasedShadowMvpMatrix;
    GLint uniform_location_shadowMap;
    GLint uniform_location_shadowDarkening;
} DefaultPass;

class ShadowManager {
public:
    // camera data:
    float targetPos[3];         // please set it in resetCamera()
    float cameraYaw;            // please set it in resetCamera()
    float cameraPitch;          // please set it in resetCamera()
    float cameraDistance;       // please set it in resetCamera()
    float cameraPos[3];         // Derived value (do not edit)
    float vMatrix[16];          // view matrix
    float cameraSpeed = 0.5f;   // When moving it

// light data
    float lightYaw = M_PI*0.425f,lightPitch = M_PI*0.235f;   // must be copied to resetLight() too
    float lightDirection[4] = {0,1,0,0};                    // Derived value (do not edit) [lightDirection[3]==0]

// pMatrix data:
    float pMatrix[16],pMatrixInverse[16];   // projection matrix (pMatrixInverse is used only when USE_UNSTABLE_SHADOW_MAPPING_TECHNIQUE is defined)
    const float pMatrixFovyDeg = 45.f;      // smaller => better shadow resolution
    const float pMatrixNearPlane = 0.5f;    // bigger  => better shadow resolution
    const float pMatrixFarPlane = 20.f;     // smaller => better shadow resolution

// Optional (to speed up Helper_GlutDrawGeometry(...) a bit)
    GLuint gDisplayListBase = 0;GLuint* pgDisplayListBase = &gDisplayListBase;  // Can be set to 0 as a fallback.

    ShadowPass shadowPass;
    DefaultPass defaultPass;
    void InitGL();
    void DestroyGL();
    void DrawGL();
    void ResizeGL(int w,int h);
    void InitShadowPass();
    void DestroyShadowPass();
    void InitDefaultPass();
    void DestroyDefaultPass();
    void resetCamera();  // Mandatory
    void resetLight();   // Mandatory
    void updateCameraPos();
    void updateDirectionalLight();
};

#endif