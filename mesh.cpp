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
    //createTetrahedron();
    //std::string filename = "./Documents/cours/m2/geoAlgo/mesh_modelisation/queen.off";

    std::string filename = "./M2/maillage/Mesh_Computational_Geometry/queen.off";
    std::cout << "open = " << load_off_file(filename) << std:: endl;

    createTetrahedron();
    //glEnable(GL_LIGHTING);

    //Create the laplacian calculator
    lcalc = new LaplacianCalc();
    lcalc->calculate(this);

    float start = 0.f;
    float stop = 360.f;

    printf("Courbure au vertex 0 : %f\n", lcalc->getCurvatureMapped(0, start, stop));
    printf("Courbure au vertex 1 : %f\n", lcalc->getCurvatureMapped(1, start, stop));
    printf("Courbure au vertex 2 : %f\n", lcalc->getCurvatureMapped(2, start, stop));
    printf("Courbure au vertex 3 : %f\n", lcalc->getCurvatureMapped(3, start, stop));

    Vector v = lcalc->getNormal(0);
    printf("Normale vertex 0 : x=%f, y=%f, z=%f\n", v.x, v.y, v.z);
}

Mesh::~Mesh() { delete lcalc; }

void Mesh::createTetrahedron() {
    _vertices.clear();
    _faces.clear();

    _vertices.push_back(Vertex(-0.5,-0.5,-0.5   , 3));
    _vertices.push_back(Vertex(0.5,-0.5,-0.5    , 0));
    _vertices.push_back(Vertex(0,0.5,-0.5       , 1));
    _vertices.push_back(Vertex(0,-0.5,0.5       , 2));

    _faces.push_back(Face(1,2,3, 1,2,3)); //BCD
    _faces.push_back(Face(3,2,0, 3,2,0)); //DCA
    _faces.push_back(Face(3,0,1, 3,0,1)); //DAB
    _faces.push_back(Face(0,2,1, 0,2,1)); //ACB

}

void Mesh::createPyramid() {
    //TODO manual
    _vertices.clear();
    _faces.clear();

    _vertices.push_back(Vertex(-0.5,0,-0.5   , 3));
    _vertices.push_back(Vertex(0.5,0,-0.5    , 0));
    _vertices.push_back(Vertex(-0.5,0,0.5       , 1));
    _vertices.push_back(Vertex(0.5,0,0.5       , 1));
    _vertices.push_back(Vertex(0,1,0       , 2));



    _faces.push_back(Face(0,2,1, 4,1,5)); //ACB
    _faces.push_back(Face(1,2,3, 5,1,0)); //BCD
    _faces.push_back(Face(0,4,1, 4,1,5)); //AEB
    _faces.push_back(Face(1,4,3, 5,1,0)); //BED
    _faces.push_back(Face(3,4,2, 0,1,1)); //DEC
    _faces.push_back(Face(2,4,0, 1,1,4)); //CEA


}

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
            _vertices.push_back(Vertex(x, y, z, -1));//-1 -> no info yet
        }

        /** faces definition + adjacency map construction **/

        std::map<std::pair<VERTEX_INDEX, VERTEX_INDEX>, FACE_INDEX> infoEdges; //map between edge and id of face

        for (int idCurrentFace = 0; idCurrentFace < nbFaces; idCurrentFace++) {
            getline(myfile, line);

            int vertexId1 = atoi(line.substr(line.find(spaceDelimiter, 1), line.find(spaceDelimiter, 2)).c_str());
            int vertexId2 = atoi(line.substr(line.find(spaceDelimiter, 2), line.rfind(spaceDelimiter)).c_str());
            int vertexId3 = atoi(line.substr(line.rfind(spaceDelimiter), line.find(lineDelimiter)).c_str());

            _faces.push_back(Face(vertexId1, vertexId2, vertexId3, -1, -1, -1));// Face creation (-1 useless values, need to change constructor)

            //add incident faces into vertices (we store the last id of face who holding the vertex)
            _vertices[vertexId1].setFi(idCurrentFace);
            _vertices[vertexId2].setFi(idCurrentFace);
            _vertices[vertexId3].setFi(idCurrentFace);

            for (int j = 0; j < 3; j++) { //3 edges by face
                VERTEX_INDEX v1;
                VERTEX_INDEX v2;

                v1 = _faces[idCurrentFace].vertex(std::max(0,j - 1));//first vertex of the edge
                v2 = _faces[idCurrentFace].vertex(std::min(j + 1, 2));//second

                std::pair<VERTEX_INDEX, VERTEX_INDEX> edge (v1, v2); //edge creation

                std::pair<VERTEX_INDEX, VERTEX_INDEX> edge2 (v2, v1); // IN ORDER TO ACCEPT <v1,v2> == <v2,v1>

                //map construction: if an entry elready exist for the edge (key) in the map, we create an association, else we add the edge with the FACE_INDEX (value) to the map
                if (infoEdges.find(edge) != infoEdges.end()) {//find(x) return .end if x doesn't exist in map
                    // Entry found for this edge (same edge order <v1,v2>), so...

                    //...set opposite for current face and also ...
                    int index = getIndexOfOpposite(idCurrentFace, v1, v2);
                    _faces[idCurrentFace].setOppositeFace(infoEdges[edge], index);

                    //...set opposite for opposite face (=current)
                    index = getIndexOfOpposite(infoEdges[edge], v1, v2);
                    _faces[infoEdges[edge]].setOppositeFace(idCurrentFace, index);

                } else if (infoEdges.find(edge2) != infoEdges.end()) {

                    //We do the same but according to diffrent edge direction (<v2,v1>) for the two faces
                    int index = getIndexOfOpposite(idCurrentFace, v1, v2);
                    _faces[idCurrentFace].setOppositeFace(infoEdges[edge2], index);

                    index = getIndexOfOpposite(infoEdges[edge2], v1, v2);// edge is not in the same direction in this face (using edge2)
                    _faces[infoEdges[edge2]].setOppositeFace(idCurrentFace, index);

                }

                //todo: delete edge entry in map

                else { //edge doesn't exist yet in map
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
    if (vs < 0 || vs >= (int) _vertices.size()) return;

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
        currentStartVertexIndex >= (int)_vertices.size()) return;
    printf("Next face : %d\n", s);
    if (s < 0) fcirc--;
    else fcirc++;
    currentNeighborFace = fcirc.currentFaceIndex;
}

void Mesh::nextVertex(int s) {
    if (currentStartVertexIndex < 0 ||
        currentStartVertexIndex >= (int)_vertices.size()) return;
    if (s < 0) vcirc--;
    else vcirc++;
    currentNeighborVertex = vcirc.currentVertexIndex;
}

void Mesh::resetVertexFaceIndex() {
    currentStartVertexIndex = -1;
    currentNeighborVertex = -1;
    currentNeighborFace = -1;
}

int Mesh::getIndexOfOpposite(FACE_INDEX f_id, VERTEX_INDEX v1, VERTEX_INDEX v2) {
    int id1 = _faces[f_id].getIndexOf(v1);
    int id2 = _faces[f_id].getIndexOf(v2);
    //return (id1 + id2) - 1;
    return (3 - (id1 + id2));
}


// The following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
// Draw a vertex
void glVertexDraw(const Point & p) {
    glVertex3f(p.x, p.y, p.z);
}

void glVertexDraw(const Vertex & v) {
    glVertexDraw(v.getPoint());
}


//Code from www.programmingalgorithms.com/algorithm/hsv-to-rgb?lang=C
void HSVToRGB(double hue, double s, double v, double& r, double& g, double& b) {
    r=0, g=0, b=0;

    if (s == 0)
    {
        r = v;
        g = v;
        b = v;
    }
    else
    {
        int i;
        double f, p, q, t;

        if (hue == 360)
            hue = 0;
        else
            hue = hue / 60;

        i = (int)trunc(hue);
        f = hue - i;

        p = v * (1.0 - s);
        q = v * (1.0 - (s * f));
        t = v * (1.0 - (s * (1.0 - f)));

        switch (i)
        {
        case 0:
            r = v;
            g = t;
            b = p;
            break;

        case 1:
            r = q;
            g = v;
            b = p;
            break;

        case 2:
            r = p;
            g = v;
            b = t;
            break;

        case 3:
            r = p;
            g = q;
            b = v;
            break;

        case 4:
            r = t;
            g = p;
            b = v;
            break;

        default:
            r = v;
            g = p;
            b = q;
            break;
        }

    }

    // struct RGB rgb;
    // rgb.R = r * 255;
    // rgb.G = g * 255;
    // rgb.B = b * 255;
}

void Mesh::glVertexIndexDraw(const VERTEX_INDEX vi, bool isCurrentFace) {
    //The normal
    const Vector& normal = lcalc->getNormal(vi);
    //The curvature
    if (isCurrentFace) //Use color white
        glColor3f(1,1,1);
    else {
        float curvature = lcalc->getCurvatureMapped(vi,0.f,360.f);
        double r,g,b;
        //Convert curvature to rgb color
        HSVToRGB(curvature, 1, 1, r, g, b);
        glColor3d(r,g,b);
    }
    glNormal3f(normal.x, normal.y, normal.z);
    const Point& p = _vertices[vi].getPoint();
    glVertexDraw(p);

}

void Mesh::glFaceDraw(const Face & f, bool isCurrentFace) {
    glBegin(GL_TRIANGLES);


    //glColor3d()
    // glVertexDraw(_vertices[f.v1()]);
    // glVertexDraw(_vertices[f.v2()]);
    // glVertexDraw(_vertices[f.v3()]);

    glVertexIndexDraw(f.v1(), isCurrentFace);
    glVertexIndexDraw(f.v2(), isCurrentFace);
    glVertexIndexDraw(f.v3(), isCurrentFace);


    glEnd();
}


void Mesh::drawMesh() {
    for(unsigned i = 0; i < _faces.size(); i++) {
        glFaceDraw(_faces[i], i == currentNeighborFace);
    }

    drawSelectedPoints();
    //drawCurrentNeighborFace();
}

void Mesh::drawCurrentNeighborFace() {
    if (currentNeighborFace == -1 ||
        currentNeighborFace >= (int)_faces.size()) return;
    glColor3d(1,1,1);
    glFaceDraw(_faces[currentNeighborFace], true);
}

void Mesh::drawSelectedPoints() {
    if (currentStartVertexIndex == -1 ||
        currentStartVertexIndex >= (int)_vertices.size()) return;
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
    glVertexDraw(_vertices[currentStartVertexIndex]);

    //Neighbor vertex index
    glColor3d(1,0,1);
    glVertexDraw(_vertices[currentNeighborVertex]);

    glEnd();
}

void Mesh::drawMeshWireFrame() {
    Face f;
    for(unsigned i = 0; i < _faces.size(); i++) {
        f = _faces[i];
        glBegin(GL_LINE_STRIP);
            glVertexDraw(_vertices[f.v1()]);
            glVertexDraw(_vertices[f.v2()]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(_vertices[f.v2()]);
            glVertexDraw(_vertices[f.v3()]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(_vertices[f.v3()]);
            glVertexDraw(_vertices[f.v1()]);
        glEnd();
    }

    //TODO with iterators
    drawSelectedPoints();
    drawCurrentNeighborFace();
}


