#include "gldisplaywidget.h"
#ifdef __APPLE__
    #include <glu.h>
#else
    #include <GL/glu.h>
#endif

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
    gluLookAt(0,0,5,  0,0,0,   0,1,0);

    // Translation
    glTranslated(_X, _Y, _Z);

    // Rotation
    glRotatef(_angleX, _angleY, 1.0f, 0.0f);

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
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

    updateGL();
}

void GLDisplayWidget::testIterators() {

    Point p;
    int i = 0;
    for (Vertices_iterator vi = _mesh.vertices_iterator_begin(); vi != _mesh.vertices_iterator_end(); vi++) {
        
        p = (*vi).getPoint();

        printf("Sommet %d : x=%f, y=%f, z=%f\n", i, p.x, p.y, p.z);
        i++;

        Faces_circulator fcBegin = _mesh.incident_faces_circulator(*vi);
        Faces_circulator fc;
        int cp = 0;
        for (fc = fcBegin, fc++; cp++, fc != fcBegin; fc++)
            ;
        printf("Compteur = %d\n", cp);
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

//Mouse ray casting
void GLDisplayWidget::mouseReleaseEvent(QMouseEvent *event)
{

    /*
    if( event == NULL )
        return;

    int xMouse = event->x();
    int yMouse = event->y();

    printf("xMouse:%d, yMouse:%d",xMouse,yMouse);

    float x = ( 2.0f * xMouse ) / width - 1.0f;
    float y = 1.0f - (2.0f * yMouse) / height;
    vec2 ray_ndc = vec2(x,y);

    vec4 ray_clip = vec4(ray_ndc, -1.0, 1.0);
    vec4 rey_eye = inverse();
    */




}

// Mouse movement management
void GLDisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - _lastPosMouse.x();
    //int dy = event->y() - _lastPosMouse.y();

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
