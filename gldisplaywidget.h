#ifndef GLDISPLAYWIDGET_H
#define GLDISPLAYWIDGET_H

#include <QGLWidget>
#include <QtWidgets>
#include <QTimer>
#include "mesh.h"

class GLDisplayWidget : public QGLWidget
{
public:
    explicit GLDisplayWidget(QWidget *parent = 0);

    void initializeGL(); // The scene may be initialized in this function since the mesh is a data member...
    void paintGL(); // Display the scene Gl
    void resizeGL(int width, int height);

    void setWireframe();
    bool wf() { return wireframe; }

    unsigned int vertexNb() { return _mesh.vertexNb(); }
    unsigned int faceNb() { return _mesh.faceNb(); }
    void setVertexStart(unsigned int vs) { _mesh.setVertexStart(vs); }
    void nextFace() { _mesh.nextFace(+1); }
    void prevFace() { _mesh.nextFace(-1); }
    void nextVertex() { _mesh.nextVertex(+1); }
    void prevVertex() { _mesh.nextVertex(-1); }
    int currentFace() { return _mesh.currFace(); }
    int currentStartVertex() { return _mesh.currStartVertex(); }
    int currentVertex() { return _mesh.currVertex(); }
    void resetVertexFaceIndex() { _mesh.resetVertexFaceIndex(); }
    
    void testIterators();


protected:
    // Mouse Management
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:

    QTimer _timer; // To update the scene
    float _X, _Y ,_Z; // Translation
    float _angleX, _angleY; // Rotation

    bool wireframe = false;

    QPoint _lastPosMouse; // To keep the last position of the mouse

    Mesh _mesh; // The object to be displayed, may be replaced by a scene if there are several objects....
};

#endif // GLDISPLAYWIDGET_H
