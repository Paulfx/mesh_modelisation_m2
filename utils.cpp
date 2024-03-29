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

//alternate version
Vector computeBarycentric(Point p1, Point p2, Point p3){
    Vector cb = p2 - p3;
    Vector ba = p1 - p2;
    Vector ca = p1 - p3;
    Vector ab = p2 - p1;

    float x = 2 * length2(cross(ba, cb));

    float a = length2(cb) * dot(ba, ca) / x;
    float b = length2(ca) * dot(ab, cb) / x;
    float c = length2(ba) * dot(ca, cb) / x;

    return Vector(a, b, c);
}

Point computeCenterOfCircumscribedCercle(Point a, Point b, Point c) {
    //sum tans need to be 1 (baricentric coordinates)
   //float tanA = tan_from_angle(c, a, b);
   // float tanB = tan_from_angle(a, b, c);
   // float tanC = tan_from_angle(b, c, a);

    Vector h = computeBarycentric(a, b, c);


    //convert baricentric to cartesian
    float cx = h.x * a.x + h.y * b.x + h.z * c.x;
    float cy = h.x * a.y + h.y * b.y + h.z * c.y;
    float cz = h.x * a.z + h.y * b.z + h.z * c.z;


    return Point(cx, cy, cz);
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
    if (o_dab == 1 && o_dbc == 1 && o_dca == 1 ) return 1;
    return -1;
}

int pred_inCercle(Point p, Point q, Point r, Point s) {
    float pqx = q.x - p.x;
    float pqy = q.y - p.y;
    float prx = r.x - p.x;
    float pry = r.y - p.y;
    float psx = s.x - p.x;
    float psy = s.y - p.y;

    Vector pq = Vector(pqx, pqy, pqx * pqx + pqy * pqy);
    Vector pr = Vector(prx, pry, prx * prx + pry * pry);
    Vector ps = Vector(psx, psy, psx * psx + psy * psy);

    float result = -dot(cross(pq, pr), ps);

    return result > 0 ? 1 : result < 0 ? -1 : 0;
}
