#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "stdafx.h"

#include "MyPoint.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

private:
    bool    _isMouseDown;
    float   _zoomFactor;
    QPoint  _scrollOffset;

    std::vector<MyPoint> _tempPoints;
    std::vector< std::vector<MyPoint> > _points;

    /* OpenGL 3.3 */
    QOpenGLBuffer _vbo;
    QOpenGLVertexArrayObject _vertexArrayObject;
    QOpenGLTexture* _texture;
    QOpenGLShaderProgram* _shaderProgram;

    int _mvpMatrixLocation;
    int _colorLocation;
    QMatrix4x4 _perspMatrix;
    QMatrix4x4 _transformMatrix;



    // image size (does not depend on QImage)
    int _img_width;
    int _img_height;

    // image
    QImage _imgOriginal;
    //QImage _imgGL;
    //GLuint _imgID;

public:

    // constructor
    GLWidget( QGLFormat format, QWidget *parent = 0);
    // destructor
    ~GLWidget();

    QSize GetCanvasSize() { return QSize(_img_width, _img_height); }

    void SetImage(QString img);

    // save current buffer to image
    void SaveImage(QString filename);

    // zoom in handle
    void ZoomIn();
    // zoom out handle
    void ZoomOut();
    // set zoom value
    void SetZoom(int val){this->_zoomFactor = val;}
    // get zoom value
    float GetZoomFactor() { return this->_zoomFactor; }

    // set horizontal scroll position
    void HorizontalScroll(int val);
    // set vertical scroll position
    void VerticalScroll(int val);
    // get scroll position (horizontal and vertical)
    QPoint GetScrollOffset() {return this->_scrollOffset;}

    // mouse press
    void mousePressEvent(int x, int y);
    // mouse move
    void mouseMoveEvent(int x, int y);
    // mouse release
    void mouseReleaseEvent(int x, int y);
    // mouse double click
    void mouseDoubleClick(int x, int y);

    // reset everything
    void Reset();

protected:
    // qt event
    bool event( QEvent * event );
    // init opengl
    void initializeGL();
    // draw
    void paintGL();

    void resizeGL(int width, int height);

private:
    void SetColor(const QColor& col);
    void DrawLine(MyPoint p1, MyPoint p2);
    void DrawImage();

    QMatrix4x4 GetCameraMatrix();
    void TranslateWorld(float x, float y, float z);
    void RotateWorld(float x, float y, float z);
    void ScaleWorld(float x, float y, float z);

};


#endif // GLWIDGET_H
