#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>

#include "vec.h"
#include "mesh_iterators.h" 
#include "laplacianCalc.h"
#include "utils.h"

#include <cassert>

// ------------------------------------------------------------------------
//                        VERTEX
// ------------------------------------------------------------------------

// typedef int FACE_INDEX;
// typedef int VERTEX_INDEX;

using FACE_INDEX = int;
using VERTEX_INDEX = int;

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

    Face(   VERTEX_INDEX v1_, VERTEX_INDEX v2_, VERTEX_INDEX v3_) {
        _v[0] = v1_;
        _v[1] = v2_;
        _v[2] = v3_;

        _f[0] = -1;
        _f[1] = -1;
        _f[2] = -1;
    }

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
    int getLocalIndexOf(VERTEX_INDEX vi) const {
        for (int i = 0; i < 3; ++i) {
            if (_v[i] == vi){
                return i;
            }
        }
        return -1;
    }

    //Compute local id of an opposite face for a face (v1, v2 are global ids representing the edge)
    int getLocalIndexOfOppositeFromVertexIndex(VERTEX_INDEX v1, VERTEX_INDEX v2) const {
        //Precondition : v1 and v2 are in the face
        int id1 = getLocalIndexOf(v1);
        int id2 = getLocalIndexOf(v2);
        //return (id1 + id2) - 1;
        return getLocalIndexOfOppositeFromLocalIndex(id1,id2);
    }

    int getLocalIndexOfOppositeFromLocalIndex(int id1, int id2) const {
        return (3 - (id1 + id2));
    }

    //Return the front face of the vertex number i in the face
    FACE_INDEX getFrontFace(unsigned int i) const  {
        return _f[i];
    }

    //Return the front face of vertexIndex vi
    FACE_INDEX getFrontFaceOf(VERTEX_INDEX vi) const {
        return _f[getLocalIndexOf(vi)];
    }

    bool haveInfiniteFaceInFront() const {
        for (int i = 0; i < 3; ++i) {
            if (_f[i] == -1){
                return true;
            }
        }
        return false;
    }

    int getLocalIdVertexOppositeTofinite() const {
        for (int i = 0; i < 3; ++i) {
            if (_f[i] == -1){
                return i;
            }
        }
        return -1;
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
    //The vertices
    std::vector<Vertex> _vertices;
    //The faces
    std::vector<Face> _faces;

    //The calculator of the laplacian vectors
    LaplacianCalc* lcalc; 

    //Iterators to track a current vertex and face
    Faces_circulator fcirc;
    Vertices_circulator vcirc;

    VERTEX_INDEX currentStartVertexIndex = -1;  //The starting vertex (chose by the user)
    FACE_INDEX currentNeighborFace = -1;        //Neighboring faces of start vertex
    VERTEX_INDEX currentNeighborVertex = -1;    //Neighboring vertices of start vertex

    //Used to show a point with a varying point size for selected vertices
    float pointSize = 1.0f;
    float decreaseFactorPointSize = 0.01;


//Drawing functions

    //Draw the vertex 'vi' and use hsv color for the curvature if isCurrentFace is false
    //Else use white color if the vertex vi is in the isCurrentFace
    void glVertexIndexDraw(const VERTEX_INDEX vi, bool isCurrentFace);
    //Draw the face with curvature colors if isCurrentFace is false, else use white color
    void glFaceDraw(const Face & f, bool isCurrentFace);
    //Draw startVertex and currentNeigborVertex as point with breathing size
    void drawSelectedPoints();
    //Draw the current neigbor face in white
    void drawCurrentNeighborFace();

    // //If the mesh is too large, try to adapt frustrum...
    // double _maxX, _maxY, _maxZ;
    // //Compute the max X, max Y of points of the mesh (used to know the frustrum and eye position)
    // void computeMaxValues();

    //Load data from an .off file
    int load_off_file(const std::string& path_to_file);

    //Add the point newPoint contained in the face fi by splitting fi into 3 new faces
    //Pre condition : the face fi contains the point newPoint
    void split_face(const Point& newPoint, FACE_INDEX fi);
    //Add a point outside the convex hull and create all the necessary faces
    //Precondition: idsExtHull is not empty, and contains all the faces which
    //contain a point linked to the implicit infinite point
    void addPointAndFlipToInfinite(std::vector<FACE_INDEX> idsExtHull,Point p);

    //Flip an edge shared between two triangles
    void flip_edge(const FACE_INDEX f1, const FACE_INDEX f2, const VERTEX_INDEX v1, const VERTEX_INDEX v2);
    
    //Split edge in front of localIndexF1
    //void Mesh::flip_edge(const FACE_INDEX f1, int localIndexF1) {

public:

//Construction

    Mesh();
    ~Mesh(); //Erase the laplacian calculator
    void createTetrahedron();
    void create2DSquare();
    void createPyramid();
    void createFromOFF(const std::string& filename);

    void testPredicates();

//Accessors/mutators used by the window to interact with the mesh

    unsigned int numberOfVertices() { return _vertices.size(); }
    unsigned int numberOfFaces() { return _faces.size(); }
    FACE_INDEX currFace() { return currentNeighborFace; }
    VERTEX_INDEX currStartVertex() { return currentStartVertexIndex; }
    VERTEX_INDEX currVertex() { return currentNeighborVertex; }

    //Set the index of the vertex to iterate over its neighbors
    void setVertexStart(int vs);
    //Next neighboring face of currentStartVertex
    void nextFace(int);
    //Next neighboring vertex of currentStartVertex
    void nextVertex(int);
    //Put indexes of currentStartVertex, currentNeighbors to -1 (disable display)
    void resetVertexFaceIndex();

    void splitFaceMiddle(int faceIndex);

//Draw functions

    //Draw the mesh with colors on faces
    void drawMesh();
    //Draw the mesh with wireframe
    void drawMeshWireFrame();
    //Draw the voronoi cells
    void drawVoronoiDiagram();

//Insertion

    //Naive insertion of a point in the convexHull (only a split face), or outside the convex hull
    //(use of addPointAndFlipToInfinite)
    void naiveInsertion(const Point p);
    void delaunayInsertion(const Point p);

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
