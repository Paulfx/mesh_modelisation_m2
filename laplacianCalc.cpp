#include "laplacianCalc.h"

float LaplacianCalc::map(float value, float istart, float istop, float ostart, float ostop) {
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

float LaplacianCalc::cotan(const Vector& Vi, const Vector& Vj) {
	return dot(Vi, Vj) / length(cross(Vi, Vj));
}

float LaplacianCalc::triangleArea(const Vector& v1, const Vector& v2) {
    return 1/2.f * length(cross(v1,v2));
}

void LaplacianCalc::calculate(Mesh* mesh) {
    laplacian.clear();
	curvatures.clear();
	rgbColors.clear();
    
    int i = 0; //Get from vit? TODO

	Vertex vi,vj,vprev,vnext; //Use to construct the differents vectors
	Vector v1,v2,v3,v4; //Vectors to compute the cotan angle
	//The cotan angle
	float cotAlpha, cotBeta;
	//The sum of the cotan angles
	Vector sum;
	//Local triangle area;
	float area;
	bool firstLoop;

	Vertices_circulator vcircFirst;

    for (Vertices_iterator vit = mesh->vertices_iterator_begin(); vit != mesh->vertices_iterator_end(); vit++) {
		vi = *vit;
        vcircFirst = mesh->vertices_circulator_begin(i);
		sum = Vector(0.f,0.f,0.f);
        area = 0.f;
        firstLoop = true;
    	unsigned int n=0; //To not infinite loop
        for (Vertices_circulator vcirc = vcircFirst--; vcirc != mesh->vertices_circulator_begin(i); vcirc++) {
            if (firstLoop) {
            	vcirc++; //We increment only once to pass the test of the for loop
				firstLoop = false;
			}
			if (n++ > 15) break; //Avoid infinite loop if the mesh is not well constructed
			vj = *vcirc;

			vprev = *vcirc--; //Previous vertex
			vnext = *vcirc++; //Next vertex = actual

			v1 = vi - vprev;
			v2 = vj - vprev;
			v3 = vi - vnext;
			v4 = vj - vnext;

			cotAlpha = cotan(v1, v2);
			cotBeta = cotan(v3, v4);

			//Sum of the cotan angles
			sum = sum + (cotAlpha + cotBeta) * (vj - vi);
			//Sum of the triangles
            area += triangleArea(vprev- vi, vj - vi);
		}

		//1/3 (not barycentric)	
		area /= 3.f;
        laplacian.push_back(sum);
        curvatures.push_back(getCurvature(i));
        i++;
	}

	//Compute min and max curvature
    minCurvature = *std::min_element(curvatures.begin(), curvatures.end());
    maxCurvature = *std::max_element(curvatures.begin(), curvatures.end());

    //Compute rgb colors
    fillRgbColors();

}

void LaplacianCalc::fillRgbColors() {
	double r,g,b;
	float curvature;
	for (int i = 0; i < curvatures.size(); ++i) {
		curvature = getCurvatureMapped(i, 0.f, 360.f);
		//Convert curvature to rgb color
   		HSVToRGB(curvature, 1, 1, r, g, b);
   		rgbColors.push_back(RGB(r,g,b));
	}
}

RGB LaplacianCalc::getRgbColor(VERTEX_INDEX vi) {
	return rgbColors[vi];
}

Vector LaplacianCalc::getNormal(VERTEX_INDEX vi) {
	return normalize(laplacian[vi]);
}

float LaplacianCalc::getCurvatureMapped(VERTEX_INDEX vi, float start, float stop) {
	//Map from [min,max] to [start,stop];
	//float curvature = 1/2.f * length(laplacian[vi]); //TODO store curvature to not recalculate it
	float curvature = curvatures[vi];
	float curvatureMapped = map(curvature, minCurvature, maxCurvature, start, stop); 
	return curvatureMapped;
}

float LaplacianCalc::getCurvature(VERTEX_INDEX vi) {
	return 1/2.f * length(laplacian[vi]);
}
