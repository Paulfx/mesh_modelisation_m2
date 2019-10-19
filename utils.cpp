#include "utils.h"


void HSVToRGB(double hue, double s, double v, double& r, double& g, double& b) {
    r=0, g=0, b=0;

    if (s == 0)
    {
        r = v;
        g = v;
        b = v;
    }
    else
    {
        int i;
        double f, p, q, t;

        if (hue == 360)
            hue = 0;
        else
            hue = hue / 60;

        i = (int)trunc(hue);
        f = hue - i;

        p = v * (1.0 - s);
        q = v * (1.0 - (s * f));
        t = v * (1.0 - (s * (1.0 - f)));

        switch (i)
        {
        case 0:
            r = v;
            g = t;
            b = p;
            break;

        case 1:
            r = q;
            g = v;
            b = p;
            break;

        case 2:
            r = p;
            g = v;
            b = t;
            break;

        case 3:
            r = p;
            g = q;
            b = v;
            break;

        case 4:
            r = t;
            g = p;
            b = v;
            break;

        default:
            r = v;
            g = p;
            b = q;
            break;
        }

    }
}

Point convertTo2d(Point p) {
    return Point(p.x, p.y, 0);
}

float tan_from_angle(Point p1, Point p2, Point p3){
    int sign = pred_orientation(p2,p3,p1);
    Vector bc (p2, p3);
    Vector ba (p2, p1);
    return sign * length((cross(bc, ba)) / dot(bc, ba));
}

// Predicates

//work in 2d
int pred_orientation(Point a,Point b, Point c) {
    //switch to 2d
    Point a2d = convertTo2d(a);
    Point b2d = convertTo2d(b);
    Point c2d = convertTo2d(c);
    Vector ab (a2d, b2d);
    Vector ac (a2d, c2d);

    float result = dot(cross(ab, ac), vec3(0, 0, 1));

    return result > 0 ? 1 : result < 0 ? -1 : 0;
}

int pred_inTriangle(Point a, Point b, Point c, Point d) {
    int o_dab = pred_orientation(d, a, b);
    int o_dbc = pred_orientation(d, b, c);
    int o_dca = pred_orientation(d, c, a);

    if (o_dab == 0 || o_dbc == 0 || o_dca == 0) return 0;
    if (o_dab == o_dbc == o_dca == 1 ) return 1;
    return -1;
}

Point compute_circumscribedCercle(Point a, Point b, Point c) {
    float tanA = tan_from_angle(c, a, b);
    float tanB = tan_from_angle(a, b, c);
    float tanC = tan_from_angle(b, c, a);
    return Point(tanB + tanC, tanC + tanA, tanA + tanB);
}

int pred_inCercle(Point a, Point b, Point c, Point d) {
   // Point center = compute_circumscribedCercle(a, b, c);
   //
   // float r = length(center - a);
   // float length(center - d);
   //


}
