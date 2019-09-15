#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "mesh_iterators.h" 


// ------------------------------------------------------------------------
//                        POINT
// ------------------------------------------------------------------------

class Point {
    double _x;
    double _y;
    double _z;

public:
    Point():_x(),_y(),_z() {}
    Point(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_) {}
    // get
    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }
};

typedef int FACE_INDEX;
typedef int VERTEX_INDEX;

// ------------------------------------------------------------------------
//                        VERTEX
// ------------------------------------------------------------------------

class Vertex {
    Point p;
    FACE_INDEX faceIndex; //In front

public:
    Vertex(): p(), faceIndex(-1) {}
    Vertex(float x_, float y_, float z_, FACE_INDEX fi_): p(x_,y_,z_), faceIndex(fi_) {}
    const Point & getPoint() const { return p; }
    //Return incident face
    FACE_INDEX fi() const { return faceIndex; }
    void setFi(FACE_INDEX fi) { faceIndex = fi; }
};


// ------------------------------------------------------------------------
//                        FACE
// ------------------------------------------------------------------------

class Face
{
    VERTEX_INDEX _v[3];
    FACE_INDEX _f1, _f2, _f3; //INUTILE ?? Redondance
    //Peut etre on s'est trompé..
    //Il serait plus logique de stocker l'indice dans _v de chaque vertex

public:

    Face(): _f1(), _f2(), _f3() {}
    Face(   VERTEX_INDEX v1_, VERTEX_INDEX v2_, VERTEX_INDEX v3_,
            FACE_INDEX f1_, FACE_INDEX f2_, FACE_INDEX f3_) : _f1(f1_), _f2(f2_), _f3(f3_) {
        _v[0] = v1_;
        _v[1] = v2_;
        _v[2] = v3_;
    }

    // Accessors
    VERTEX_INDEX v1() const { return _v[0]; }
    VERTEX_INDEX v2() const { return _v[1]; }
    VERTEX_INDEX v3() const { return _v[2]; }

    //TODO rename
    VERTEX_INDEX v(unsigned int i) const {
        return _v[i];
    }

    //Peut être pas meilleure solution..
    int getIndexOf(VERTEX_INDEX vi) const {
        //Retourne l'indice dans _v du vertex vi
        for (int i = 0; i < 3; ++i)
            if (_v[i] == vi) return i;
        return -1;
    }
};

// ------------------------------------------------------------------------
//                        MESH
// ------------------------------------------------------------------------

class Mesh {
    friend class Vertices_iterator;
    friend class Faces_circulator;
    friend class Vertices_circulator;

private:
    //Les sommets
    QVector<Vertex> vertexTab;
    //Les faces
    QVector<Face> faceTab;

    //Itérateurs pour le parcours
    Faces_circulator fcirc;
    Vertices_circulator vcirc;

    //Pour l'affichage des face actuelle
    VERTEX_INDEX currentStartVertexIndex = -1;//Le vertex dont on cherche les voisins
    FACE_INDEX currentNeighborFace = -1; //Face voisine sélectionnée
    VERTEX_INDEX currentNeighborVertex = -1; //Les vertex voisins de startVertex

    //Pour l'affichage d'un point sur un vertex
    float pointSize = 1.0f;
    float decreaseFactorPointSize = 0.01;
    
    void glFaceDraw(const Face & f);
    void drawSelectedPoints();
    void drawCurrentNeighborFace();

    unsigned int getOppositeVextex(FACE_INDEX f_id, VERTEX_INDEX v1, VERTEX_INDEX v2);

public:

    Mesh();

    void createTetrahedron();
    void createPyramid();
    int load_off_file(std::string path_to_file);

    unsigned int vertexNb() { return vertexTab.size(); }
    unsigned int faceNb() { return faceTab.size(); }
    FACE_INDEX currFace() { return currentNeighborFace; }
    VERTEX_INDEX currStartVertex() { return currentStartVertexIndex; }
    VERTEX_INDEX currVertex() { return currentNeighborVertex; }

    void setVertexStart(int vs);
    void nextFace();
    void nextVertex();
    void resetVertexFaceIndex();

    void drawMesh();
    void drawMeshWireFrame();


    //Iterators
    Vertices_iterator vertices_iterator_begin() { 
        return Vertices_iterator(this, 0); 
    }

    Vertices_iterator vertices_iterator_end() {
        return Vertices_iterator(this, vertexTab.size());
    }

    //TODO use index of vertex??
    Faces_circulator incident_faces_circulator(const Vertex& v) {
        return Faces_circulator(v, this);
    }

    Vertices_circulator Vertices_circulator_begin(const VERTEX_INDEX vi) {
        return Vertices_circulator(this, vi);
    }

};


#endif // MESH_H
