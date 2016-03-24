#ifndef GLWIDGET_H
#define GLWIDGET_H

/*
* Reza Adhitya Saputra
* radhitya@uwaterloo.ca
*
*/

#include "stdafx.h"
#include "MyPoint.h"
#include "MyLine.h"
#include "RejectionSampling.h"
#include "MyIndexedLine.h"
//#include "TSPIO.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

private:
    bool    _isMouseDown;
    float   _zoomFactor;
    QPoint  _scrollOffset;

    std::vector<MyPoint> _tempPoints;
    std::vector< std::vector<MyPoint> > _points;

    QOpenGLTexture* _texture;
    QOpenGLShaderProgram* _shaderProgram;

    // the input image
    QOpenGLBuffer _imageVbo;
    QOpenGLVertexArrayObject _imageVao;

    // centroids
    QOpenGLBuffer _centroidsVbo;
    QOpenGLVertexArrayObject _centroidsVao;

    // cones
    QOpenGLBuffer _conesVbo;
    QOpenGLVertexArrayObject _conesVao;
    //int _coneSlice;
    //int _verticesPerCone; // _coneSlice + 2

    // for rendering
    int _mvpMatrixLocation;
    int _colorLocation;
    QMatrix4x4 _perspMatrix;
    QMatrix4x4 _transformMatrix;

    // image size
    int _img_width;
    int _img_height;

    // original image
    QImage _imgOriginal;
    QImage _imgColor;
    // a buffer for lloyd's method
    QImage _imageBuffer;
    //QImage _imgGL;
    //GLuint _imgID;

    // random sampling
    //int _numSample;
    RejectionSampling* _rSampling;      // an instance that generates random sampling
    std::vector<MyPoint> _centroids ;   // centroids of voronoi cells
    std::vector<float>   _centroidsArea;
    std::vector<MyPoint> _prevCentroids ;
    std::vector<QColor> _centroidColors ;
    float _displacement;
    //std::vector<float> _displacements;
    std::vector<QColor> _coneColors;    // this stores indices of centroids as colors

    std::vector<MyLine> _lines;

    //TSPIO* _tspIo;
    std::vector<int> _tspPath;

    // variables for weighted voronoi diagram
    //std::vector<float> _mArray;     // weighted moment
    //std::vector<float> _cxArray;
    //std::vector<float> _cyArray;

public:

	void OneStepLloydIteration();

    // constructor
    GLWidget( QGLFormat format, QWidget *parent = 0);
    // destructor
    ~GLWidget();

    QSize GetCanvasSize() { return QSize(_img_width, _img_height); }

    void SetImage(QString img);

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

    // render to svg
    void SaveToSvg();

    // render a scene to a buffer,
    // then the buffer is saved as an image
    void SaveToBitmap();

    bool IsCalculationDone(){ return _iterStatus == -1; }

protected:
    // qt event
    bool event( QEvent * event );
    // init opengl
    void initializeGL();
    // draw
    void paintGL();

    void resizeGL(int width, int height);

private:
    QImage LoadImageAsGrayscale(QString img);

    void SetColor(const QColor& col);

    //void LloydIteration();
    void PrepareCentroids();
    void UpdateCentroids();
    void CalculateCones();

    void PaintLine(MyPoint p1, MyPoint p2);
    void PaintImage();
    void PaintPoints();
    void PaintCones();

    void PerformTriangulation();
    void ProcessTSP();
    //void DrawCones(int xInit, int yInit);

    //QMatrix4x4 GetCameraMatrix();
    //void TranslateWorld(float x, float y, float z);
    //void RotateWorld(float x, float y, float z);
    //void ScaleWorld(float x, float y, float z);

    std::vector<float> GetGrayValues();

    void GenerateConeColors();
    int IndexFromColor(QColor col);

    int _iterStatus; // -1 do nothing or stop
                     //  0 init
                     //  1 running
    int _currentIter;
    //QTimer* _iterTimer;
    void InitLloydIteration();
    void EndLloydIteration();
	

private slots:
    void NextLloydIteration();
};


#endif // GLWIDGET_H
