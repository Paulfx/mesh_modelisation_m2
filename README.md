# Mesh Computational Geometry

by Langlade Maxime and Lafoix Paul (M2 image)

### Interface

##### At the top

The mesh is displayed in a GL_Display_Widget. The mesh is colorized depending on the value of the curvature of each vertex. The curvature is converted from HSV to RGB value.

##### At the left side

Infos about the mesh and buttons to create a pyramid (not functional), a tetrahedron or open a OFF file.

##### At the middle

Iterators on the mesh.
You can chose the index of one vertex (ex. 0 for A) to be the reference vertex. This vertex is shown with a white dot with a little animation.

The buttons nextFace, prevFace allows you to visualize the neighbooring faces. The current face has a white color. 

The buttons nextVertex, prevVertex iterates over the neighbooring vertices. The current vertex has a pink color.

##### At the right side

You can reset vertex index (ie. put the reference index to -1), as well as currentFace and currentVertex. It is useful to not have the currentFace, currentVertex and referenceVertex shown on the mesh.

You can show the mesh with wireframe or not.

The button test iterators iterates over all the vertices and faces of the mesh at once. Output in the terminal. 

You can exit the application.

### Code

##### Files *vec.cpp/.h* extracted from GKit [(Source)](https://forge.univ-lyon1.fr/Alexandre.Meyer/gkit2light)

Theses files define a Point structure, and a Vector struct with geometrical operations such as dot product, cross product, and so on..

##### Files *mesh.cpp/.h*

Theses files define the vertex, face and mesh classes. The vertex hold a Point (defined in *vec.h*) and one of his incident face.
The face contains 3 vertices, and holds an array that store the front face of its vertices. For example, if the first vertex of a face is **vertex 3**, the first element of the array **_f** will be the index of the front face of **vertex 3**. This informations is used to iterate over the neighboring faces of a vertex.

The mesh contains informations about all faces and vertices. It's created from an '.off' file stored in the root directory of this project but it is possible to load it from a custom path.
During mesh loading, we use a map structure to set front faces of vertices. In order to add these informations we search shared edges.

##### Files *mesh_iterators.cpp/.h*

Define 3 iterators for the mesh.

First, an iterator on all the vertices of the mesh (**VerticesIterator**)

Then, a circulator on neighboring faces of a vertex (**FacesCirculator**)

Finally a circulator on neighboring vertices of a vertex (**VerticesCirculator**). This circulator encapsulate a **facesCirculator** to iterate over the neighboors.

##### Files *laplacianCalc.cpp/.h*

Define a class that calculate the laplacian vector for each vertex of an input mesh. Contain functions to get the resulting curvature (mapped between values, or not) and the resulting normal for each vertex.