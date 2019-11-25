#include "mesh.h"

//using VERTEX_INDEX = int;

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
    //Create the laplacian calculator
    lcalc = new LaplacianCalc();

    //createPyramid();
    //createTetrahedron();


    create2DSquare();

    printf("Before flip : \n");
    printMesh();

    //Point np(0,0,-0.5);
    //split_face(np, 3);

    //flip_edge(0, 1, 0, 2);
    flip_edge(0,1);

    printf("After flip : \n");
    printMesh();
}

Mesh::~Mesh() { delete lcalc; }

void Mesh::testPredicates() {
    Point p1 = Point(0.5, 1, 0);
    Point p2 = Point(0, 0, 0);
    Point p3 = Point(1, 0, 0);
    Point p4 = Point(0.5, 0, 0); // align to b & c
    Point p5 = Point(0.5,0.5,0);
    Point p6 = Point(-1,0.5,0);

   printf("test orientation (expected 1): %d \n", pred_orientation(p1,p2,p3));
   printf("test orientation (expected -1): %d \n", pred_orientation(p2,p1,p3));
   printf("test orientation (expected 0): %d \n", pred_orientation(p2,p4,p3));

   printf("test in tirangle (expected 0): %d \n", pred_inTriangle(p1,p2,p3,p4));
   printf("test in tirangle (expected 1): %d \n", pred_inTriangle(p1,p2,p3,p5));
   printf("test in tirangle (expected -1): %d \n", pred_inTriangle(p1,p2,p3,p6));

   printf("test in circle (expected 1): %d \n", pred_inCercle(p1,p2,p3,p5));
   printf("test in circle (expected 1): %d \n", pred_inCercle(p1,p2,p3,p4));
   printf("test in circle (expected -1): %d \n", pred_inCercle(p1,p2,p3,p6));


    Point c = computeCenterOfCircumscribedCercle(p1,p2,p3);
    printf("center approx = (0.5,0.5,0): %f, %f, %f \n", c.x, c.y, c.z );


}

void Mesh::createTetrahedron() {
    resetShape();

    _vertices.push_back(Vertex(-0.5,-0.5,-0.5   , 3));
    _vertices.push_back(Vertex(0.5,-0.5,-0.5    , 0));
    _vertices.push_back(Vertex(0,0.5,-0.5       , 1));
    _vertices.push_back(Vertex(0,-0.5,0.5       , 2));

    _faces.push_back(Face(1,2,3, 1,2,3)); //BCD
    _faces.push_back(Face(3,2,0, 3,2,0)); //DCA
    _faces.push_back(Face(3,0,1, 3,0,1)); //DAB
    _faces.push_back(Face(0,2,1, 0,2,1)); //ACB

    //Calculate laplacian
    lcalc->calculate(this);
    //Compute max X and Y of the mesh
    //computeMaxValues();
}

void Mesh::create2DSquare() {

    resetShape();

    _vertices.push_back(Vertex(-0.5,-0.5,0     , 0));
    _vertices.push_back(Vertex(0.5,-0.5,0     , 0));
    _vertices.push_back(Vertex(0.5,0.5,0     , 1));
    _vertices.push_back(Vertex(-0.5,0.5,0     , 1));

    _faces.push_back(Face(0,1,2, -1,1,-1));
    _faces.push_back(Face(0,2,3, -1,-1,0));

    //Calculate laplacian
    lcalc->calculate(this);

}

void Mesh::createPyramid() {
    resetShape();

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

    //Calculate laplacian
    lcalc->calculate(this);
    //computeMaxValues();
}

void Mesh::createFromOFF(const std::string &filename) {
    resetShape();

    load_off_file(filename);

    //Calculate laplacian
    lcalc->calculate(this);
    //computeMaxValues();
}

int Mesh::load_off_file(const std::string& path_to_file) {
    std::string line;
    std::ifstream myfile (path_to_file);
    std::string spaceDelimiter = " ";
    std::string lineDelimiter = "\n";
    int nbVertices;
    int nbFaces;
    if (myfile.is_open()) {

        /** read first line of file **/
        getline(myfile, line);

        //If file begin by a letter, try to discard the line..
        if (!isdigit(line[0])) getline(myfile, line);

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
                    int index = _faces[idCurrentFace].getLocalIndexOfOppositeFromVertexIndex(v1, v2);
                    _faces[idCurrentFace].setOppositeFace(infoEdges[edge], index);

                    //...set opposite for opposite face (=current)
                    index = _faces[infoEdges[edge]].getLocalIndexOfOppositeFromVertexIndex(v1, v2);
                    _faces[infoEdges[edge]].setOppositeFace(idCurrentFace, index);

                } else if (infoEdges.find(edge2) != infoEdges.end()) {

                    //We do the same but according to diffrent edge direction (<v2,v1>) for the two faces
                    int index = _faces[idCurrentFace].getLocalIndexOfOppositeFromVertexIndex(v1, v2);
                    _faces[idCurrentFace].setOppositeFace(infoEdges[edge2], index);

                    index = _faces[infoEdges[edge2]].getLocalIndexOfOppositeFromVertexIndex(v1, v2);// edge is not in the same direction in this face (using edge2)
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
    else std::cout << "Unable to open file" << std::endl;
    return -1;
}

void Mesh::resetShape() {
    _vertices.clear();
    _faces.clear();
    resetVertexFaceIndex();
    voronoiIsInit = false;
}

void Mesh::printMesh() {

    for (unsigned int i=0; i<_faces.size(); ++i) {

        printf("Face %d :", i);

        printf("\n\tVertex 0: vi=%d", _faces[i].vertex(0));
        printf("\n\tVertex 1: vi=%d", _faces[i].vertex(1));
        printf("\n\tVertex 2: vi=%d\n", _faces[i].vertex(2));

    }

}
 
//Precondition : the face fi contains the newPoint
VERTEX_INDEX Mesh::split_face(const Point &newPoint, FACE_INDEX fi) {
    printf("Split face %d\n", fi);

    Vertex newVertex = Vertex(newPoint, fi);
    VERTEX_INDEX newVertexIndex = _vertices.size();

    //We add the vertex
    _vertices.push_back(newVertex);

    FACE_INDEX indexF1 = _faces.size();
    FACE_INDEX indexF2 = _faces.size() + 1;

    Face f1(    _faces[fi].vertex(1), _faces[fi].vertex(2), newVertexIndex,
                indexF2, fi, _faces[fi].getFrontFace(0));

    _faces.push_back(f1);

    Face f2(    _faces[fi].vertex(2), _faces[fi].vertex(0), newVertexIndex,
                fi, indexF1, _faces[fi].getFrontFace(1));

    _faces.push_back(f2);

    //Modify incident faces

    _vertices[_faces[fi].vertex(0)].setFi(indexF2);

    _vertices[_faces[fi].vertex(1)].setFi(fi);

    _vertices[_faces[fi].vertex(2)].setFi(indexF1);


    //Modify neighbor faces..    
    FACE_INDEX frontFace0 = _faces[fi].getFrontFace(0);
    FACE_INDEX frontFace1 = _faces[fi].getFrontFace(1);

    //Get local id
    int localId = _faces[frontFace0].getLocalIndexOfOppositeFromVertexIndex(_faces[fi].vertex(1), _faces[fi].vertex(2));
    _faces[frontFace0].setOppositeFace(indexF1, localId);

    localId = _faces[frontFace1].getLocalIndexOfOppositeFromVertexIndex(_faces[fi].vertex(2), _faces[fi].vertex(0));
    _faces[frontFace1].setOppositeFace(indexF2, localId);

    //Modify input face
    _faces[fi].setVertex(2, newVertexIndex);

    _faces[fi].setOppositeFace(indexF1, 0);
    _faces[fi].setOppositeFace(indexF2, 1);

    //Useless
    //f.setOppositeFace(f.getFrontFace(2), 2);

    voronoiIsInit = false;

    return newVertexIndex;
}


void Mesh::flip_edge(const FACE_INDEX f1, const FACE_INDEX f2, const VERTEX_INDEX v1, const VERTEX_INDEX v2) {
    //Precondition: v1, v2 are in faces f1 and f2 

    //Reference possible car pas de push_back = pas d'invalidation de la ref
    Face& F1 = _faces[f1];
    Face& F2 = _faces[f2];

    int f1_localIndex_v1 = F1.getLocalIndexOf(v1);
    int f1_localIndex_v2 = F1.getLocalIndexOf(v2);

    int f2_localIndex_v1 = F2.getLocalIndexOf(v1);
    int f2_localIndex_v2 = F2.getLocalIndexOf(v2);

    int f1_localIndex_opposite = F1.getLocalIndexOfOppositeFromLocalIndex(f1_localIndex_v1, f1_localIndex_v2);
    int f2_localIndex_opposite = F2.getLocalIndexOfOppositeFromLocalIndex(f2_localIndex_v1, f2_localIndex_v2);

    FACE_INDEX if1 = F1.getFrontFace(f1_localIndex_v2);
    FACE_INDEX if2 = F2.getFrontFace(f2_localIndex_v1);

    F1.setOppositeFace(if2, f1_localIndex_opposite);
    F1.setOppositeFace(f2, f1_localIndex_v2);

    F1.setVertex(f1_localIndex_v1, F2.vertex(f2_localIndex_opposite));

    F2.setOppositeFace(if1, f2_localIndex_opposite);
    F2.setOppositeFace(f1, f2_localIndex_v1);

    F2.setVertex(f2_localIndex_v2, F1.vertex(f1_localIndex_opposite));

    if (if1 != -1)
        _faces[if1].setOppositeFace(f2, (_faces[if1].getLocalIndexOf(v1) + 2) % 3);
    if (if2 != -1)
        _faces[if2].setOppositeFace(f1, (_faces[if2].getLocalIndexOf(v2) + 1) % 3);

    voronoiIsInit = false;
}

void Mesh::flip_edge(const FACE_INDEX f1, int localIndexF1) {
   //Split edge in front of localIndexF1
    
    VERTEX_INDEX v1 = _faces[f1].vertex(localIndexF1);
    VERTEX_INDEX v2 = _faces[f1].vertex((localIndexF1+1)%3);
    VERTEX_INDEX v3 = _faces[f1].vertex((localIndexF1+2)%3);

    //Get front face of f1
    FACE_INDEX f2 = _faces[f1].getFrontFace(localIndexF1);

    //Index of vertex in F2, in front of face F1
    int localvInF2_frontOfF1 = _faces[f2].getLocalIndexOfOppositeFromVertexIndex(v2,v3);
    int globalvInF2_frontOfF1 = _faces[f2].vertex(localvInF2_frontOfF1);

    //Incident faces that need to be modified:
    FACE_INDEX if1 = _faces[f1].getFrontFace((localIndexF1+1)%3);
    FACE_INDEX if2 = _faces[f2].getFrontFace((localvInF2_frontOfF1+1)%3);

    _faces[f1].setVertex((localIndexF1+2)%3, globalvInF2_frontOfF1);

    _faces[f2].setVertex((localvInF2_frontOfF1+1)%3, v3);
    _faces[f2].setVertex((localvInF2_frontOfF1+2)%3, v1);

    if (if1 != -1)
        _faces[if1].setOppositeFace(f2, _faces[if1].getLocalIndexOfOppositeFromVertexIndex(v1,v3));
    if (if2 != -1)
        _faces[if2].setOppositeFace(f1, _faces[if2].getLocalIndexOfOppositeFromVertexIndex(v2,globalvInF2_frontOfF1));

    voronoiIsInit = false;
}

void Mesh::addPointAndFlipToInfinite(std::vector<FACE_INDEX> idsExtHull, Point p) {
    assert(idsExtHull.size() > 0);
    unsigned int newVertexIndex = _vertices.size();
    //Incident face of the new point is one of the new faces that will be created
    //And we are sure that at least one face will be created
    _vertices.push_back(Vertex(p.x, p.y, p.z, _faces.size()));
    voronoiIsInit = false;

    int idOldNewFace = -1;

    //For all face who need a connection with the new point p, we create a new face (oriented couter-clockwise)
    for (unsigned i = 0; i < idsExtHull.size(); i++) {
        //We need to flip all infinite edge ( if frontFace == -1)
        for (unsigned j=0; j<3; ++j) {
            if (_faces[idsExtHull[i]].getFrontFace(j) == -1) {

                int idVertexOpposite = j;
                int idLocalV1 = (idVertexOpposite + 1) % 3;
                int idLocalV2 = (idVertexOpposite + 2) % 3;
                VERTEX_INDEX v1 = _faces[idsExtHull[i]].vertex(idLocalV1);
                VERTEX_INDEX v2 = _faces[idsExtHull[i]].vertex(idLocalV2);

                if (pred_orientation(p, _vertices[v2].getPoint(), _vertices[v1].getPoint()) > 0) {
                    //counter clockwise (the edge is visible from the point) = we create a face
                    FACE_INDEX newFaceIndex = _faces.size();
                    //modify opposite face of actual face
                    _faces[idsExtHull[i]].setOppositeFace(newFaceIndex, idVertexOpposite);

                    //Create the new face. The actual v2 has for opposite face the previous newFace (if it's not
                    //the first face) else -1, stored in idOldNewFace
                    _faces.push_back(Face(newVertexIndex, v2, v1, idsExtHull[i], idOldNewFace, -1));

                    //The v2 (index 2) of the old face has for opposite face the newFace
                    if (idOldNewFace != -1)
                        _faces[idOldNewFace].setOppositeFace(newFaceIndex, 2);

                    idOldNewFace = newFaceIndex;
                }
            }
        }
    }
}


//Insertion
void Mesh::naiveInsertion(const Point p) {
    std::vector<FACE_INDEX> idsExtHull;
    for (unsigned i = 0; i < _faces.size(); i++) {
        int isInTriangle = pred_inTriangle(_vertices[_faces[i].v1()].getPoint(), _vertices[_faces[i].v2()].getPoint(), _vertices[_faces[i].v3()].getPoint(), p);
        if (isInTriangle == 1) {
            split_face(p, i); //Split face add the new point in the triangle
            return; //No need to continue
        }
        
        //We store the face at the edge of the convex hull (ie. faces that have a link to the infinite face)
        if (_faces[i].haveInfiniteFaceInFront()) {
            idsExtHull.push_back(i);
        }
    }
    //idsExtHull is not empty
    //Add the new point outside the convex hull
    addPointAndFlipToInfinite(idsExtHull, p);

    lcalc->calculate(this);
}

void Mesh::resolved_conflict(const Faces_circulator &f, FACE_INDEX faceToResolve, Point p) {
    //don't test pred_inCercle for faceToResolve
    if (f.currentFaceIndex != faceToResolve) {
        //If p is in cercle, so we need to flip
        if (pred_inCercle(_vertices[(*f).v1()].getPoint(), _vertices[(*f).v2()].getPoint(), _vertices[(*f).v3()].getPoint(), p) >= 0) {
            //always flip with these vertices (consequence of the implementation of split_face)
            flip_edge(f.currentFaceIndex, faceToResolve, _faces[faceToResolve].v1(), _faces[faceToResolve].v2());
        }
    }
}

//Is the face delaunay?
bool Mesh::isDelaunay(const Face& f) {
    //The face is not a delaunay one if his neighbor faces contain a point 

    //On parcourt tous les sommets voisins

    // for(int i=0; i<3; ++i) {
        
    //     Faces_circulator fcBegin = incident_faces_circulator(f.vertex(i));
    //     Faces_circulator fc = fcBegin;

    //     do {

    //         //Pour tous les vertex
    //         for (int j=0; j<3; ++j) {

    //             //Ne regarder que le vertex qui n'est pas contenu dans le triangle

    //         }




    //         fc++;

    //     } while(fc != fcBegin);

    // }




}


void Mesh::delaunayInsertion(const Point p) {
    int isInTriangle;
    for (unsigned int i = 0; i < _faces.size(); i++) {

        isInTriangle = pred_inTriangle( _vertices[_faces[i].v1()].getPoint(), 
                                            _vertices[_faces[i].v2()].getPoint(), 
                                            _vertices[_faces[i].v3()].getPoint(), 
                                            p);
        if (isInTriangle >= 0) {

            VERTEX_INDEX newVertexIndex = split_face(p, i);

            //We can use the flip edge with localIndex of P and the 3 new faces

            //We know the 3 new faces => i, i+1, i+2
            //We will if necessary do flip_edge with i,i+1, or i+2 and the localIndex of
            //The newVertex in faces i,i+1 or i+2






            //we splited the face, but we need a circulator around 3 vertices of old face (so 2 of the modified and one of a new face)
            //Circulators iterate on faces who possible are in conflict with the new face

            for (int j = 0; j < 2; j++) {
                Faces_circulator fcBegin = incident_faces_circulator(_faces[i].vertex(j));
                Faces_circulator fc;
                for (fc = fcBegin, fc++; resolved_conflict(fc, i , p), fc != fcBegin; fc++) {
                    ;     
                }
            }

            Faces_circulator fcBegin = incident_faces_circulator(_faces[_faces.size() - 1].vertex(0));
            Faces_circulator fc;
            for (fc = fcBegin, fc++; resolved_conflict(fc, i , p), fc != fcBegin; fc++) {
                ;
            }
        }

    }

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
    //printf("Next face : %d\n", s);
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


// The following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
// Draw a vertex
void glVertexDraw(const Point & p) {
    glVertex3f(p.x, p.y, p.z);
}

void glVertexDraw(const Vertex & v) {
    glVertexDraw(v.getPoint());
}


void Mesh::glVertexIndexDraw(const VERTEX_INDEX vi, bool isCurrentFace) {
    //The normal
    const Vector& normal = lcalc->getNormal(vi);
    
    RGB rgbColor;

    //The curvature
    if (isCurrentFace) //Use color white
        glColor3f(1,1,1);
    else {
        //float curvature = lcalc->getCurvatureMapped(vi,0.f,360.f);
        //double r,g,b;
        //Convert curvature to rgb color
        //HSVToRGB(curvature, 1, 1, r, g, b);
        //glColor3d(r,g,b);

        //Get the rgb color
        rgbColor = lcalc->getRgbColor(vi);
        glColor3d(rgbColor.r,rgbColor.g,rgbColor.b);

    }
    glNormal3f(normal.x, normal.y, normal.z);
    const Point& p = _vertices[vi].getPoint();
    glVertexDraw(p);

}

void Mesh::glFaceDraw(const Face & f, bool isCurrentFace) {
    
    glBegin(GL_TRIANGLES);
    //glBegin(GL_TRIANGLE_STRIP);
    //glBegin(GL_TRIANGLE_FAN);

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
        glFaceDraw(_faces[i], (int)i == currentNeighborFace);
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
            glVertexIndexDraw(f.v1(),false);
            glVertexIndexDraw(f.v2(),false);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexIndexDraw(f.v2(),false);
            glVertexIndexDraw(f.v3(),false);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexIndexDraw(f.v3(),false);
            glVertexIndexDraw(f.v1(),false);
        glEnd();
    }

    drawSelectedPoints();
}


Point Mesh::computeCenterFace(const Face &f) {
    //fIndex = fc.currentFaceIndex;
    Point a = _vertices[f.v1()].getPoint();
    Point b = _vertices[f.v2()].getPoint();
    Point c = _vertices[f.v3()].getPoint();

    Point center = computeCenterOfCircumscribedCercle(a, b, c);

    return center;
}

void Mesh::initVoronoiDiagram() {
    //std::vector<std::vector<Point>> voronoiPoint;

    voronoi_points = std::vector<std::vector<Point>>();
    int i = 0;
    for (Vertices_iterator vi = vertices_iterator_begin(); vi != vertices_iterator_end(); vi++) {
        Faces_circulator fcBegin = incident_faces_circulator(i);
        Faces_circulator fc;
        
        std::vector<Point> localPoint;
        for (fc = fcBegin, fc++; localPoint.push_back(computeCenterFace(*fc)), fc != fcBegin; fc++) {

            ;

            //This code is in the computeCenterFace function (in order to not skip the first face,
            //We put this function in the loop...., maybe we could find an other way)
            // if (fc.currentFaceIndex == -1 || fc.refVertex == -1) {
            //     printf("break voronoi\n");
            //     break;
            // }
            // //fIndex = fc.currentFaceIndex;
            // Point a = _vertices[(*fc).v1()].getPoint();
            // Point b = _vertices[(*fc).v2()].getPoint();
            // Point c = _vertices[(*fc).v3()].getPoint();

            // Point center = computeCenterOfCircumscribedCercle(a, b, c);
            // localPoint.push_back(center);
        }
        voronoi_points.push_back(localPoint);
        i++;
    }
    voronoiIsInit = true;
}

void Mesh::drawVoronoiDiagram() {
    glColor3d(0,1,0); //Green

    if (!voronoiIsInit) {
        initVoronoiDiagram();
    }

    for (unsigned i = 0; i < voronoi_points.size(); ++i) {
        glBegin(GL_LINE_LOOP);
        for (unsigned j = 0; j < voronoi_points[i].size(); ++j) {
            glVertexDraw(voronoi_points[i][j]);
        }
        glEnd();
    }

}

void Mesh::splitFaceMiddle(int faceIndex) {

    if (faceIndex < 0 || faceIndex > (int)_faces.size())
        return;

    Face &f = _faces[faceIndex];
    Point middle = (_vertices[f.v1()].getPoint() + _vertices[f.v2()].getPoint() + _vertices[f.v3()].getPoint()) / 3.f;

    split_face(middle, faceIndex);

}

//Replace v_old by v_new and uptade opposite faces.
void Mesh::replace_vertex(FACE_INDEX f1, FACE_INDEX f2, VERTEX_INDEX v_new, VERTEX_INDEX v_old, FACE_INDEX face_id){
    Face &face = _faces[face_id];
    //update opposite face if the current face have f1 or f2 as oppossite
    for (int i = 0; i < 3; ++i) {
        if (face.getFrontFace(i) == f1) {
            FACE_INDEX new_opp_face = _faces[f1].getFrontFaceOf(v_old);
            face.setOppositeFace(new_opp_face, i);
            //in return
            int opp_local_tmp = _faces[f1].getLocalIndexOfOppositeFromVertexIndex(v_new, v_old);
            FACE_INDEX opp_tmp = _faces[f1].vertex(opp_local_tmp);
            int opp_local_in_opp_face = _faces[new_opp_face].getLocalIndexOfOppositeFromVertexIndex(v_new, opp_tmp);
            _faces[new_opp_face].setOppositeFace(face_id, opp_local_in_opp_face);
            break;
        } else if (face.getFrontFace(i) == f2) {
            FACE_INDEX new_opp_face = _faces[f2].getFrontFaceOf(v_old);
            face.setOppositeFace(new_opp_face, i);
            //in return
            int opp_local_tmp = _faces[f2].getLocalIndexOfOppositeFromVertexIndex(v_new, v_old);
            FACE_INDEX opp_tmp = _faces[f2].vertex(opp_local_tmp);
            int opp_local_in_opp_face = _faces[new_opp_face].getLocalIndexOfOppositeFromVertexIndex(v_new, opp_tmp);
            _faces[new_opp_face].setOppositeFace(face_id, opp_local_in_opp_face);
            break;
        }
    }

    //replace vertices
    int v_old_local = face.getLocalIndexOf(v_old);
    face.setVertex(v_old_local, v_new);
}

void Mesh::remove_edge(VERTEX_INDEX v1, VERTEX_INDEX v2) {
    //searching the two faces who hold these vextices
    FACE_INDEX f1;
    FACE_INDEX f2;
    int local_v_index1;
    int local_v_index2;
    //In order to avoid this loop, we can also change our edge definition (maybe a global face id and the index of the oppsite vertex of the edge in this face).
    for (int i = 0; i < _faces.size(); ++i) {
        local_v_index1 = _faces[i].getLocalIndexOf(v1);
        local_v_index2 = _faces[i].getLocalIndexOf(v2);
        if (local_v_index1 != -1 && local_v_index2 != -1) {
            f1 = (FACE_INDEX)i;
            break;
        }
    }
    int opposite = _faces[f1].getLocalIndexOfOppositeFromLocalIndex(local_v_index1, local_v_index2);
    f2 = _faces[f1].getFrontFace(opposite);

    //we use v1 as the new vertex and we remove v2, but before, we need to set neighbors. f1 and f2 will be removed
    //For all faces around v2, set v1 instead, and set the opposite faces.
    Faces_circulator fcBegin = incident_faces_circulator(v2);
    Faces_circulator fc;
    for (fc = fcBegin, fc++; replace_vertex(f1, f2, v1, v2, fc.currentFaceIndex), fc != fcBegin; fc++) {
        ;
    }

    //update v1 position
    const Point& p1 = _vertices[v1].getPoint();
    const Point& p2 = _vertices[v2].getPoint();
    Point c = center(p1, p2);
    _vertices[v1].setPoint(c);
    //remove v2
    _vertices.erase(_vertices.begin() + (v2 - 1));

}

void Mesh::edges_collapse(unsigned int n) {
    //Iterative collapse of the smallest edge of a surface
    //sort edges length
    //selection of the two vertices of sortest edge
    //merge this two in only one vertex (median point of the edge)
    //don't update face_tab (maybe just at the end) but after edge suppression, update distance values of all the edges of merged vertices

    //edge can be an id of face with the id of the opposite face (=> opposite edge)


    float lenght;// length of an edge
    //Construction of edges ordered by length
    typedef std::pair<VERTEX_INDEX, VERTEX_INDEX> Edge;

    //holds edges ordered by length (smallest first)
    std::priority_queue<std::pair<float, Edge>, std::vector<std::pair<float, Edge>>, CompareEdgeSize> edges_size;


    //needed to have ramdom acces in order to see if an edge is already in the priority queue
    std::map<Edge, bool> edges_container;

    for (int i = 0; i < _faces.size(); ++i) {
        for (int j = 0; j < 3; ++j) {//3 edges by face
            //edge creation
            VERTEX_INDEX v1;
            VERTEX_INDEX v2;
            v1 = _faces[i].vertex(std::max(0,j - 1));//first vertex of the edge
            v2 = _faces[i].vertex(std::min(j + 1, 2));//second
            Edge edge (v1, v2); //edge creation
            Edge edge2 (v2, v1); // IN ORDER TO ACCEPT <v1,v2> == <v2,v1>

            //adding edge..
            if (edges_container.find(edge) == edges_container.find(edge2)/* && edges_container.find(edge1) == edges_container.end()*/) {
                //compute length
                lenght = length(Vector(_vertices[v1].getPoint() - _vertices[v2].getPoint()));
                //..to map
                edges_container[edge] = true;
                //..to priority queue
                //edges_size.push(std::pair<length, edge>);
             }

        }
    }


    //collapse edges until the mesh have only n vertices
    while (_vertices.size() != n) {
        std::pair<float, Edge> edge_to_collapse = edges_size.top();
        remove_edge(edge_to_collapse.second.first, edge_to_collapse.second.second);

        //update map: edges size + remove edge entry in map & recompute neighbors edge sizes

        edges_size.pop();

    }




}
