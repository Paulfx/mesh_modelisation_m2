#include "mesh_iterators.h"
#include "mesh.h"

// ------------------------------------------------------------------------
//                        VERTICES_ITERATOR
// ------------------------------------------------------------------------

const Vertex& Vertices_iterator::operator*() const { 
    return _mesh->vertexTab[_index];
}

Vertices_iterator& Vertices_iterator::operator++() {
    _index++;
    return *this;
}

Vertices_iterator Vertices_iterator::operator++(int) {
    Vertices_iterator tmp(*this);
    _index++;
    return tmp;
}

bool operator !=(   const Vertices_iterator& vi1,
                    const Vertices_iterator& vi2) {
    return vi1._index != vi2._index;
}

bool operator ==(   const Vertices_iterator& vi1,
                    const Vertices_iterator& vi2) {
    return vi1._index == vi2._index;
}

// ------------------------------------------------------------------------
//                        FACES_CIRCULATOR
// ------------------------------------------------------------------------

// Faces_circulator::Faces_circulator(Mesh* mesh, const Vertex& v) : _mesh(mesh), startVertexIndex(0) {

//     //Indice de la face en face de v
//     frontFaceIndexOfStartVertex = 0;

//     //frontFaceIndexOfStartVertex = _mesh->faceTab[v.fi()].getFrontFaceOf(); //TODO, pb poujr trouver index du vertex..

//     update();
// }

Faces_circulator::Faces_circulator(Mesh* mesh, VERTEX_INDEX vi) : _mesh(mesh), startVertexIndex(0) {

    frontFaceIndexOfStartVertex = _mesh->faceTab[_mesh->vertexTab[vi].getIncidentFace()].getFrontFaceOf(vi);

    update();
}

Faces_circulator::Faces_circulator(const Faces_circulator& fc) 
: _mesh(fc._mesh), frontFaceIndexOfStartVertex(fc.frontFaceIndexOfStartVertex), 
    startVertexIndex(fc.startVertexIndex), currentFaceIndex(fc.currentFaceIndex) {}


void Faces_circulator::update() {
    const Face& frontFaceOfStartVertex = _mesh->faceTab[frontFaceIndexOfStartVertex];
    VERTEX_INDEX nextVertex = frontFaceOfStartVertex.vertex(startVertexIndex);
    
    //currentFaceIndex = _mesh->vertexTab[nextVertex].fi();
    
    currentFaceIndex = frontFaceOfStartVertex.getFrontFaceOf(nextVertex);

    //startVertexIndex = (startVertexIndex + 1) % 3; 
}

const Face& Faces_circulator::operator*() const {
    
    return _mesh->faceTab[currentFaceIndex];
}

Faces_circulator& Faces_circulator::operator++() {
    startVertexIndex = (startVertexIndex + 1) % 3;
    update();
    return *this;
}

Faces_circulator& Faces_circulator::operator--(int) {
    if (startVertexIndex == 0) startVertexIndex = 2;
    else startVertexIndex = (startVertexIndex - 1) % 3;
    update();
    return *this;
}

Faces_circulator Faces_circulator::operator++(int) {
    Faces_circulator tmp(*this);
    startVertexIndex = (startVertexIndex + 1) % 3;
    update();
    return tmp;
}

bool operator !=(   const Faces_circulator& fc1,
                    const Faces_circulator& fc2) {
    return fc1.currentFaceIndex != fc2.currentFaceIndex;
}

bool operator ==(   const Faces_circulator& fc1,
                    const Faces_circulator& fc2) {
    return fc1.currentFaceIndex == fc2.currentFaceIndex;
}

// ------------------------------------------------------------------------
//                        VERTICES_CIRCULATOR
// ------------------------------------------------------------------------

Vertices_circulator::Vertices_circulator(Mesh* mesh, VERTEX_INDEX base) : _mesh(mesh), baseVertexIndex(base), fit(mesh, base) {

    //premier sommet
    update();
}

Vertices_circulator::Vertices_circulator(const Vertices_circulator& vc)
: _mesh(vc._mesh), baseVertexIndex(vc.baseVertexIndex), fit(vc.fit), currentVertexIndex(vc.currentVertexIndex) {}


void Vertices_circulator::update() {


    //currentvertexIndex doit ê != de baseVertexIndex..
    //On doit connaitre l'indice du base vertex dans les faces voisines qu'on
    //Parcours..
    const Face& currentFace = *fit;
    //Le sommet voisin est l'indice de baseVertex dans currentFace + 1
    VERTEX_INDEX indexOfBaseVertexInCurrFace = currentFace.getIndexOf(baseVertexIndex);
    
    if (indexOfBaseVertexInCurrFace == -1) {
        //GROS PROBLEME
        printf("indexOfBaseVertexInCurrFace == -1, ERROR ! \n");
    }
    VERTEX_INDEX nextVertex = (indexOfBaseVertexInCurrFace + 1)%3;
    currentVertexIndex = currentFace.vertex(nextVertex);
}

const Vertex& Vertices_circulator::operator*() const {
    return _mesh->vertexTab[currentVertexIndex];
}

Vertices_circulator& Vertices_circulator::operator++() {
    //On change de face
    fit++;
    update();
    return *this;
}

Vertices_circulator& Vertices_circulator::operator--(int) {
    //On change de face
    fit--;
    update();
    return *this;
}

Vertices_circulator Vertices_circulator::operator++(int) {
    Vertices_circulator tmp(*this);
    fit++;
    update();
    return tmp;
}

bool operator !=(   const Vertices_circulator& vc1,
                    const Vertices_circulator& vc2) {
    return vc1.currentVertexIndex != vc2.currentVertexIndex;
}

bool operator ==(   const Vertices_circulator& vc1,
                    const Vertices_circulator& vc2) {
    return vc1.currentVertexIndex == vc2.currentVertexIndex;
}

