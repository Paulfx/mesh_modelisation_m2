# Mesh Computational Geometry

by Langlade Maxime and Lafoix Paul (M2 image)

### Interface

##### At the top

The mesh is displayed in a GL_Display_Widget. The mesh is colorized depending on the value of the curvature of each vertex. The curvature is converted from HSV to RGB value.

##### At the left side

Infos about the mesh and buttons to create it (ie: pyramid, tetrahedron or open a OFF file).

The button Square2D create a 2D square that is used to add points (naively or with delaunay).

Warning: With our creation of the pyramid, because the vertex at the top has 2 opposite faces, it is possible that we never came back to the first neighboring face when using iterators...

##### At the middle-L

Iterators on the mesh.
You can chose the index of one vertex (ex. 0 for A) to be the reference vertex. This vertex is shown with a white dot with a little animation.

The buttons nextFace, prevFace allows you to visualize the neighboring faces. The current face has a white color. 

The buttons nextVertex, prevVertex iterates over the neighbooring vertices. The current vertex has a pink color and is shown with a little animation.

##### At the middle-R

Modification of the mesh.
You can split the mesh, by an insertion of point at the middle of Face Index (default face=0). Creation of two new faces and modification of one.

Given coordinates of a 2D point, it is also possible to insert naïvely this point in the mesh. Or to perform this insertion with a Delaunay triangulation. 

The insertion is possible only in case of 2D triangulations (you can use the square2D to test it).

##### At the right side

Thanks to voronoï diagram button, it is possible to display the voronoï diagram of the mesh. Work well on finite face like queen.off or tetrahedron.

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
If the face is at the edge of the convex hull (used in 2D), the opposite face of the vertex is set to -1.

The mesh contains informations about all faces and vertices. It's created from an '.off' file stored in the root directory of this project but it is possible to load it from a custom path.
During mesh loading, we use a map structure to set front faces of vertices. In order to add these informations we search shared edges. If no front face of a vertex was found, then we use an unexisting value as id of face (is value represente a fictitious vertex).

Also, it's possible to split face in order to insert a point in the middle of the triangle. This split manage all adjacencies updates, incrementally.

In order to stay in Delaunay triangulation after a split (insertion of new point), we also provide a flip function for update the edges.

Only in 2D: It is possible to add a new point in the convex hull, or outside naively or with delaunay

The voronoï diagram is computed thanks to the duality between voronoï and Delaunay. To display points we approximate the center of the circumscribed cercle of the triangle.

##### Files *mesh_iterators.cpp/.h*

Define 3 iterators for the mesh.

First, an iterator on all the vertices of the mesh (**VerticesIterator**)

Then, a circulator on neighboring faces of a vertex (**FacesCirculator**)

Finally a circulator on neighboring vertices of a vertex (**VerticesCirculator**). This circulator encapsulate a **facesCirculator** to iterate over the neighboors.

##### Files *laplacianCalc.cpp/.h*

Define a class that calculate the laplacian vector for each vertex of an input mesh. Contain functions to get the resulting curvature (mapped between values, or not) and the resulting normal for each vertex.

We store the colors in a vector in order to display it in the mesh.

##### Files *Utils.cpp/.h*

The Utils class provide a lot of useful functions, like predicates.
We implement all predicates asked, with no approximation:
* pred_orientation();
* pred_inTriangle()
* pred_inCercle()

Also there is the possibility for an approximation of center of a circumscribed circle in order to display voronoï diagram.
