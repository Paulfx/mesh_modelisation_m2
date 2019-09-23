#include "gldisplaywidget.h"
#ifdef __APPLE__
    #include <glu.h>
#else
    #include <GL/glu.h>
#endif

#include <math.h>
#include "QDebug"

GLDisplayWidget::GLDisplayWidget(QWidget *parent) : QGLWidget(parent)
{
    // Update the scene
    connect( &_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    _timer.start(16);
}

void GLDisplayWidget::initializeGL()
{
    // background color
    glClearColor(0.2, 0.2, 0.2, 1);

    // Shader
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    // Construction of the mesh before it is displayed
    // To add....
}

void GLDisplayWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Center the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,10,  0,0,0,   0,1,0);

    // Translation
    glTranslated(_X, _Y, _Z);

    // Rotation
    glRotatef(_angleX, 1.0f, 1.0f, 0.0f);

    // Color for your mesh
    glColor3f(0, 1 ,0);

    // example with a tetraedre
    if (wireframe) _mesh.drawMeshWireFrame(); 
    else _mesh.drawMesh();
}



void GLDisplayWidget::resizeGL(int width, int height){

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 1000.0f);

    updateGL();
}

void GLDisplayWidget::testIterators() {

    Point p;
    int i = 0;
    for (Vertices_iterator vi = _mesh.vertices_iterator_begin(); vi != _mesh.vertices_iterator_end(); vi++) {
        
        p = (*vi).getPoint();

        printf("Sommet %d : x=%f, y=%f, z=%f\n", i, p.x, p.y, p.z);

        Faces_circulator fcBegin = _mesh.incident_faces_circulator(i);
        Faces_circulator fc;
        int cp = 0;
        for (fc = fcBegin, fc++; cp++, fc != fcBegin; fc++)
            ;
        printf("Compteur de faces = %d\n", cp);

        cp=0;
        Vertices_circulator vcircFirst = _mesh.vertices_circulator_begin(i); //Todo ctor with vertex?
        for (Vertices_circulator vcirc = vcircFirst--; vcirc != _mesh.vertices_circulator_begin(i); vcirc++) {
            if (cp==0) vcirc++;
            cp++;
        }
        printf("Compteur de vertex voisins = %d\n", cp);
        i++;

    }

}

void GLDisplayWidget::setWireframe() {
    wireframe = !wireframe;
}

// - - - - - - - - - - - - Mouse Management  - - - - - - - - - - - - - - - -
// When you click, the position of your mouse is saved
void GLDisplayWidget::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL )
        _lastPosMouse = event->pos();
}

//void glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
//                  float znear, float zfar)
//{
//    float temp, temp2, temp3, temp4;
//    temp = 2.0 * znear;
//    temp2 = right - left;
//    temp3 = top - bottom;
//    temp4 = zfar - znear;
//    matrix[0] = temp / temp2;
//    matrix[1] = 0.0;
//    matrix[2] = 0.0;
//    matrix[3] = 0.0;
//    matrix[4] = 0.0;
//    matrix[5] = temp / temp3;
//    matrix[6] = 0.0;
//    matrix[7] = 0.0;
//    matrix[8] = (right + left) / temp2;
//    matrix[9] = (top + bottom) / temp3;
//    matrix[10] = (-zfar - znear) / temp4;
//    matrix[11] = -1.0;
//    matrix[12] = 0.0;
//    matrix[13] = 0.0;
//    matrix[14] = (-temp * zfar) / temp4;
//    matrix[15] = 0.0;
//}

//void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
//                      float znear, float zfar)
//{
//    float ymax, xmax;
//    float temp, temp2, temp3, temp4;
//    ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
//    // ymin = -ymax;
//    // xmin = -ymax * aspectRatio;
//    xmax = ymax * aspectRatio;
//    glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
//}

//bool gluInvertMatrix(const double m[16], double invOut[16])
//{
//    double inv[16], det;
//    int i;

//    inv[0] = m[5]  * m[10] * m[15] -
//             m[5]  * m[11] * m[14] -
//             m[9]  * m[6]  * m[15] +
//             m[9]  * m[7]  * m[14] +
//             m[13] * m[6]  * m[11] -
//             m[13] * m[7]  * m[10];

//    inv[4] = -m[4]  * m[10] * m[15] +
//              m[4]  * m[11] * m[14] +
//              m[8]  * m[6]  * m[15] -
//              m[8]  * m[7]  * m[14] -
//              m[12] * m[6]  * m[11] +
//              m[12] * m[7]  * m[10];

//    inv[8] = m[4]  * m[9] * m[15] -
//             m[4]  * m[11] * m[13] -
//             m[8]  * m[5] * m[15] +
//             m[8]  * m[7] * m[13] +
//             m[12] * m[5] * m[11] -
//             m[12] * m[7] * m[9];

//    inv[12] = -m[4]  * m[9] * m[14] +
//               m[4]  * m[10] * m[13] +
//               m[8]  * m[5] * m[14] -
//               m[8]  * m[6] * m[13] -
//               m[12] * m[5] * m[10] +
//               m[12] * m[6] * m[9];

//    inv[1] = -m[1]  * m[10] * m[15] +
//              m[1]  * m[11] * m[14] +
//              m[9]  * m[2] * m[15] -
//              m[9]  * m[3] * m[14] -
//              m[13] * m[2] * m[11] +
//              m[13] * m[3] * m[10];

//    inv[5] = m[0]  * m[10] * m[15] -
//             m[0]  * m[11] * m[14] -
//             m[8]  * m[2] * m[15] +
//             m[8]  * m[3] * m[14] +
//             m[12] * m[2] * m[11] -
//             m[12] * m[3] * m[10];

//    inv[9] = -m[0]  * m[9] * m[15] +
//              m[0]  * m[11] * m[13] +
//              m[8]  * m[1] * m[15] -
//              m[8]  * m[3] * m[13] -
//              m[12] * m[1] * m[11] +
//              m[12] * m[3] * m[9];

//    inv[13] = m[0]  * m[9] * m[14] -
//              m[0]  * m[10] * m[13] -
//              m[8]  * m[1] * m[14] +
//              m[8]  * m[2] * m[13] +
//              m[12] * m[1] * m[10] -
//              m[12] * m[2] * m[9];

//    inv[2] = m[1]  * m[6] * m[15] -
//             m[1]  * m[7] * m[14] -
//             m[5]  * m[2] * m[15] +
//             m[5]  * m[3] * m[14] +
//             m[13] * m[2] * m[7] -
//             m[13] * m[3] * m[6];

//    inv[6] = -m[0]  * m[6] * m[15] +
//              m[0]  * m[7] * m[14] +
//              m[4]  * m[2] * m[15] -
//              m[4]  * m[3] * m[14] -
//              m[12] * m[2] * m[7] +
//              m[12] * m[3] * m[6];

//    inv[10] = m[0]  * m[5] * m[15] -
//              m[0]  * m[7] * m[13] -
//              m[4]  * m[1] * m[15] +
//              m[4]  * m[3] * m[13] +
//              m[12] * m[1] * m[7] -
//              m[12] * m[3] * m[5];

//    inv[14] = -m[0]  * m[5] * m[14] +
//               m[0]  * m[6] * m[13] +
//               m[4]  * m[1] * m[14] -
//               m[4]  * m[2] * m[13] -
//               m[12] * m[1] * m[6] +
//               m[12] * m[2] * m[5];

//    inv[3] = -m[1] * m[6] * m[11] +
//              m[1] * m[7] * m[10] +
//              m[5] * m[2] * m[11] -
//              m[5] * m[3] * m[10] -
//              m[9] * m[2] * m[7] +
//              m[9] * m[3] * m[6];

//    inv[7] = m[0] * m[6] * m[11] -
//             m[0] * m[7] * m[10] -
//             m[4] * m[2] * m[11] +
//             m[4] * m[3] * m[10] +
//             m[8] * m[2] * m[7] -
//             m[8] * m[3] * m[6];

//    inv[11] = -m[0] * m[5] * m[11] +
//               m[0] * m[7] * m[9] +
//               m[4] * m[1] * m[11] -
//               m[4] * m[3] * m[9] -
//               m[8] * m[1] * m[7] +
//               m[8] * m[3] * m[5];

//    inv[15] = m[0] * m[5] * m[10] -
//              m[0] * m[6] * m[9] -
//              m[4] * m[1] * m[10] +
//              m[4] * m[2] * m[9] +
//              m[8] * m[1] * m[6] -
//              m[8] * m[2] * m[5];

//    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

//    if (det == 0)
//        return false;

//    det = 1.0 / det;

//    for (i = 0; i < 16; i++)
//        invOut[i] = inv[i] * det;

//    return true;
//}


//Mouse ray casting
void GLDisplayWidget::mouseReleaseEvent(QMouseEvent *event)
{


//    if( event == NULL )
//        return;

//    int xMouse = event->x();
//    int yMouse = event->y();

//    printf("xMouse:%d, yMouse:%d",xMouse,yMouse);

//    float x = ( 2.0f * xMouse ) / width - 1.0f;
//    float y = 1.0f - (2.0f * yMouse) / height;
//    vec2 ray_ndc = vec2(x,y);

//    vec4 ray_clip = vec4(ray_ndc, -1.0, 1.0);
//    //vec4 ray_eye = inverse();


//    float mat[16];
//    glhPerspectivef2(mat, 45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
//    float matInv[16];
//    gluInvertMatrix(mat, matInv);

//    vec4 ray_eye = matInv * ray_clip;

//    ray_eye = vec4(ray_eye.xy, -1.f, 0.f);

//    vec3 ray_wor = (inverse(view_matrix) * ray_eye).xyz;
//    // don't forget to normalise the vector at some point
//    ray_wor = normalise(ray_wor);

}

// Mouse movement management
void GLDisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - _lastPosMouse.x();
    //int dy = event->y() - _lastPosMouse.y();

    //printf("dx : %d, dy : %d\n", dx, dy);

    if( event != NULL )
    {
        _angleX += dx;
        //_angleY += dy;
        _lastPosMouse = event->pos();

        updateGL();
    }
}

// Mouse Management for the zoom
void GLDisplayWidget::wheelEvent(QWheelEvent *event) {
    QPoint numDegrees = event->angleDelta();
    double stepZoom = 0.25;
    if (!numDegrees.isNull())
    {
      _Z = (numDegrees.x() > 0 || numDegrees.y() > 0) ? _Z + stepZoom : _Z - stepZoom;
    }
}
