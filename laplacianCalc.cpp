#include "laplacianCalc.h"

LaplacianCalc::LaplacianCalc(Mesh* mesh) {
	_mesh = mesh;
	//Create function U for each vertex
	//TODO comprendre U

	vertexNb = mesh->vertexNb();


	//Calcul:

	//Pour tout les vertex
	//On calcule son laplacien (un vec3)
	//En parcourant tous ses vertex voisins via un circulator

	//Et en sommant la formule de la cotangente

	//valeur de Ai => tiers de l'aire de la face
	//Ou bien barycentrique..

}

float LaplacianCalc::map(float value, float istart, float istop, float ostart, float ostop) {
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

float LaplacianCalc::cotan(const Vector& Vi, const Vector& Vj) {
	return dot(Vi, Vj) / length(cross(Vi, Vj));
}

float LaplacianCalc::aireTriangle(const Vector& v1, const Vector& v2) {

    return 1/2.f * length(cross(v1,v2));
}

void LaplacianCalc::calculate() {
    int i = 0; //Get from vit? TODO
	Vertex vi;
	Vertex vj;
	Vertex vprev;
	Vertex vnext;

	//Les vecteurs
	Vector v1,v2,v3,v4;

	//Les deux angles
	float cotAlpha, cotBeta;

	//La somme
	Vector sum;
	//Aire des triangles locaux;
	float aire;

	Vertices_circulator vcircFirst;

    for (Vertices_iterator vit = _mesh->vertices_iterator_begin(); vit != _mesh->vertices_iterator_end(); vit++) {

		vi = *vit;
        vcircFirst = _mesh->vertices_circulator_begin(i); //TODO change
		sum = Vector(0.f,0.f,0.f);
        aire = 0.f;

        //printf("Vertex %d\n", i);

        for (Vertices_circulator vcirc = vcircFirst--; vcirc != _mesh->vertices_circulator_begin(i); vcirc++) {
            if (aire==0.f) vcirc++; //On incrémente la première fois pour passer le test du for

			vj = *vcirc;

			//Voisin précédent
			vprev = *vcirc--;
			vnext = *vcirc++;


			v1 = vi - vprev;
			v2 = vj - vprev;
			v3 = vi - vnext;
			v4 = vj - vnext;

			cotAlpha = cotan(v1, v2);
			cotBeta = cotan(v3, v4);

			//Somme
			sum = sum + (cotAlpha + cotBeta) * (vj - vi);
			//+ somme aire, à partir des faces ?
            aire += aireTriangle(vprev- vi, vj - vi);

            //printf("Aire triangle %f\n", aireTriangle(vprev-vi, vj-vi));

		}

		//1/3	
		aire /= 3.f;
        laplacian.push_back(sum);

        curvatures.push_back(getCurvature(i));

        i++;
	}


    // for (i = 0; i < laplacian.size(); ++i) {

    //     printf("Laplacian vertex %d, x=%f, y=%f, z=%f, norme=%f\n", i, laplacian[i].x, laplacian[i].y, laplacian[i].z, length(laplacian[i]));

    // }

	//Compute min and max curvature
    minCurvature = *std::min_element(curvatures.begin(), curvatures.end());
    maxCurvature = *std::max_element(curvatures.begin(), curvatures.end());
}

Vector LaplacianCalc::getNormal(VERTEX_INDEX vi) {
	return normalize(laplacian[vi]);
}

float LaplacianCalc::getCurvatureMapped(VERTEX_INDEX vi, float start, float stop) {

	//Map from [min,max] to [start,stop];
	float curvature = 1/2.f * length(laplacian[vi]); //TODO store curvature to not recalculate it

	float curvatureMapped = map(curvature, minCurvature, maxCurvature, start, stop); 

	return curvatureMapped;
}

float LaplacianCalc::getCurvature(VERTEX_INDEX vi) {
	return 1/2.f * length(laplacian[vi]);
}
