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
        if (!((i >= 15100 && i <= 15500) || (i >= 16600 && i <= 17000))) {
            // Disegno a parte le eliche e i timoni
            glColor3f(1, 1, 1);
            f[i].n.SendAsNormal(); // flat shading
            (f[i].v[0])->p.SendAsVertex();
            (f[i].v[1])->p.SendAsVertex();
            (f[i].v[2])->p.SendAsVertex();
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}

void ShipMesh::drawHelixes(){
    float offset[3] = {-19, 1.75, 1.45};
    setupTexture(4, bbmin, bbmax);
    // mandiamo tutti i triangoli a schermo
    glBegin(GL_TRIANGLES);
    for (int i = 15100; i <= 15500; i++) {
        glColor3f(1, 1, 1);
        f[i].n.SendAsNormal(); // flat shading
        if (f[i].v[0]->p.coord[2] < 0){
            // Ne disegno solo una (poi la duplico), l'altra non mi serve
            glVertex3f(f[i].v[0]->p.coord[0] + offset[0], f[i].v[0]->p.coord[1] + offset[1], f[i].v[0]->p.coord[2] + offset[2]);
            glVertex3f(f[i].v[1]->p.coord[0] + offset[0], f[i].v[1]->p.coord[1] + offset[1], f[i].v[1]->p.coord[2] + offset[2]);
            glVertex3f(f[i].v[2]->p.coord[0] + offset[0], f[i].v[2]->p.coord[1] + offset[1], f[i].v[2]->p.coord[2] + offset[2]);
        }
    }
    for (int i = 34001; i <= 34250; i++) {
        glColor3f(1, 1, 1);
        f[i].n.SendAsNormal(); // flat shading
        if (f[i].v[0]->p.coord[2] < 0){
            // Ne disegno solo una (poi la duplico), l'altra non mi serve
            glVertex3f(f[i].v[0]->p.coord[0] + offset[0], f[i].v[0]->p.coord[1] + offset[1], f[i].v[0]->p.coord[2] + offset[2]);
            glVertex3f(f[i].v[1]->p.coord[0] + offset[0], f[i].v[1]->p.coord[1] + offset[1], f[i].v[1]->p.coord[2] + offset[2]);
            glVertex3f(f[i].v[2]->p.coord[0] + offset[0], f[i].v[2]->p.coord[1] + offset[1], f[i].v[2]->p.coord[2] + offset[2]);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}

void ShipMesh::drawRudders(){
    float offset[3] = {-21.5, 0.3, 1.5};
    setupTexture(4, bbmin, bbmax);
    // mandiamo tutti i triangoli a schermo
    glBegin(GL_TRIANGLES);
    for (int i = 16600; i <= 17000; i++) {
        glColor3f(1, 1, 1);
        f[i].n.SendAsNormal(); // flat shading
        if (f[i].v[0]->p.coord[2] < 0){
            // Ne disegno solo una (poi la duplico), l'altra non mi serve
            glVertex3f(f[i].v[0]->p.coord[0] + offset[0], f[i].v[0]->p.coord[1] + offset[1], f[i].v[0]->p.coord[2] + offset[2]);
            glVertex3f(f[i].v[1]->p.coord[0] + offset[0], f[i].v[1]->p.coord[1] + offset[1], f[i].v[1]->p.coord[2] + offset[2]);
            glVertex3f(f[i].v[2]->p.coord[0] + offset[0], f[i].v[2]->p.coord[1] + offset[1], f[i].v[2]->p.coord[2] + offset[2]);
        }
    }
    for (int i = 35800; i <= 36250; i++) {
        glColor3f(1, 1, 1);
        f[i].n.SendAsNormal(); // flat shading
        if (f[i].v[0]->p.coord[2] < 0){
            // Ne disegno solo una (poi la duplico), l'altra non mi serve
            glVertex3f(f[i].v[0]->p.coord[0] + offset[0], f[i].v[0]->p.coord[1] + offset[1], f[i].v[0]->p.coord[2] + offset[2]);
            glVertex3f(f[i].v[1]->p.coord[0] + offset[0], f[i].v[1]->p.coord[1] + offset[1], f[i].v[1]->p.coord[2] + offset[2]);
            glVertex3f(f[i].v[2]->p.coord[0] + offset[0], f[i].v[2]->p.coord[1] + offset[1], f[i].v[2]->p.coord[2] + offset[2]);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}

void ShipMesh::render(float speed, float angle) {
    char *filepath = (char *) "assets/ship/textures.dat";
    FILE *file;
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
    while (fgets(line, 100, file) != NULL) {
        if (line[0] != '#' && line[0] != '\n') {                    // Avoid comments and blank lines
            sscanf(line, "%d - %d -> %d\n", &start, &end, &tex);
            drawTriangles(tex, start, end);
            // Elica di sinistra
            glPushMatrix();
            glTranslatef(19, -1.75, 1.45);
            glRotatef(SDL_GetTicks()*speed, 1, 0, 0);
            drawHelixes();
            glPopMatrix();
            // Elica di destra
            glPushMatrix();
            glTranslatef(19, -1.75, -1.45);
            glRotatef(SDL_GetTicks()*speed, 1, 0, 0);
            drawHelixes();
            glPopMatrix();
            // Timone di sinistra
            glPushMatrix();
            glTranslatef(21.5, -0.3, 1.5);
            glRotatef(angle, 0, 1, 0);
            drawRudders();
            glPopMatrix();
            // Timone di destra
            glPushMatrix();
            glTranslatef(21.5, -0.3, -1.5);
            glRotatef(angle, 0, 1, 0);
            drawRudders();
            glPopMatrix();
        }
    }
    fclose(file);
    //displayBoundingBox();
}
