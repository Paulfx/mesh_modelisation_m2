#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <vec.h>

//Code from www.programmingalgorithms.com/algorithm/hsv-to-rgb?lang=C
void HSVToRGB(double hue, double s, double v, double& r, double& g, double& b);
Point convertTo2d(Point p);
//compute tan of angle form by points p1,p2,p3,
float tan_from_angle(Point p1, Point p2, Point p3);

//Used to display voronoi
Point compute_circumscribedCercle(Point a, Point b, Point c);

// Predicates

//Given three 2D points (z=0), returning a positive value if these 3
//points are oriented counter-clockwise, negative if they are oriented clockwise or 0 if they are
//aligned for the arithmetic used to encode the coordinates.
int pred_orientation(Point p1, Point p2, Point p3);


//Given a 2D triangle, write the geometric predicate returning a positive value if an input
//point is located inside a triangle, negative if it is located outside or 0 if it is located on the boundary.
int pred_inTriangle(Point a, Point b, Point c, Point d);

//Test if d is in circumscribed circle define by a, b and c
int pred_inCercle(Point a, Point b, Point c, Point d);

#endif // UTILS_H
