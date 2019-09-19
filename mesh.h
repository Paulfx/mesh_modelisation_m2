#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <stdlib.h>

#include "vec.h"
#include "mesh_iterators.h" 
#include "laplacianCalc.h"


// ------------------------------------------------------------------------
//                        VERTEX
// ------------------------------------------------------------------------

typedef int FACE_INDEX;
typedef int VERTEX_INDEX;

class Vertex {
    Point p;
    FACE_INDEX incidentFace;

public:
    Vertex(): p(), incidentFace(-1) {}
    Vertex(float x_, float y_, float z_, FACE_INDEX if_): p(x_,y_,z_), incidentFace(if_) {}
    const Point & getPoint() const { return p; }
    //Return incident face
    FACE_INDEX getIncidentFace() const { return incidentFace; }

    //Operator -, return a Vector
    Vector operator-(const Vertex& v);
};


// ------------------------------------------------------------------------
//                        FACE
// ------------------------------------------------------------------------

class Face
{
    VERTEX_INDEX _v[3];
    FACE_INDEX _f[3]; //Face in front of vertex
public:

    Face() {}
    Face(   VERTEX_INDEX v1_, VERTEX_INDEX v2_, VERTEX_INDEX v3_,
            FACE_INDEX f1_, FACE_INDEX f2_, FACE_INDEX f3_) {
        _v[0] = v1_;
        _v[1] = v2_;
        _v[2] = v3_;

        _f[0] = f1_;
        _f[1] = f2_;
        _f[2] = f3_;
    }

    // Accessors
    VERTEX_INDEX v1() const { return _v[0]; }
    VERTEX_INDEX v2() const { return _v[1]; }
    VERTEX_INDEX v3() const { return _v[2]; }

    //TODO rename
    VERTEX_INDEX vertex(unsigned int i) const {
        return _v[i];
    }

    FACE_INDEX face(unsigned int i) const {
        return _f[i];
    }

    //Peut être pas meilleure solution..
    int getIndexOf(VERTEX_INDEX vi) const {
        //Retourne l'indice dans _v du vertex vi
        for (int i = 0; i < 3; ++i)
            if (_v[i] == vi) return i;
        return -1;
    }

    FACE_INDEX getFrontFaceOf(VERTEX_INDEX vi) const {
        return _f[getIndexOf(vi)];
    }

};

// ------------------------------------------------------------------------
//                        MESH
// ------------------------------------------------------------------------

class LaplacianCalc; //WHY??

class Mesh {
    friend class Vertices_iterator;
    friend class Faces_circulator;
    friend class Vertices_circulator;

private:
    //Les sommets
    std::vector<Vertex> vertexTab;
    //Les faces
    std::vector<Face> faceTab;

    //Itérateurs pour le parcours
    Faces_circulator fcirc;
    Vertices_circulator vcirc;

    //Pour l'affichage des face actuelle
    VERTEX_INDEX currentStartVertexIndex = -1;//Le vertex dont on cherche les voisins
    FACE_INDEX currentNeighborFace = -1; //Face voisine sélectionnée
    VERTEX_INDEX currentNeighborVertex = -1; //Les vertex voisins de startVertex

    //Pour l'affichage d'un point sur un vertex avec une taille qui varie
    float pointSize = 1.0f;
    float decreaseFactorPointSize = 0.01;

    //Les laplaciens
    LaplacianCalc* lcalc; 
    
    void glFaceDraw(const Face & f);
    void drawSelectedPoints();
    void drawCurrentNeighborFace();

public:

    Mesh();

    void createTetrahedron();
    void createPyramid();

    unsigned int vertexNb() { return vertexTab.size(); }
    unsigned int faceNb() { return faceTab.size(); }
    FACE_INDEX currFace() { return currentNeighborFace; }
    VERTEX_INDEX currStartVertex() { return currentStartVertexIndex; }
    VERTEX_INDEX currVertex() { return currentNeighborVertex; }

    void setVertexStart(int vs);
    void nextFace(int);
    void nextVertex(int);
    void resetVertexFaceIndex();

    void drawMesh();
    void drawMeshWireFrame();

    void testLaplacian();

    //Iterators
    Vertices_iterator vertices_iterator_begin() { 
        return Vertices_iterator(this, 0); 
    }

    Vertices_iterator vertices_iterator_end() {
        return Vertices_iterator(this, vertexTab.size());
    }

    Faces_circulator incident_faces_circulator(const VERTEX_INDEX vi) {
        return Faces_circulator(this, vi);
    }

    Vertices_circulator vertices_circulator_begin(const VERTEX_INDEX vi) {
        return Vertices_circulator(this, vi);
    }

};


#endif // MESH_H
