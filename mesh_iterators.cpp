#include "mesh_iterators.h"
#include "mesh.h"

// ------------------------------------------------------------------------
//                        VERTICES_ITERATOR
// ------------------------------------------------------------------------

const Vertex& Vertices_iterator::operator*() const { 
    return _mesh->_vertices[_index];
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

Faces_circulator::Faces_circulator(Mesh* mesh, VERTEX_INDEX vi) : _mesh(mesh), refVertex(vi) {
    //At first, the neighboring face is the incident face of the vertex vi
    currentFaceIndex = mesh->_vertices[vi].getIncidentFace();
}

Faces_circulator::Faces_circulator(const Faces_circulator& fc) 
: _mesh(fc._mesh), refVertex(fc.refVertex), currentFaceIndex(fc.currentFaceIndex) {}

void Faces_circulator::update(DIRECTION dir) {
    //Actual face
    const Face& actualFace = _mesh->_faces[currentFaceIndex];

    //Get index of the ref vertex in the actual face
    int indexRefVertex = actualFace.getLocalIndexOf(refVertex);

    if (indexRefVertex == -1) {
        fprintf(stderr, "Error Faces_circulator indexrefVertex=-1, refVertex=%d, actualFace=%d\n", refVertex, currentFaceIndex);
        
        fprintf(stderr, "Face %d, v0=%d, v1=%d, v2=%d\n", currentFaceIndex, actualFace.v1(), actualFace.v2(), actualFace.v3());
        //indexRefVertex = 0;
    }

    //Next face is the front face of indexRefVertex +- 1 (mod 3)
    VERTEX_INDEX nextVertex = (indexRefVertex + dir) % 3;
    currentFaceIndex = actualFace.getFrontFace(nextVertex);
}

const Face& Faces_circulator::operator*() const {
    //-1 if the refVertex has no front face, we return first element to stop loops...
    if (currentFaceIndex == -1) return *_mesh->incident_faces_circulator(refVertex);
    return _mesh->_faces[currentFaceIndex];
}

Faces_circulator& Faces_circulator::operator++() {
    update(FORWARD);
    return *this;
}

Faces_circulator& Faces_circulator::operator--(int) {
    update(BACKWARD);
    return *this;
}

Faces_circulator Faces_circulator::operator++(int) {
    Faces_circulator tmp(*this);
    update(FORWARD);
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
    //First vertex
    update();
}

Vertices_circulator::Vertices_circulator(const Vertices_circulator& vc)
: _mesh(vc._mesh), baseVertexIndex(vc.baseVertexIndex), fit(vc.fit), currentVertexIndex(vc.currentVertexIndex) {}


void Vertices_circulator::update() {
    const Face& currentFace = *fit;
    //Next vertex is the local index of baseVertex in the current face + 1
    int indexOfBaseVertexInCurrFace = currentFace.getLocalIndexOf(baseVertexIndex);

    if (indexOfBaseVertexInCurrFace == -1) {
        //We shouldn't get here, it mean that the mesh is not well constructed
        printf("indexOfBaseVertexInCurrFace == -1, currVertex=%d, baseVertex=%d\n", currentVertexIndex, baseVertexIndex);
    }

    int nextVertex = (indexOfBaseVertexInCurrFace + 1) % 3;
    currentVertexIndex = currentFace.vertex(nextVertex);
}

const Vertex& Vertices_circulator::operator*() const {
    return _mesh->_vertices[currentVertexIndex];
}

Vertices_circulator& Vertices_circulator::operator++() {
    //We go to the next face
    fit++;
    update();
    return *this;
}

Vertices_circulator& Vertices_circulator::operator--(int) {
    //We go to the previous face
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

