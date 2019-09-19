#include "mesh.h"

// ------------------------------------------------------------------------
//                        VERTEX
// ------------------------------------------------------------------------

Vector Vertex::operator-(const Vertex &v) {
    return p - v.p;
}


// ------------------------------------------------------------------------
//                        MESH
// ------------------------------------------------------------------------

Mesh::Mesh()  {
    //createPyramid();
    createTetrahedron();

    //On créé le laplacien
    // lcalc = new LaplacianCalc(this);
    // lcalc->calculate();

    // printf("Courbure au vertex 0 : %f\n", lcalc->getCurvature(0));
    // printf("Courbure au vertex 1 : %f\n", lcalc->getCurvature(1));
    // printf("Courbure au vertex 2 : %f\n", lcalc->getCurvature(2));
    // printf("Courbure au vertex 3 : %f\n", lcalc->getCurvature(3));

    // Vector v = lcalc->getNormal(0);
    // printf("Normale vertex 0 : x=%f, y=%f, z=%f\n", v.x, v.y, v.z);

    // delete lcalc;
    //testLaplacian();
}

void Mesh::testLaplacian() {

    LaplacianCalc lcalc(this);

    lcalc.calculate();

    //Vertex v0 = vertexTab[0];
    //Vertex v1 = vertexTab[1];

    //printf("Cotan : %f\n", lcalc.cotan(v0,v1));
}

void Mesh::createTetrahedron() {

    vertexTab.push_back(Vertex(-0.5,-0.5,-0.5   , 3));
    vertexTab.push_back(Vertex(0.5,-0.5,-0.5    , 0));
    vertexTab.push_back(Vertex(0,0.5,-0.5       , 1));
    vertexTab.push_back(Vertex(0,-0.5,0.5       , 2));

    faceTab.push_back(Face(1,2,3, 1,2,3)); //BCD
    faceTab.push_back(Face(3,2,0, 3,2,0)); //DCA
    faceTab.push_back(Face(3,0,1, 3,0,1)); //DAB
    faceTab.push_back(Face(0,2,1, 0,2,1)); //ACB

}

void Mesh::createPyramid() {
    //TODO manual

    vertexTab.push_back(Vertex(-0.5,0,-0.5   , 3));
    vertexTab.push_back(Vertex(0.5,0,-0.5    , 0));
    vertexTab.push_back(Vertex(-0.5,0,0.5       , 1));
    vertexTab.push_back(Vertex(0.5,0,0.5       , 1));
    vertexTab.push_back(Vertex(0,1,0       , 2));

    

    faceTab.push_back(Face(0,2,1, 4,1,5)); //ACB
    faceTab.push_back(Face(1,2,3, 5,1,0)); //BCD
    faceTab.push_back(Face(0,4,1, 4,1,5)); //AEB
    faceTab.push_back(Face(1,4,3, 5,1,0)); //BED
    faceTab.push_back(Face(3,4,2, 0,1,1)); //DEC
    faceTab.push_back(Face(2,4,0, 1,1,4)); //CEA


}


void Mesh::setVertexStart(int vs) {
    if (vs < 0 || vs >= vertexTab.size()) return;

    currentStartVertexIndex = vs;

    //Circulator on faces
    fcirc = incident_faces_circulator(vs);
    //Circulator on neighbor vertices
    vcirc = vertices_circulator_begin(vs);

    currentNeighborFace = fcirc.currentFaceIndex;
    currentNeighborVertex = vcirc.currentVertexIndex;
}


void Mesh::nextFace(int s) {
    if (currentStartVertexIndex < 0 || 
        currentStartVertexIndex >= vertexTab.size()) return;
    printf("Next face : %d\n", s);
    if (s < 0) fcirc--;
    else fcirc++;
    currentNeighborFace = fcirc.currentFaceIndex;
}

void Mesh::nextVertex(int s) {
    if (currentStartVertexIndex < 0 || 
        currentStartVertexIndex >= vertexTab.size()) return;
    if (s < 0) vcirc--;
    else vcirc++;
    currentNeighborVertex = vcirc.currentVertexIndex;
}

void Mesh::resetVertexFaceIndex() {
    currentStartVertexIndex = -1;
    currentNeighborVertex = -1;
    currentNeighborFace = -1;
}

// The following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
// Draw a vertex
void glVertexDraw(const Point & p) {
    glVertex3f(p.x, p.y, p.z);
}

void glVertexDraw(const Vertex & v) {
    glVertexDraw(v.getPoint());
}

void Mesh::glFaceDraw(const Face & f) {
    glBegin(GL_TRIANGLES);
    glVertexDraw(vertexTab[f.v1()]);
    glVertexDraw(vertexTab[f.v2()]);
    glVertexDraw(vertexTab[f.v3()]);
    glEnd();
}


void Mesh::drawMesh() {
    for(int i = 0; i < faceTab.size(); i++) {
        if (i == 0) glColor3d(1,0,0);
        else if (i == 1) glColor3d(0,1,0);
        else if (i == 2) glColor3d(0,0,1);
        else glColor3d(1,1,0);

        //Couleurs en fonction de la courbure

        if (i == currentNeighborFace) glColor3d(1,1,1);

        glFaceDraw(faceTab[i]);
    }

    drawSelectedPoints();
    drawCurrentNeighborFace();
}

void Mesh::drawCurrentNeighborFace() {
    if (currentNeighborFace == -1 ||
        currentNeighborFace >= faceTab.size()) return;
    glColor3d(1,1,1);
    glFaceDraw(faceTab[currentNeighborFace]);
}

void Mesh::drawSelectedPoints() {
    if (currentStartVertexIndex == -1 ||
        currentStartVertexIndex >= vertexTab.size()) return;
    //currentNeighborIndex != -1

    //Adapt point size with time
    pointSize = pointSize + decreaseFactorPointSize;
    //Max point size => 7
    if (pointSize > 7) decreaseFactorPointSize = -0.1;
    else if (pointSize <= 1) {
        pointSize = 1;
        decreaseFactorPointSize = 0.1;
    }
    glPointSize(pointSize);
    glBegin(GL_POINTS);

    //Start vertex index
    glColor3d(1,1,1); //White
    glVertexDraw(vertexTab[currentStartVertexIndex]);
    
    //Neighbor vertex index
    glColor3d(1,0,1);
    glVertexDraw(vertexTab[currentNeighborVertex]);

    glEnd();
}

//Example with a wireframe tedraedra
void Mesh::drawMeshWireFrame() {
    Face f;
    for(int i = 0; i < faceTab.size(); i++) {
        f = faceTab[i];
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[f.v1()]);
            glVertexDraw(vertexTab[f.v2()]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[f.v2()]);
            glVertexDraw(vertexTab[f.v3()]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[f.v3()]);
            glVertexDraw(vertexTab[f.v1()]);
        glEnd();
    }

    //TODO avec itérateurs
    drawSelectedPoints();
    drawCurrentNeighborFace();

}


