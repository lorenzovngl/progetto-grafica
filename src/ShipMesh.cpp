#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>
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

#include "../lib/GCGraLib2/GCGraLib2.h"
#include "headers/Mesh.h"
#include "headers/texture.h"
#include "headers/glm.h"
#include "headers/Texture.h"
#include "headers/ShipMesh.h"

void ShipMesh::initModel()
{
    model_file = (char*)"assets/ship/fishing_ship.obj";
    /* read in the model */
    model = glmReadOBJ(model_file);
    scale = glmUnitize(model);
    glmFacetNormals(model);
    glmVertexNormals(model, smoothing_angle);

    if (model->nummaterials > 0)
        material_mode = 2;

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE2);
    m_texture[0] = new Texture(TEXTURE_DECK_WOODEN_FLOOR);
    m_texture[0]->loadTexture();
    glBindTexture(GL_TEXTURE_2D, m_texture[0]->getBind());

    glActiveTexture(GL_TEXTURE3);
    m_texture[1] = new Texture(TEXTURE_TOP_BODY_BLACK_METAL);
    m_texture[1]->loadTexture();
    glBindTexture(GL_TEXTURE_2D, m_texture[1]->getBind());

    glActiveTexture(GL_TEXTURE4);
    m_texture[2] = new Texture(TEXTURE_TOP_BODY_WHITE_METAL);
    m_texture[2]->loadTexture();
    glBindTexture(GL_TEXTURE_2D, m_texture[1]->getBind());

    glActiveTexture(GL_TEXTURE5);
    m_texture[3] = new Texture(TEXTURE_BOTTOM_BODY_DARK_RED_METAL);
    m_texture[3]->loadTexture();
    glBindTexture(GL_TEXTURE_2D, m_texture[1]->getBind());
}

// funzione che prepara tutto per creare le coordinate texture (s,t) da (x,y,z)
// Mappo l'intervallo [ minY , maxY ] nell'intervallo delle T [0..1]
//     e l'intervallo [ minZ , maxZ ] nell'intervallo delle S [0..1]
void setupTexture(GLenum n_texture, Point3 min, Point3 max){
    glActiveTexture(n_texture);
    glEnable(GL_TEXTURE_2D);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    // ulilizzo le coordinate OGGETTO
    // cioe' le coordnate originali, PRIMA della moltiplicazione per la ModelView
    // in modo che la texture sia "attaccata" all'oggetto, e non "proiettata" su esso
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE , GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE , GL_OBJECT_LINEAR);
    float sz=1.0/(max.Z() - min.Z());
    float ty=1.0/(max.Y() - min.Y());
    float s[4]={0,0,sz,  - min.Z()*sz};
    float t[4]={0,ty,0,  - min.Y()*ty};
    glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, t);
}

// Permette di indicare gli indici dell'array in termini percentuali sulla sua lunghezza
int indexPercent(double request, int lenght){
    return request * lenght / 100;
}

void ShipMesh::applyTexture(int gl_texture, int start, int end){
    setupTexture(gl_texture, bbmin, bbmax);
    // mandiamo tutti i triangoli a schermo
    glBegin(GL_TRIANGLES);
    for (int i = start; i < end; i++) {
        f[i].n.SendAsNormal(); // flat shading
        //glTexCoord2f((f[i].v[0])->p.X(),(f[i].v[0])->p.Y());
        (f[i].v[0])->p.SendAsVertex();
        //glTexCoord2f((f[i].v[1])->p.X(),(f[i].v[1])->p.Y());
        (f[i].v[1])->p.SendAsVertex();
        //glTexCoord2f((f[i].v[2])->p.X(),(f[i].v[2])->p.Y());
        (f[i].v[2])->p.SendAsVertex();
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void ShipMesh::render() {
    if (model_file == NULL){
        initModel();
    }
    //applyTexture(GL_TEXTURE2, 0, indexPercent(98, f.size()) - 1);
    // Bottom body dark red metal
    applyTexture(GL_TEXTURE5, indexPercent(98, f.size()), f.size());
    printf("%d\n", f.size());
}
