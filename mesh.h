#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

#include "utils.h"
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
    Vertex(const Point& p_, FACE_INDEX if_) : p(p_), incidentFace(if_) {}
    const Point & getPoint() const { return p; }

    //Return incident face
    FACE_INDEX getIncidentFace() const { return incidentFace; }
    void setFi(FACE_INDEX fi) { incidentFace = fi; }

    //Operator -, return a Vector (used in laplacianCalc)
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

    VERTEX_INDEX vertex(unsigned int i) const {
        return _v[i];
    }

    //Return the local index of the vertex index vi
    int getIndexOf(VERTEX_INDEX vi) const {
        for (int i = 0; i < 3; ++i) {
            if (_v[i] == vi){
                return i;
            }
        }
        return -1;
    }

    //Return the front face of the vertex number i in the face
    FACE_INDEX getFrontFace(unsigned int i) const  {
        return _f[i];
    }

    //Return the front face of vertexIndex vi
    FACE_INDEX getFrontFaceOf(VERTEX_INDEX vi) const {
        return _f[getIndexOf(vi)];
    }

    //Set the opposite face of vertex (local index i)
    void setOppositeFace(FACE_INDEX fi, unsigned int i) {
        _f[i] = fi;
    }

    void setVertex(unsigned int i, VERTEX_INDEX vi) {
        _v[i] = vi;
    }

};

// ------------------------------------------------------------------------
//                        MESH
// ------------------------------------------------------------------------

class LaplacianCalc; //Forward declaration

class Mesh {
    friend class Vertices_iterator;
    friend class Faces_circulator;
    friend class Vertices_circulator;

private:
    //Vertices
    std::vector<Vertex> _vertices;
    //Faces
    std::vector<Face> _faces;

    //Iterators
    Faces_circulator fcirc;
    Vertices_circulator vcirc;

    //Infos for the drawing of currentFace, currentVertex..
    VERTEX_INDEX currentStartVertexIndex = -1;//The starting vertex (chose by the user)
    FACE_INDEX currentNeighborFace = -1;
    VERTEX_INDEX currentNeighborVertex = -1; //Neighboring vertices of start vertex

    //Used to show a point with a varying point size for selected vertices
    float pointSize = 1.0f;
    float decreaseFactorPointSize = 0.01;

    //The calculator of the laplacian vectors
    LaplacianCalc* lcalc; 

    //Compute local id of an opposite face for a face (v1, v2 are global ids representing the edge)
    int getIndexOfOpposite(FACE_INDEX f_id, VERTEX_INDEX v1, VERTEX_INDEX v2);

    //Draw the vertex 'vi' and use hsv color for the curvature if isCurrentFace is false
    //Else use white color if the vertex vi is in the isCurrentFace
    void glVertexIndexDraw(const VERTEX_INDEX vi, bool isCurrentFace);
    //Draw the face with curvature colors if isCurrentFace is false, else use white color
    void glFaceDraw(const Face & f, bool isCurrentFace);
    void drawSelectedPoints();
    void drawCurrentNeighborFace();


    double _maxX, _maxY, _maxZ;
    //Compute the max X, max Y of points of the mesh (used to know the frustrum and eye position)
    void computeMaxValues();

    //Load data from an .off file
    int load_off_file(const std::string& path_to_file);

    //Split face into 3 faces
    void split_face(const Point& newPoint);

public:

    Mesh();
    ~Mesh(); //Erase the laplacian calculator

    void createTetrahedron();
    void createPyramid();
    void createFromOFF(const std::string& filename);

    //The number of vertices
    unsigned int numberOfVertices() { return _vertices.size(); }
    unsigned int numberOfFaces() { return _faces.size(); }
    FACE_INDEX currFace() { return currentNeighborFace; }
    VERTEX_INDEX currStartVertex() { return currentStartVertexIndex; }
    VERTEX_INDEX currVertex() { return currentNeighborVertex; }
    double maxX() { return _maxX; }
    double maxY() { return _maxY; }
    double maxZ() { return _maxZ; }

    //Set the index of the vertex to iterate over its neighbors
    void setVertexStart(int vs);
    //Next neighboring face of currentStartVertex
    void nextFace(int);
    //Next neighboring vertex of currentStartVertex
    void nextVertex(int);
    //Put indexes of currentStartVertex, currentNeighbors to -1
    void resetVertexFaceIndex();

    //Draw the mesh with colors on faces
    void drawMesh();
    //Draw the mesh with wireframe
    void drawMeshWireFrame();

    //Iterators
    Vertices_iterator vertices_iterator_begin() { 
        return Vertices_iterator(this, 0); 
    }

    Vertices_iterator vertices_iterator_end() {
        return Vertices_iterator(this, _vertices.size());
    }

    Faces_circulator incident_faces_circulator(const VERTEX_INDEX vi) {
        return Faces_circulator(this, vi);
    }

    Vertices_circulator vertices_circulator_begin(const VERTEX_INDEX vi) {
        return Vertices_circulator(this, vi);
    }

};


#endif // MESH_H
