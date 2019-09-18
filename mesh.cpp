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

Mesh::Mesh() {
    //createTetrahedron();
    std::cout << "open = " << load_off_file("./Documents/cours/m2/geoAlgo/mesh_modelisation/queen.off") << std:: endl;
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
}

/**
 * Load data from .off file
 */
int Mesh::load_off_file(std::string path_to_file) {
    std::string line;
    std::ifstream myfile (path_to_file);
    std::string spaceDelimiter = " ";
    std::string lineDelimiter = "\n";
    int nbVertices;
    int nbFaces;
    if (myfile.is_open()) {

        /** read first line of file **/
        getline(myfile, line);
        nbVertices = atoi(line.substr(0, line.find(spaceDelimiter)).c_str());
        nbFaces = atoi(line.substr(line.find(spaceDelimiter), line.find(lineDelimiter)).c_str());

        /** read point positions to create vertices **/
        for (int i = 0; i < nbVertices; i++) {
            getline(myfile, line);
            double x = atof(line.substr(0, line.find(spaceDelimiter)).c_str());
            double y = atof(line.substr(line.find(spaceDelimiter), line.rfind(spaceDelimiter)).c_str());
            double z = atof(line.substr(line.rfind(spaceDelimiter), line.find(lineDelimiter)).c_str());
            vertexTab.push_front(Vertex(x, y, z, -1));//-1 -> no info yet
        }

        /** faces definition + adjacency map construction **/

        std::map<std::pair<VERTEX_INDEX, VERTEX_INDEX>, FACE_INDEX> infoEdges; //map between edge and id of face

        for (int idCurrentFace = 0; idCurrentFace < nbFaces; idCurrentFace++) {
            getline(myfile, line);

            int vertexId1 = atoi(line.substr(line.find(spaceDelimiter, 1), line.find(spaceDelimiter, 2)).c_str());
            int vertexId2 = atoi(line.substr(line.find(spaceDelimiter, 2), line.rfind(spaceDelimiter)).c_str());
            int vertexId3 = atoi(line.substr(line.rfind(spaceDelimiter), line.find(lineDelimiter)).c_str());

            faceTab.push_back(Face(vertexId1, vertexId2, vertexId3, -1, -1, -1));//-1 useless values, need to change constructor

            for (int j = 0; j < 3; j++) { //3 edges by face
                VERTEX_INDEX v1;
                VERTEX_INDEX v2;
                if (j == 0) {
                    v1 = faceTab[idCurrentFace].v1(); //first vertex of the edge
                    v2 = faceTab[idCurrentFace].v2(); //second

                } else if (j == 1) {
                    v1 = faceTab[idCurrentFace].v1();
                    v2 = faceTab[idCurrentFace].v2();
                } else {
                    v1 = faceTab[idCurrentFace].v1();
                    v2 = faceTab[idCurrentFace].v2();
                }

                std::pair<VERTEX_INDEX, VERTEX_INDEX> edge (v1, v2); //edge creation

                //map construction, if no entry for the edge (key) in the map, we add it with the FACE_INDEX (value)
                if (infoEdges.find(edge) != infoEdges.end()) { //find(x) return .end if x doesn't exist in map - NEED TO IMPROVE IN ORDER TO ACCEPT <v1,v2> == <v2,v1>
                    /** Entry found for this edge in map, so... **/

                    //...set Fi for current face and also ...
                    unsigned int oppositeToEdge = getOppositeVextex(idCurrentFace, v1, v2);
                    vertexTab[faceTab[idCurrentFace].v(oppositeToEdge)].setFi(infoEdges[edge]);

                    //...set Fi for opposite face
                    oppositeToEdge = getOppositeVextex(infoEdges[edge], v1, v2);
                    vertexTab[faceTab[infoEdges[edge]].v(oppositeToEdge)].setFi(idCurrentFace);

                    //todo: delete edge entry in map

                } else { //edge doesn't exist yet in map
                    infoEdges[edge] = idCurrentFace; //save id of current face
                }
            }
        }
        myfile.close();
        return 0;
    }
    else std::cout << "Unable to open file";
    return -1;
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

// return local id of third vertex in face (v1, v2 are global ids)
unsigned int Mesh::getOppositeVextex(FACE_INDEX f_id, VERTEX_INDEX v1, VERTEX_INDEX v2) {
    int id1 = faceTab[f_id].getIndexOf(v1);
    int id2 = faceTab[f_id].getIndexOf(v2);
    return 3 - (id1 + id2);
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

void Mesh::testLaplacian() {

    LaplacianCalc lcalc(this);

    //Vertex v0 = vertexTab[0];
    //Vertex v1 = vertexTab[1];

    //printf("Cotan : %f\n", lcalc.cotan(v0,v1));
}


