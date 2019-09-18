#ifndef LAPLACIAN_H
#define LAPLACIAN_H

#include <vector>
#include "mesh.h"
#include "vec.h"

class LaplacianCalc {

private:

	std::vector<float> U;

	//Result
	std::vector<vec3> laplacian;

	Mesh* _mesh;
	unsigned int vertexNb;

public:

    LaplacianCalc(Mesh* mesh);

    //Cotangente entre deux vecteurs
	float cotan(const Vector& Vi, const Vector& Vj);

	void calculate();

};


#endif // LAPLACIAN_H
