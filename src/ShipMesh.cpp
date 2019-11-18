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
#include "headers/Utils.h"

void ShipMesh::initModel() {
    model_file = (char *) "assets/ship/fishing_ship.obj";
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
    textureManager->loadTexture(TEXTURE_DECK_WOODEN_FLOOR);
    textureManager->loadTexture(TEXTURE_TOP_BODY_BLACK_METAL);
    textureManager->loadTexture(TEXTURE_TOP_BODY_WHITE_METAL);
    textureManager->loadTexture(TEXTURE_BOTTOM_BODY_DARK_RED_METAL);
}

// funzione che prepara tutto per creare le coordinate texture (s,t) da (x,y,z)
// Mappo l'intervallo [ minY , maxY ] nell'intervallo delle T [0..1]
//     e l'intervallo [ minZ , maxZ ] nell'intervallo delle S [0..1]
void ShipMesh::setupTexture(GLenum n_texture, Point3 min, Point3 max) {
    //glActiveTexture(n_texture);
    textureManager->enableTexture(n_texture);
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    // ulilizzo le coordinate OGGETTO
    // cioe' le coordnate originali, PRIMA della moltiplicazione per la ModelView
    // in modo che la texture sia "attaccata" all'oggetto, e non "proiettata" su esso
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    float s[4] = {0.1, 0, 0, 0};
    float t[4] = {0, 0, 0.1, 0};
    glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, t);
}

void ShipMesh::drawTriangles(int gl_texture, int start, int end) {
    setupTexture(gl_texture, bbmin, bbmax);
    // mandiamo tutti i triangoli a schermo
    glBegin(GL_TRIANGLES);
    for (int i = start; i <= end; i++) {
        /*float dx = std::max(std::max(f[i].v[0]->p.X(), f[i].v[1]->p.X()), f[i].v[2]->p.X());
        float dy = std::max(std::max(f[i].v[0]->p.Y(), f[i].v[1]->p.Y()), f[i].v[2]->p.Y());
        float dz = std::max(std::max(f[i].v[0]->p.Z(), f[i].v[1]->p.Z()), f[i].v[2]->p.Z());
        float s[4] = {0, 0, 0, 0};
        float t[4] = {0, 0, 0, 0};
        if (dx < dy){
            if (dx < dz){
                // DX è minore di DY e DZ -> faccia rivolta verso l'asse X
                printf("Asse X\n");
                s[1] = 0.1;
                t[2] = 0.1;
            } else {
                // DZ è minore di DX e DY -> faccia rivolta verso l'asse Z
                printf("Asse Z\n");
                s[0] = 0.1;
                t[1] = 0.1;
            }
        } else {
            if (dy < dz){
                // DY è minore di DX e DZ -> faccia rivolta verso l'asse Y
                printf("Asse Y\n");
                s[0] = 0.1;
                t[2] = 0.1;
            } else {
                // DZ è minore di DX e DY -> faccia rivolta verso l'asse Z
                printf("Asse Z\n");
                s[0] = 0.1;
                t[1] = 0.1;
            }
        }
        glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, t);*/
        glColor3f(1, 1, 1);
        f[i].n.SendAsNormal(); // flat shading
        (f[i].v[0])->p.SendAsVertex();
        (f[i].v[1])->p.SendAsVertex();
        (f[i].v[2])->p.SendAsVertex();
    }
    glEnd();
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}

void ShipMesh::render() {
    char* filepath = (char*) "assets/ship/textures.dat";
    FILE* file;
    int start, end, tex;
    char line[100];
    if (model_file == NULL) {
        initModel();
    }
    file = fopen(filepath, "r");
    if (!file) {
        perror(filepath);
        return;
    }
    while (fgets(line, 100, file) != NULL){
        if (line[0] != '#' && line[0] != '\n'){                    // Avoid comments and blank lines
            sscanf(line, "%d - %d -> %d\n", &start, &end, &tex);
            //printf("%d - %d -> %d\n", start, end, tex);
            drawTriangles(tex, start, end);
        }
    }
    fclose(file);
    //displayBoundingBox();
}
