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
    unsigned int _index; //Index du vertex actuel dans mesh
    Vertices_iterator(Mesh* mesh, unsigned int index) : _mesh(mesh), _index(index) {}

public :

    Vertices_iterator(const Vertices_iterator& vi) : _mesh(vi._mesh), _index(vi._index) {}

    Vertices_iterator& operator++();
    Vertices_iterator operator++(int);
    const Vertex& operator*() const;
    friend bool operator !=(const Vertices_iterator&,
                            const Vertices_iterator&);
    friend bool operator ==(const Vertices_iterator&,
                            const Vertices_iterator&);
};

// ------------------------------------------------------------------------
//                        FACES_CIRCULATOR
// ------------------------------------------------------------------------

class Faces_circulator {
    friend class Mesh;
    friend class Vertices_circulator;

private:
    Mesh* _mesh;
    //L'indice de la face dont on parcourt les sommets
    FACE_INDEX frontFaceIndexOfStartVertex; 

    VERTEX_INDEX startVertexIndex;
    FACE_INDEX currentFaceIndex;

    //Todo ctor affectation + operator
    Faces_circulator(const Vertex& v, Mesh* mesh);
    Faces_circulator(VERTEX_INDEX vi, Mesh* mesh);
    

    void update();

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


    Faces_circulator() : _mesh(nullptr) {}; //TODO constrain functions

};

// ------------------------------------------------------------------------
//                        VERTICES_CIRCULATOR
// ------------------------------------------------------------------------

class Vertices_circulator {
    friend class Mesh;
    friend class Faces_circulator;

private:
    Mesh* _mesh;
    //Le vertex de référence (autour duquel on cherche les voisins)
    VERTEX_INDEX baseVertexIndex;
    //Version 1.0 : à l'aide d'un faces_circulator
    Faces_circulator fit;
    //Le vertex actuel
    VERTEX_INDEX currentVertexIndex;


    Vertices_circulator(Mesh* mesh, VERTEX_INDEX base);
    Vertices_circulator(const Vertices_circulator& vc);

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


};




#endif // MESH_ITERATORS_H
