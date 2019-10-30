// file mesh.cpp
//
// Implementazione dei metodi di Mesh

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

char*      model_file = NULL;		/* name of the obect file */
GLuint     model_list = 0;		/* display list for object */
GLMmodel*  model;		        /* glm model data structure */
GLfloat    scale;		        /* original scale factor */
GLfloat    smoothing_angle = 90.0;	/* smoothing angle */
GLuint     material_mode = 3;		/* 0=none, 1=color, 2=material, 3=texture */
GLboolean  facet_normal = GL_FALSE;	/* draw with facet normal? */

void Mesh::ComputeNormalsPerFace() {
    for (int i = 0; i < f.size(); i++) {
        f[i].ComputeNormal();
    }
}

void lists(){
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

    glActiveTexture(GL_TEXTURE2);
    m_texture[0] = new Texture(TEXTURE_DECK_WOODEN_FLOOR);
    m_texture[0]->loadTexture();
    glBindTexture(GL_TEXTURE_2D, m_texture[0]->getBind());
    glActiveTexture(GL_TEXTURE3);
    m_texture[1] = new Texture(TEXTURE_TOP_BODY_BLACK_METAL);
    m_texture[1]->loadTexture();
    glBindTexture(GL_TEXTURE_2D, m_texture[1]->getBind());
}

// funzione che prepara tutto per creare le coordinate texture (s,t) da (x,y,z)
// Mappo l'intervallo [ minY , maxY ] nell'intervallo delle T [0..1]
//     e l'intervallo [ minZ , maxZ ] nell'intervallo delle S [0..1]
void Mesh::setupTexture(GLenum n_texture, Point3 min, Point3 max){
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

void Mesh::render() {
    if (model_file == NULL){
        initModel();
    }
    setupTexture(GL_TEXTURE2, bbmin, bbmax);
    // mandiamo tutti i triangoli a schermo
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < f.size()/5; i++) {
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
    setupTexture(GL_TEXTURE3, bbmin, bbmax);
    glBegin(GL_TRIANGLES);
    //setupTexture(GL_TEXTURE3, bbmin, bbmax);
    for (int i = f.size()/5; i < (f.size()/5)*2; i++) {
        f[i].n.SendAsNormal(); // flat shading
        (f[i].v[0])->p.SendAsVertex();

        (f[i].v[1])->p.SendAsVertex();

        (f[i].v[2])->p.SendAsVertex();
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
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

void Mesh::ComputeBoundingBox() {
    if (!v.size()) return;
    bbmin = bbmax = v[0].p;
    for (int i = 0; i < v.size(); i++) {
        for (int k = 0; k < 3; k++) {
            if (bbmin.coord[k] > v[i].p.coord[k]) bbmin.coord[k] = v[i].p.coord[k];
            if (bbmax.coord[k] < v[i].p.coord[k]) bbmax.coord[k] = v[i].p.coord[k];
        }
    }
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
