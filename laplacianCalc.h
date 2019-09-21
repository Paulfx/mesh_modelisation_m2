#ifndef LAPLACIAN_H
#define LAPLACIAN_H

#include <vector>
#include "mesh.h"
#include "vec.h"
#include <algorithm>

class LaplacianCalc {

private:

	//std::vector<float> U;

	//Result
	std::vector<Vector> laplacian;
	//The curvatures = the length of the laplacian vectors
	std::vector<float> curvatures;
	float minCurvature, maxCurvature;

	//Mesh* _mesh;
	//unsigned int vertexNb; //Number of vertices of the mesh
public:

	LaplacianCalc() {};

    //Cotangente between two vectors
	float cotan(const Vector& Vi, const Vector& Vj);

	//Area of the triangle created from the two vectors
	//TODO aire signée? convexe/pas convexe
	float triangleArea(const Vector& v1, const Vector& v2);

	//Map value from [istart,istop] to [ostart,ostop] linearly
	float map(float value, float istart, float istop, float ostart, float ostop);

	void calculate(Mesh* mesh);

	//Accesseurs
	Vector getNormal(VERTEX_INDEX vi);
	float getCurvature(VERTEX_INDEX vi);
	float getCurvatureMapped(VERTEX_INDEX vi, float start, float stop);

};


#endif // LAPLACIAN_H
