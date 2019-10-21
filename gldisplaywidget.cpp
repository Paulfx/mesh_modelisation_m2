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

    if (voronoi) _mesh.drawVoronoiDiagram();
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

void GLDisplayWidget::setVoronoi() {
    voronoi = !voronoi;
}



// - - - - - - - - - - - - Mouse Management  - - - - - - - - - - - - - - - -
// When you click, the position of your mouse is saved
void GLDisplayWidget::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL )
        _lastPosMouse = event->pos();
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
