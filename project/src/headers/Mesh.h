#ifndef PROGETTOGRAFICA_MESH_H
#define PROGETTOGRAFICA_MESH_H

#include <vector>
#include "point3.h"
#include "Texture.h"
#include "../../lib/glm.h"
#include "BoundingBox.h"
#include "Options.h"
#include "ShaderParams.h"

class Vertex {
public:
    Point3 p; // posizione

    // attributi per vertice
    // ...
};

class Edge {
public:
    Vertex *v[2]; // due puntatori a Vertice (i due estremi dell'edge)
    // attributi per edge:
};

class Face {
public:
    Vertex *v[3]; // tre puntatori a Vertice (i tre vertici del triangolo)

    // costruttore
    Face(Vertex *a, Vertex *b, Vertex *c) {
        v[0] = a;
        v[1] = b;
        v[2] = c;
    }

    // attributi per faccia
    Vector3 n; // normale (per faccia)

    // computa la normale della faccia
    void ComputeNormal() {
        n = -((v[1]->p - v[0]->p) % (v[2]->p - v[0]->p)).Normalize();
    }

    // attributi per wedge
};

class Mesh {
protected:
    std::vector <Vertex> v; // vettore di vertici
    std::vector <Face> f;   // vettore di facce
    std::vector <Edge> e;   // vettore di edge (per ora, non usato)

    char*      model_file = NULL;		/* name of the obect file */
    GLuint     model_list = 0;		/* display list for object */
    GLMmodel*  model;		        /* glm model data structure */
    GLfloat    scale;		        /* original scale factor */
    GLfloat    smoothing_angle = 90.0;	/* smoothing angle */
    GLuint     material_mode = 3;		/* 0=none, 1=color, 2=material, 3=texture */
    GLboolean  facet_normal = GL_FALSE;	/* draw with facet normal? */

private:
    void initModel();
protected:
    Options* options;
    ShaderParams* mShaderParams;
public:
    Mesh(char *filename, ShaderParams* shaderParams, Options* options);
    void render(); // manda a schermo la mesh
    void Texturize();
    bool LoadFromObj(char *filename); //  carica la mesh da un file obj
    void ComputeNormalsPerFace();
    void ComputeNormalsPerVertex();
    void ComputeBoundingBox(float, float, float, float, float);
    void displayBoundingBox();
    static void displayWorldBoundingBox(Point3*, Point3*);
    void lists();

    // centro del axis aligned bounding box
    Point3 Center() { return (bbmin + bbmax) / 2.0; };

    Point3 bbmin, bbmax; // bounding box (coordinate locali)
    BoundingBox *w_bounding_box; // bounding box (coordinate globali)
};

#endif