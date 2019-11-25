#-------------------------------------------------
#
# Project created by QtCreator 2018-08-28T10:55:17
#
#-------------------------------------------------

QT       += core gui opengl

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mesh_Computational_Geometry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gldisplaywidget.cpp \
    mesh.cpp \
    mesh_iterators.cpp \
    vec.cpp \
    laplacianCalc.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    gldisplaywidget.h \
    mesh.h \
    mesh_iterators.h \
    laplacianCalc.h \
    vec.h \
    utils.h

FORMS    += mainwindow.ui

#---- Comment the following line on MacOS
LIBS = -lGLU

