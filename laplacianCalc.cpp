#include "laplacianCalc.h"

LaplacianCalc::LaplacianCalc(Mesh* mesh) {
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

float LaplacianCalc::cotan(const Vertex& Vi, const Vertex& Vj) {
	//Transformation en vec3
	vec3 vi = vec3(Vi.getPoint());
	vec3 vj = vec3(Vj.getPoint());
	
	return dot(vi, vj) / length(cross(vi, vj));

}

void LaplacianCalc::calculate() {

	// int i = 0;
	// //Pour tout les vertex
	// for (Vertices_iterator vit = _mesh->vertices_iterator_begin(); vit != _mesh->vertices_iterator_end(); vit++) {

	// 	//Vi = *vit;

	// 	//Parcours des voisins
	// 	Vertices_circulator vcirc;

	// 	for (vcirc = _mesh->vertices_circulator_begin(i); )

	// 		//Vj = *vcirc

	// 		//Il faudrait prendre pour chaque voisin
	// 		//Le voisin -- et le voisin ++
			
	// 		//Vprev = vcirc--;
	// 		//Vnext = vcirc++:


	// 		//et faire 
	// 		//vec3 v1 = Vi - Vprev 
	// 		//vec3 v2 = Vj - Vprev 

	// 		//vec3 v3 = Vi - Vnext
	// 		//vec3 v4 = Vj - Vnext


	// }


}