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

float LaplacianCalc::cotan(const Vector& Vi, const Vector& Vj) {
	return dot(Vi, Vj) / length(cross(Vi, Vj));
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


	// } TODO !!!!

	// int i = 0; //Get from vit?

	// Vertex vi;
	// Vertex vj;
	// Vertex vprev;
	// Vertex vnext;

	// //Les vecteurs
	// Vector v1,v2,v3,v4;

	// //Les deux angles
	// float cotAlpha, cotBeta;

	// for (Vertices_iterator vit = _mesh->vertices_iterator_begin(); vit != _mesh->vertices_iterator_end(); vit++) {

	// 	vi = *vit;

	// 	for (Vertices_circulator vcirc = _mesh->vertices_circulator_begin(i); ; vcirc++) {

	// 		vj = *vcirc;

	// 		//Voisin précédent
	// 		vprev = *vcirc--;
	// 		vnext = *vcirc++;

	// 		v1 = vi - vprev;
	// 		v2 = vj - vprev;
	// 		v3 = vi - vnext;
	// 		v4 = vj - vnext;

	// 		cotAlpha = cotan(v1, v2);
	// 		cotBeta = cotan(v3, v4);

	// 	}


	// }

}