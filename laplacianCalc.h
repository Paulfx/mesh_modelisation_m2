#ifndef LAPLACIAN_H
#define LAPLACIAN_H

#include <vector>
#include "mesh.h"
#include "vec.h"

class LaplacianCalc {

private:

	//std::vector<float> U;

	//Result
	std::vector<Vector> laplacian;

	Mesh* _mesh;
	unsigned int vertexNb;

public:

	LaplacianCalc() {};
    LaplacianCalc(Mesh* mesh);

    //Cotangente entre deux vecteurs
	float cotan(const Vector& Vi, const Vector& Vj);
	//Aire du triangle formé par les deux vecteurs
	//TODO aire signée? convexe/pas convexe
	float aireTriangle(const Vector& v1, const Vector& v2);

	void calculate();

	//Accesseurs
	Vector getNormal(VERTEX_INDEX vi);
	float getCurvature(VERTEX_INDEX vi);

};


#endif // LAPLACIAN_H
