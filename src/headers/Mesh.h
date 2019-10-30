// classe Vertex: 
// i vertici della mesh

#include <vector>
#include "point3.h"
#include "Texture.h"

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
    std::vector <Vertex> v; // vettore di vertici
    std::vector <Face> f;   // vettore di facce
    std::vector <Edge> e;   // vettore di edge (per ora, non usato)

private:
    Texture *m_texture[3];
    void initModel();
public:

    // costruttore con caricamento
    Mesh(char *filename) {
        LoadFromObj(filename);
        ComputeNormalsPerFace();
        ComputeBoundingBox();
    }

    // metodi
    void setupTexture(GLenum, Point3, Point3);
    void render(); // manda a schermo la mesh
    void Texturize();
    bool LoadFromObj(char *filename); //  carica la mesh da un file obj
    void ComputeNormalsPerFace();
    void ComputeNormalsPerVertex();
    void ComputeBoundingBox();

    // centro del axis aligned bounding box
    Point3 Center() { return (bbmin + bbmax) / 2.0; };

    Point3 bbmin, bbmax; // bounding box
};
