#ifndef LAPLACIAN_H
#define LAPLACIAN_H

#include <vector>
#include "mesh.h"
#include "vec.h"
#include <algorithm>

class LaplacianCalc {

private:
	//std::vector<float> U;

	//The resulting vectors
	std::vector<Vector> laplacian;
	//The curvatures = the length of the laplacian vectors
	std::vector<float> curvatures;
	float minCurvature, maxCurvature;

public:

	LaplacianCalc() {};

    //Cotangente between two vectors
	float cotan(const Vector& Vi, const Vector& Vj);

	//Area of the triangle created from the two vectors
	//TODO signed area?
	float triangleArea(const Vector& v1, const Vector& v2);

	//Map value from [istart,istop] to [ostart,ostop] linearly
	float map(float value, float istart, float istop, float ostart, float ostop);

	//Compute the laplacian vectors for each vertex of the mesh,
	//And store it into laplacian, and curvatures
	void calculate(Mesh* mesh);

	//Accessors
	Vector getNormal(VERTEX_INDEX vi);
	float getCurvature(VERTEX_INDEX vi);
	//Return the curvature mapped from [minCurvature, maxCurvature] to [start,stop] 
	float getCurvatureMapped(VERTEX_INDEX vi, float start, float stop);

};


#endif // LAPLACIAN_H
