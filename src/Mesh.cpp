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
#include "headers/texture.h"
#include "headers/glm.h"
#include "headers/Texture.h"
#include "headers/Mesh.h"
#include "headers/Utils.h"

void Mesh::ComputeNormalsPerFace() {
    for (int i = 0; i < f.size(); i++) {
        f[i].ComputeNormal();
    }
}

void Mesh::lists(){
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat shininess = 65.0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    if (model_list)
        glDeleteLists(model_list, 1);

//    glDisable(GL_TEXTURE_2D);
    /* generate a list */
    switch (material_mode)
    {
        case 0:
            if (facet_normal)
                model_list = glmList(model, GLM_FLAT);
            else
                model_list = glmList(model, GLM_SMOOTH);
            break;
        case 1:
            if (facet_normal)
                model_list = glmList(model, GLM_FLAT | GLM_COLOR);
            else
                model_list = glmList(model, GLM_SMOOTH | GLM_COLOR);
            break;
        case 2:
            if (facet_normal)
                model_list = glmList(model, GLM_FLAT | GLM_MATERIAL);
            else
                model_list = glmList(model, GLM_SMOOTH | GLM_MATERIAL);
            break;
        case 3:
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,1);
            model_list = glmList(model, GLM_TEXTURE);
            glDisable(GL_TEXTURE_2D);
            break;
    }
}

void Mesh::initModel()
{
    model_file = (char*)"assets/ship/fishing_ship.obj";
    /* read in the model */
    model = glmReadOBJ(model_file);
    scale = glmUnitize(model);
    glmFacetNormals(model);
    glmVertexNormals(model, smoothing_angle);

    if (model->nummaterials > 0)
        material_mode = 2;

    /* create new display lists */
    lists();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}

void Mesh::render() {
    if (model_file == NULL){
        initModel();
    }

    // mandiamo tutti i triangoli a schermo
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < f.size(); i++) {
        f[i].n.SendAsNormal(); // flat shading
        (f[i].v[0])->p.SendAsVertex();
        (f[i].v[1])->p.SendAsVertex();
        (f[i].v[2])->p.SendAsVertex();
    }
    glEnd();
    displayBoundingBox();

}

unsigned char* ppmRead(char* filename, int* width, int* height) {
    FILE* fp;
    int i, w, h, d;
    unsigned char* image;
    char head[70];			/* max line <= 70 in PPM (per spec). */

    fp = fopen(filename, "rb");
    if (!fp) {
        perror(filename);
        return NULL;
    }

    /* grab first two chars of the file and make sure that it has the
       correct magic cookie for a raw PPM file. */
    fgets(head, 70, fp);
    if (strncmp(head, "P6", 2)) {
        fprintf(stderr, "%s: Not a raw PPM file\n", filename);
        return NULL;
    }

    /* grab the three elements in the header (width, height, maxval). */
    i = 0;
    while(i < 3) {
        fgets(head, 70, fp);
        if (head[0] == '#')		/* skip comments. */
            continue;
        if (i == 0)
            i += sscanf(head, "%d %d %d", &w, &h, &d);
        else if (i == 1)
            i += sscanf(head, "%d %d", &h, &d);
        else if (i == 2)
            i += sscanf(head, "%d", &d);
    }

    /* grab all the image data in one fell swoop. */
    image = (unsigned char*)malloc(sizeof(unsigned char)*w*h*3);
    fread(image, sizeof(unsigned char), w*h*3, fp);
    fclose(fp);

    *width = w;
    *height = h;
    return image;
}


void Mesh::Texturize() {
    GLubyte* texture;
    char texnames[1][64] = {"assets/militare.ppm"};
    GLint w,h;
    SDL_Surface *tex;
    SDL_RWops *rwop;

    /*tex = GC_LoadImage("/home/lorenzo/Scrivania/grafica/ProgettoGrafica/assets/ship/bottom_body_dark_red_metal.tga", 0);
    rwop = SDL_RWFromFile("/home/lorenzo/Scrivania/grafica/ProgettoGrafica/assets/ship/bottom_body_dark_red_metal.tga",
                          "rb");
    tex = IMG_LoadTGA_RW(rwop);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwid, texht, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
    free(tex);*/

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* XXX - RE bug - must enable texture before bind. */
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 1);
    texture = (GLubyte*)ppmRead(texnames[0], &w, &h);
//	   glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h,
                      GL_RGB, GL_UNSIGNED_BYTE, texture);
    free(texture);

    /* XXX - RE bug - must enable texture before bind. */
    glDisable(GL_TEXTURE_2D);
}

void Mesh::ComputeBoundingBox(float px, float py, float pz, float facing) {
    if (!v.size()) return;
    bbmin = bbmax = v[0].p;
    for (int i = 0; i < v.size(); i++) {
        for (int k = 0; k < 3; k++) {
            if (bbmin.coord[k] > v[i].p.coord[k]) bbmin.coord[k] = v[i].p.coord[k];
            if (bbmax.coord[k] < v[i].p.coord[k]) bbmax.coord[k] = v[i].p.coord[k];
        }
    }

    // Coordinate globali
    w_bbmin = new Point3(px, py, pz);
    *w_bbmin = Utils::localToWorldCoords(bbmin, *w_bbmin, facing);
    w_bbmax = new Point3(px, py, pz);
    *w_bbmax = Utils::localToWorldCoords(bbmax, *w_bbmax, facing);
    //printf("Local: %f. World: %f\n", carlinga.bbmin.Z(), point3->Z());
}

void Mesh::displayBoundingBox(){
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);

    // Quadrante Z-MIN

    glVertex3f(bbmin.X(), bbmin.Y(), bbmin.Z());
    glVertex3f(bbmin.X(), bbmax.Y(), bbmin.Z());

    glVertex3f(bbmin.X(), bbmin.Y(), bbmin.Z());
    glVertex3f(bbmax.X(), bbmin.Y(), bbmin.Z());

    glVertex3f(bbmax.X(), bbmin.Y(), bbmin.Z());
    glVertex3f(bbmax.X(), bbmax.Y(), bbmin.Z());

    glVertex3f(bbmax.X(), bbmax.Y(), bbmin.Z());
    glVertex3f(bbmin.X(), bbmax.Y(), bbmin.Z());

    // Quadrante Z-MAX

    glVertex3f(bbmin.X(), bbmin.Y(), bbmax.Z());
    glVertex3f(bbmin.X(), bbmax.Y(), bbmax.Z());

    glVertex3f(bbmin.X(), bbmin.Y(), bbmax.Z());
    glVertex3f(bbmax.X(), bbmin.Y(), bbmax.Z());

    glVertex3f(bbmax.X(), bbmin.Y(), bbmax.Z());
    glVertex3f(bbmax.X(), bbmax.Y(), bbmax.Z());

    glVertex3f(bbmax.X(), bbmax.Y(), bbmax.Z());
    glVertex3f(bbmin.X(), bbmax.Y(), bbmax.Z());

    // Quadrante X-MIN

    glVertex3f(bbmax.X(), bbmin.Y(), bbmin.Z());
    glVertex3f(bbmax.X(), bbmin.Y(), bbmax.Z());

    glVertex3f(bbmax.X(), bbmax.Y(), bbmin.Z());
    glVertex3f(bbmax.X(), bbmax.Y(), bbmax.Z());

    // Quadrante X-MAX

    glVertex3f(bbmin.X(), bbmin.Y(), bbmin.Z());
    glVertex3f(bbmin.X(), bbmin.Y(), bbmax.Z());

    glVertex3f(bbmin.X(), bbmax.Y(), bbmin.Z());
    glVertex3f(bbmin.X(), bbmax.Y(), bbmax.Z());

    glEnd();
}

bool Mesh::LoadFromObj(char *filename) {

    FILE *file = fopen(filename, "rt"); // apriamo il file in lettura
    if (!file) return false;

    //make a first pass through the file to get a count of the number
    //of vertices, normals, texcoords & triangles
    char buf[128];
    int nv, nf, nt;
    float x, y, z;
    int va, vb, vc, vd;
    int na, nb, nc, nd;
    int ta, tb, tc, td;

    nv = 0;
    nf = 0;
    nt = 0;
    while (fscanf(file, "%s", buf) != EOF) {
        switch (buf[0]) {
            case '#':               // comment
                // eat up rest of line
                fgets(buf, sizeof(buf), file);
                break;
            case 'v':               // v, vn, vt
                switch (buf[1]) {
                    case '\0':          // vertex
                        // eat up rest of line
                        fgets(buf, sizeof(buf), file);
                        nv++;
                        break;
                    default:
                        break;
                }
                break;
            case 'f':               // face
                fscanf(file, "%s", buf);
                // can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d
                if (strstr(buf, "//")) {
                    // v//n
                    sscanf(buf, "%d//%d", &va, &na);
                    fscanf(file, "%d//%d", &vb, &nb);
                    fscanf(file, "%d//%d", &vc, &nc);
                    nf++;
                    nt++;
                    while (fscanf(file, "%d//%d", &vd, &nd) > 0) {
                        nt++;
                    }
                } else if (sscanf(buf, "%d/%d/%d", &va, &ta, &na) == 3) {
                    // v/t/n
                    fscanf(file, "%d/%d/%d", &vb, &tb, &nb);
                    fscanf(file, "%d/%d/%d", &vc, &tc, &nc);
                    nf++;
                    nt++;
                    while (fscanf(file, "%d/%d/%d", &vd, &td, &nd) > 0) {
                        nt++;
                    }
                } else if (sscanf(buf, "%d/%d", &va, &ta) == 2) {
                    // v/t
                    fscanf(file, "%d/%d", &vb, &tb);
                    fscanf(file, "%d/%d", &vc, &tc);
                    nf++;
                    nt++;
                    while (fscanf(file, "%d/%d", &vd, &td) > 0) {
                        nt++;
                    }
                } else {
                    // v
                    fscanf(file, "%d", &va);
                    fscanf(file, "%d", &vb);
                    nf++;
                    nt++;
                    while (fscanf(file, "%d", &vc) > 0) {
                        nt++;
                    }
                }
                break;

            default:
                // eat up rest of line
                fgets(buf, sizeof(buf), file);
                break;
        }
    }

//printf("dopo FirstPass nv=%d nf=%d nt=%d\n",nv,nf,nt);

    // allochiamo spazio per nv vertici
    v.resize(nv);

    // rewind to beginning of file and read in the data this pass
    rewind(file);

    //on the second pass through the file, read all the data into the
    //allocated arrays

    nv = 0;
    nt = 0;
    while (fscanf(file, "%s", buf) != EOF) {
        switch (buf[0]) {
            case '#':               // comment
                // eat up rest of line
                fgets(buf, sizeof(buf), file);
                break;
            case 'v':               // v, vn, vt
                switch (buf[1]) {
                    case '\0':          // vertex
                        fscanf(file, "%f %f %f", &x, &y, &z);
                        v[nv].p = Point3(x, y, z);
                        nv++;
                        break;
                    default:
                        break;
                }
                break;
            case 'f':               // face
                fscanf(file, "%s", buf);
                // can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d
                if (strstr(buf, "//")) {
                    // v//n
                    sscanf(buf, "%d//%d", &va, &na);
                    fscanf(file, "%d//%d", &vb, &nb);
                    fscanf(file, "%d//%d", &vc, &nc);
                    va--;
                    vb--;
                    vc--;
                    Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                    f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
                    nt++;
                    vb = vc;
                    while (fscanf(file, "%d//%d", &vc, &nc) > 0) {
                        vc--;
                        Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                        f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
                        nt++;
                        vb = vc;
                    }
                } else if (sscanf(buf, "%d/%d/%d", &va, &ta, &na) == 3) {
                    // v/t/n
                    fscanf(file, "%d/%d/%d", &vb, &tb, &nb);
                    fscanf(file, "%d/%d/%d", &vc, &tc, &nc);
                    va--;
                    vb--;
                    vc--;
                    Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                    f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
                    nt++;
                    vb = vc;
                    while (fscanf(file, "%d/%d/%d", &vc, &tc, &nc) > 0) {
                        vc--;
                        Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                        f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
                        nt++;
                        vb = vc;
                    }
                } else if (sscanf(buf, "%d/%d", &va, &ta) == 2) {
                    // v/t
                    fscanf(file, "%d/%d", &va, &ta);
                    fscanf(file, "%d/%d", &va, &ta);
                    va--;
                    vb--;
                    vc--;
                    Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                    f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
                    nt++;
                    vb = vc;
                    while (fscanf(file, "%d/%d", &vc, &tc) > 0) {
                        vc--;
                        Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                        f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
                        nt++;
                        vb = vc;
                    }
                } else {
                    // v
                    sscanf(buf, "%d", &va);
                    fscanf(file, "%d", &vb);
                    fscanf(file, "%d", &vc);
                    va--;
                    vb--;
                    vc--;
                    Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                    f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
                    nt++;
                    vb = vc;
                    while (fscanf(file, "%d", &vc) > 0) {
                        vc--;
                        Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                        f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
                        nt++;
                        vb = vc;
                    }
                }
                break;

            default:
                // eat up rest of line
                fgets(buf, sizeof(buf), file);
                break;
        }
    }

//printf("dopo SecondPass nv=%d nt=%d\n",nv,nt);

    fclose(file);
    return true;
}
