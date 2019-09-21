#ifndef MESH_ITERATORS_H
#define MESH_ITERATORS_H

// ------------------------------------------------------------------------
//						Forward declarations
// ------------------------------------------------------------------------

class Point;
typedef int FACE_INDEX;
typedef int VERTEX_INDEX;
class Vertex;
class Face;
class Mesh;

// ------------------------------------------------------------------------
//                        VERTICES_ITERATOR
// ------------------------------------------------------------------------


class Vertices_iterator {
    friend class Mesh;

private :
    Mesh* _mesh;
    VERTEX_INDEX _index; //Index du vertex actuel dans mesh
    Vertices_iterator(Mesh* mesh, VERTEX_INDEX index) : _mesh(mesh), _index(index) {}

public :

    Vertices_iterator(const Vertices_iterator& vi) : _mesh(vi._mesh), _index(vi._index) {}

    Vertices_iterator& operator++();
    Vertices_iterator operator++(int);
    const Vertex& operator*() const;
    friend bool operator !=(const Vertices_iterator&,
                            const Vertices_iterator&);
    friend bool operator ==(const Vertices_iterator&,
                            const Vertices_iterator&);

    VERTEX_INDEX getIndex() {
        return _index;
    }
};

// ------------------------------------------------------------------------
//                        FACES_CIRCULATOR
// ------------------------------------------------------------------------

class Faces_circulator {
    friend class Mesh;
    friend class Vertices_circulator;

    enum DIRECTION
    {
        BACKWARD = 2, // -1 + 3
        FORWARD  = 1
    };

private:
    Mesh* _mesh;
    FACE_INDEX currentFaceIndex;
    //Reference vertex
    VERTEX_INDEX refVertex;

    //Todo ctor affectation + operator
    //Faces_circulator(const Vertex& v, Mesh* mesh);
    Faces_circulator(Mesh* mesh, VERTEX_INDEX vi);
    
    //++ => direction = 1
    //== => direction = +2 = +3 - 1
    void update(DIRECTION dir);

public:

    Faces_circulator(const Faces_circulator& fc);

    Faces_circulator& operator++();
    Faces_circulator& operator--(int);
    Faces_circulator operator++(int);
    const Face& operator*() const;
    friend bool operator !=(const Faces_circulator&,
                            const Faces_circulator&);
    friend bool operator ==(const Faces_circulator&,
                            const Faces_circulator&);


    Faces_circulator() : _mesh(nullptr) { refVertex = 0; }; //TODO constrain functions

};

// ------------------------------------------------------------------------
//                        VERTICES_CIRCULATOR
// ------------------------------------------------------------------------

class Vertices_circulator {
    friend class Mesh;
    friend class Faces_circulator;

private:
    Mesh* _mesh;
    //We iterate over the neighbors of baseVertexIndex
    VERTEX_INDEX baseVertexIndex;
    //We use a faces_circulator to get the next vertices
    Faces_circulator fit;
    //The actual vertex
    VERTEX_INDEX currentVertexIndex;


    Vertices_circulator(Mesh* mesh, VERTEX_INDEX base);

    void update();

public:

    Vertices_circulator& operator++();
    Vertices_circulator& operator--(int);
    Vertices_circulator operator++(int);
    const Vertex& operator*() const;
    friend bool operator !=(const Vertices_circulator&,
                            const Vertices_circulator&);
    friend bool operator ==(const Vertices_circulator&,
                            const Vertices_circulator&);


    Vertices_circulator() : _mesh(nullptr), baseVertexIndex(0) {};
    Vertices_circulator(const Vertices_circulator& vc);

};




#endif // MESH_ITERATORS_H
