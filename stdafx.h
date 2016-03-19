#pragma once

#define GL_GLEXT_PROTOTYPES

// Qt OpenGL
#include <QtOpenGL/QGLWidget>
#include <QGLShader>
#include <QOpenGLShaderProgram>
#include <QGLFormat>
#include <QGLFramebufferObject>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QSvgGenerator>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtGui/QMouseEvent>

// Scrollbar
#include <QAbstractScrollArea>
#include <QScrollArea>
#include <QScrollBar>

#include <QColor>
#include <QFileDialog>
#include <QImage>
#include <QImageReader>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>

#include <QStyle>
#include <QTimer>
#include <QWidget>

#define _USE_MATH_DEFINES

#include <vector>
#include <string>
#include <sstream>
#include <cfloat>
#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>

//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <malloc.h>


// on Windows it doesn't work...
//#include <GL/glu.h>
//#include <GL/glut.h>

#define my_min(a,b) ((a)<(b) ? (a) : (b))
#define my_max(a,b) ((a)>(b) ? (a) : (b))

#ifndef M_EPS
#define M_EPS		1e-8
#endif
