# Mesh Computational Geometry

by Langlade Maxime and Lafoix Paul (M2 image)

### Interface

##### At the top

The mesh is displayed in a GL_Display_Widget. The mesh is colorized depending on the value of the curvature of each vertex. The curvature is converted from HSV to RGB value.

##### At the left	

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

