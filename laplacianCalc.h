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

	std::vector<float> curvatures;
	float minCurvature, maxCurvature;

	Mesh* _mesh;
	unsigned int vertexNb;

public:

	LaplacianCalc() {};
    LaplacianCalc(Mesh* mesh);

    //Cotangente between two vectors
	float cotan(const Vector& Vi, const Vector& Vj);
	//Aire du triangle formé par les deux vecteurs
	//TODO aire signée? convexe/pas convexe
	float aireTriangle(const Vector& v1, const Vector& v2);

	//Map value from [istart,istop] to [ostart,ostop] linearly
	float map(float value, float istart, float istop, float ostart, float ostop);

	void calculate();

	//Accesseurs
	Vector getNormal(VERTEX_INDEX vi);
	float getCurvature(VERTEX_INDEX vi);
	float getCurvatureMapped(VERTEX_INDEX vi, float start, float stop);

};


#endif // LAPLACIAN_H
