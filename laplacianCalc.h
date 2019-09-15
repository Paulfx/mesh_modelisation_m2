#ifndef LAPLACIAN_H
#define LAPLACIAN_H

#include <vector>
#include "mesh.h"

class LaplacianCalc {


private:

	std::vector<float> U;


	//Result
	std::vector<vec3> laplacian;

	Mesh* _mesh;
	unsigned int vertexNb;

public:

	Laplacian(Mesh* mesh) {
		_mesh = mesh;
		//Create function U for each vertex
		vertexNb = mesh->vertexNb();


		//Calcul:

		//Pour tout les vertex
		//On calcule son laplacien (un vec3)
		//En parcourant tous ses vertex voisins via un circulator

		//Et en sommant la formule de la cotangente

		//valeur de Ai => tiers de l'aire de la face

		//Ou bien barycentrique..




	}


	float cotan(const Vertex& Vi, const Vertex& Vj) {

		vec3 res;

		//(Vi.dot(Vj) / Vi.cross(Vj)).normalized()

		//Quelle classe de vector utiliser?
		//Cross, dot...
		//GKIT ?

		return res;
	}



};


#endif // LAPLACIAN_H
