
#include "stdafx.h"
#include "GLWidget.h"
#include "SystemParams.h"


#include <limits>
#include <string>
#include <iomanip>

#define M_PI 3.14159265359
//#define _USE_MATH_DEFINES
//#include <cmath> 

// Uncomment these if you want CGAL
/*
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <fstream>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Projection_traits_xy_3<K>  Gt;
typedef CGAL::Delaunay_triangulation_2<Gt> Delaunay;
typedef K::Point_3   Point;
*/

std::vector<std::string>& my_split_str(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> my_split_str(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    my_split_str(s, delim, elems);
    return elems;
}


// VertexData
struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
    QVector3D color;

public:

    VertexData(QVector3D position, QVector2D texCoord, QVector3D color)
    {
        this->position = position;
        this->texCoord = texCoord;
        this->color = color;
    }

    VertexData(QVector3D position, QVector2D texCoord)
    {
        this->position = position;
        this->texCoord = texCoord;
        this->color = QVector3D();
    }

    VertexData()
    {
        this->position = QVector3D();
        this->texCoord = QVector2D();
        this->color = QVector3D();
    }
};

// GLWidget
GLWidget::GLWidget(QGLFormat format, QWidget *parent) :

    QGLWidget(format, parent),
    _isMouseDown(false),
    _zoomFactor(1.0),

    _texture(0),
    _shaderProgram(0),

    _rSampling(0),

    _iterStatus(-1)

    //_tspIo(0)

    //_numSample(30000),
    //_coneSlice (32),
    //_verticesPerCone (34) // _coneSlice + 2

{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    std::cout << "Qt version >= 5.1.0\n";
#endif

    //_iterTimer = new QTimer(this); connect(_iterTimer, SIGNAL(timeout()), this, SLOT(NextLloydIteration()));
}


GLWidget::~GLWidget()
{
    if(_texture) delete _texture;
    if(_shaderProgram) delete _shaderProgram;
    if(_rSampling) delete _rSampling;
    //if(_tspIo) delete _tspIo;
    //if(_iterTimer) delete _iterTimer;
}


void GLWidget::initializeGL()
{
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers())
        { std::cerr << "Could not enable sample buffers." << std::endl; return; }

    glShadeModel(GL_SMOOTH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glEnable(GL_DEPTH_TEST);

    _shaderProgram = new QOpenGLShaderProgram();
    if (!_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "../WVS/shader.vert"))
        { std::cerr << "Cannot load vertex shader." << std::endl; return; }

    if (!_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "../WVS/shader.frag"))
        { std::cerr << "Cannot load fragment shader." << std::endl; return; }

    if ( !_shaderProgram->link() )
        { std::cerr << "Cannot link shaders." << std::endl; return; }


    _shaderProgram->bind();

    _mvpMatrixLocation = _shaderProgram->uniformLocation("mvpMatrix");
    _colorLocation = _shaderProgram->attributeLocation("vertexColor");

    // sampling
    _rSampling = new RejectionSampling();

    // IO
    //_tspIo = new TSPIO();
}


bool GLWidget::event( QEvent * event )
{
    return QGLWidget::event(event);
}


// This is an override function from Qt but I can't find its purpose
void GLWidget::resizeGL(int width, int height)
{
}

void GLWidget::SetColor(const QColor& col)
{
    _shaderProgram->setAttributeValue(_colorLocation, (float)col.red() / 255.0, (float)col.green() / 255.0, (float)col.blue()  / 255.0);
}

void GLWidget::PaintImage()
{
    if(_img_width == 0 && _img_height == 0)
    {
        return;
    }

    int use_color_location = _shaderProgram->uniformLocation("use_color");
    _shaderProgram->setUniformValue(use_color_location, (GLfloat)0.0);
    _texture->bind();

   _imageVao.bind();
   glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
   _imageVao.bind();

}

void GLWidget::PaintCones()
{
    int verticesPerCone = SystemParams::cone_slices + 2;

    if(_img_width == 0 && _img_height == 0) { return; }

    int use_color_location = _shaderProgram->uniformLocation("use_color");
    _shaderProgram->setUniformValue(use_color_location, (GLfloat)1.0);

    _conesVao.bind();
    for(size_t a = 0; a < _centroids.size(); a++)
    {
        glDrawArrays(GL_TRIANGLE_FAN, a * verticesPerCone, verticesPerCone);
    }
    _conesVao.release();
}

void GLWidget::PaintPoints()
{
    if(_img_width == 0 && _img_height == 0) { return; }

    //glPointSize(5.0f);

    int use_color_location = _shaderProgram->uniformLocation("use_color");
    _shaderProgram->setUniformValue(use_color_location, (GLfloat)1.0);

    _centroidsVao.bind();
    //glDrawArrays(GL_POINTS, 0, _centroids.size());
    for(size_t a = 0; a < _centroids.size(); a++)
    {
        glDrawArrays(GL_TRIANGLE_FAN, a * 18, 18);
    }
    _centroidsVao.release();
}

void GLWidget::PaintLine(MyPoint p1, MyPoint p2)
{
    /*
    int use_color_location = _shaderProgram->uniformLocation("use_color");
    _shaderProgram->setUniformValue(use_color_location, (GLfloat)1.0);
    //SetColor(QColor(255, 0, 0));
    glLineWidth(5.0);

    QVector<VertexData> vertices;
    vertices.append(VertexData(QVector3D(p1.x, p1.y,  0.0f), QVector2D(), QVector3D(1.0, 0.0, 0.0)));
    vertices.append(VertexData(QVector3D(p2.x, p2.y,  0.0f), QVector2D(), QVector3D(1.0, 0.0, 0.0)));

    _vbo.create();
    _vbo.bind();
    _vbo.allocate(vertices.constData(), 2 * sizeof(VertexData));

    // Offset for position
   quintptr offset = 0;

   // Tell OpenGL programmable pipeline how to locate vertex position data
   int vertexLocation = _shaderProgram->attributeLocation("vert");
   _shaderProgram->enableAttributeArray(vertexLocation);
   _shaderProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

   offset += sizeof(QVector3D);
   offset += sizeof(QVector2D);

   _shaderProgram->enableAttributeArray(_colorLocation);
   _shaderProgram->setAttributeBuffer(_colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

   glDrawArrays(GL_LINES, 0, 2);
   */
}

void GLWidget::ProcessTSP()
{
	/*
    // create a .tsp
    std::cout << "TSP\n";
    std::cout << "# points: " << _centroids.size() << "\n";

    using namespace std;
    ofstream f;
    f.open ("nux.tsp");


    f << "NAME : nux\n";
    f << "COMMENT : what a day, what a lovely day\n";
    f << "TYPE : TSP\n";
    f << "DIMENSION : " << _centroids.size() << "\n";
    f << "EDGE_WEIGHT_TYPE : EUC_2D\n";
    f << "NODE_COORD_SECTION\n";


    for(size_t a = 0; a < _centroids.size(); a++)
    {
        f << (a + 1) << " "
          << std::setprecision(20) << _centroids[a].x << " "
          << std::setprecision(20) << _centroids[a].y << "\n";
    }

    f << "EOF\n";

    f.close();

    // hack
    system("./concorde -V nux.tsp");
    std::cout << "TSP DONE !!!\n";
    //system("chmod 777 nux.sol");

    // read

    std::ifstream myfile ("nux.sol");
    size_t curLine = 0;
    _tspPath.clear();
    while(!myfile.eof())
    {

        std::string line;
        std::getline (myfile,line);


        if(curLine == 0)
        {
            curLine++;
            continue;
        }


        if(line.size() == 0) {continue;}

        std::vector<std::string> arrayStr = my_split_str(line, ' ');

        for(size_t a = 0; a < arrayStr.size(); a++)
        {
            int idx = std::stoi(arrayStr[a]);
            _tspPath.push_back(idx);
        }
    }
    myfile.close();
	*/
}

void GLWidget::paintGL()
{
    if(_iterStatus == 0)
    {
        InitLloydIteration();
        _currentIter = 0;
        _iterStatus = 1;
    }

    if(_iterStatus == 1)
    {
        NextLloydIteration();
        _currentIter++;

        if(_currentIter >= SystemParams::max_iter || _displacement < std::numeric_limits<int>::epsilon())
        {
            _iterStatus = -1;
            PerformTriangulation();
            ProcessTSP();
            //EndLloydIteration();
        }
        else
        {
            std::cout << "iteration " << _currentIter << ", displacement " << _displacement << "\n";
        }

    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, this->width(),  this->height());

    int current_width = width();
    int current_height = height();

    // Set orthographic Matrix
    QMatrix4x4 orthoMatrix;


    orthoMatrix.ortho(0.0 +  _scrollOffset.x(),
                      (float)current_width +  _scrollOffset.x(),
                      (float)current_height + _scrollOffset.y(),
                      0.0 + _scrollOffset.y(),
                      -100, 100);

    // Translate the view to the middle
    QMatrix4x4 transformMatrix;
    transformMatrix.setToIdentity();
    transformMatrix.scale(_zoomFactor);

    _shaderProgram->setUniformValue(_mvpMatrixLocation, orthoMatrix * transformMatrix);

    /*
    if(_points.size() > 0)
    {
        for(size_t a = 0; a < _points.size(); a++)
        {
            for(size_t b = 0; b < _points[a].size() - 1; b++)
                { PaintLine(_points[a][b], _points[a][b+1]); }
        }

    }

    if(_tempPoints.size() > 2)
    {
        for(size_t a = 0; a < _tempPoints.size() -1 ; a++)
            { PaintLine(_tempPoints[a], _tempPoints[a+1]); }
    }
    */

    //PaintCones();

    PaintPoints();

    // draw the input image
    //PaintImage();

    // draw initial points

    //if(_iterStatus == 1)
    //{
    //    this->repaint();
    //}

}

std::vector<float> GLWidget::GetGrayValues()
{
    std::vector<float> grayValues;
    for(size_t a = 0; a < _img_height; a++)
    {
        for(size_t b = 0; b < _img_width; b++)
        {
            float val = qGray(_imgOriginal.pixel(b, a));
            grayValues.push_back(1.0 - (val / 255.0 * 0.9));
        }
    }
    return grayValues;
}

// Mouse is pressed
void GLWidget::mousePressEvent(int x, int y)
{
    _isMouseDown = true;

    double dx = x + _scrollOffset.x();
    dx /= _zoomFactor;

    double dy = y + _scrollOffset.y();
    dy /= _zoomFactor;

    _tempPoints.push_back(MyPoint(dx, dy));

    QColor col = QColor(_imageBuffer.pixel(dx, dy));

    std::cout << "alpha: " << col.alpha() << "\n";

    //int idx = IndexFromColor(col);
    //std::cout << idx << "\n";

    std::cout << col.red() << " " << col.green() << " " << col.blue() << " " << "\n";

    this->repaint();
}


// Mouse is moved
void GLWidget::mouseMoveEvent(int x, int y)
{
    double dx = x + _scrollOffset.x();
    dx /= _zoomFactor;

    double dy = y + _scrollOffset.y();
    dy /= _zoomFactor;

    if(_isMouseDown)
    {
        _tempPoints.push_back(MyPoint(dx, dy));
        this->repaint();
    }
}


// Mouse is released
void GLWidget::mouseReleaseEvent(int x, int y)
{
    _isMouseDown = false;
    double dx = x + _scrollOffset.x();
    dx /= _zoomFactor;

    double dy = y + _scrollOffset.y();
    dy /= _zoomFactor;

    if(_tempPoints.size() >= 2)
    {
        _tempPoints.push_back(MyPoint(dx, dy));
        std::vector<MyPoint> tempPoints2;
        for(size_t a = 0; a < _tempPoints.size(); a++)
        {
            tempPoints2.push_back(_tempPoints[a]);
        }
        _points.push_back(tempPoints2);
        _tempPoints.clear();
    }
    this->repaint();
}

void GLWidget::mouseDoubleClick(int x, int y)
{
    double dx = x + _scrollOffset.x();
    dx /= _zoomFactor;

    double dy = y + _scrollOffset.y();
    dy /= _zoomFactor;
}

void GLWidget::SetImage(QString img)
{
    // fixme, need a square image
    this->Reset();
    //_imgOriginal.load(img);
    _imgColor.load(img);
    _imgOriginal = LoadImageAsGrayscale(img);

    // size
    this->_img_width = _imgOriginal.width();
    this->_img_height = _imgOriginal.height();

    std::cout << "image loaded. width: " << _img_width << ", height: " << _img_height << "\n";

    _texture = new QOpenGLTexture(_imgOriginal);
    _texture->setMinificationFilter(QOpenGLTexture::Nearest);
    _texture->setMagnificationFilter(QOpenGLTexture::Linear);

    _shaderProgram->setAttributeValue("base_texture", _texture->textureId());

    // ~~~ create vao for the input image ~~~
    _imageVao.create();
    _imageVao.bind();

    QVector<VertexData> imageVertices;
    imageVertices.append(VertexData(QVector3D(0.0,        0.0,          0.0f), QVector2D(0, 0)));
    imageVertices.append(VertexData(QVector3D(_img_width, 0.0,          0.0f), QVector2D(1, 0)));
    imageVertices.append(VertexData(QVector3D(_img_width, _img_height,  0.0f), QVector2D(1, 1)));
    imageVertices.append(VertexData(QVector3D(0.0,        _img_height,  0.0f), QVector2D(0, 1)));

    _imageVbo.create();
    _imageVbo.bind();
    _imageVbo.allocate(imageVertices.data(), 4 * sizeof(VertexData));

    // Offset for position
    quintptr offset = 0;

    // vertex
    int vertexLocation = _shaderProgram->attributeLocation("vert");
    _shaderProgram->enableAttributeArray(vertexLocation);
    _shaderProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    // uv
    int texcoordLocation = _shaderProgram->attributeLocation("uv");
    _shaderProgram->enableAttributeArray(texcoordLocation);
    _shaderProgram->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    _imageVao.release();

    /*
    // calculate random sampling
    _centroids.clear();
    _centroids = _rSampling->GeneratePoints(GetGrayValues(), _numSample, _img_width, _img_height);
    std::cout << "# centroid is " << _centroids.size() << "\n";
    */

    // ~~~ generate index colors ~~~
    //GenerateConeColors();

    // ~~~ create vao for the points ~~~
    // fixme: it's static


    // ~~~ create vao for the cones ~~~
    // fixme: it's static


    // draw buffer
    //SaveToBitmap();

    //LloydIteration();
    //InitLloydIteration();
    this->_iterStatus = 0;
}


/*
void GLWidget::LloydIteration()
{
    // calculate random sampling
    _centroids.clear();
    _centroids = _rSampling->GeneratePoints(GetGrayValues(), SystemParams::num_stipples, _img_width, _img_height);
    std::cout << "# centroid is " << _centroids.size() << "\n";

    //fixme: rounding error?
    for(size_t a = 0; a < _centroids.size(); a++)
    {
        _centroids[a].x += 1.0;
        _centroids[a].y += 1.0;
    }

    // generate index colors
    GenerateConeColors();

    size_t numIter = 500;

    for(size_t iter = 0; iter < numIter; iter++)
    {
        CalculateCones();
        SaveToBitmap();
        UpdateCentroids();

        //this->repaint();
    }

    PrepareCentroids();
}
*/

void GLWidget::InitLloydIteration()
{
    // calculate random sampling
    _centroids.clear();
    _centroids = _rSampling->GeneratePoints(GetGrayValues(), SystemParams::num_stipples, _img_width, _img_height);
    _prevCentroids = std::vector<MyPoint>(_centroids.size());
    _centroidsArea = std::vector<float>(_centroids.size());
    _centroidColors = std::vector<QColor>(_centroids.size());
    std::cout << "# centroid is " << _centroids.size() << "\n";

    //fixme: rounding error?
    for(size_t a = 0; a < _centroids.size(); a++)
    {
        _centroids[a].x += 1.0;
        _centroids[a].y += 1.0;

        _centroidsArea[a] = 0;
        _centroidColors[a] = QColor(0, 0, 0);
        //_displacements[a] = 0;
    }

    for(size_t a = 0; a < _centroids.size(); a++)
    {
        _prevCentroids[a].x += _centroids[a].x;
        _prevCentroids[a].y += _centroids[a].y;
    }

    _displacement = std::numeric_limits<float>::max();

    // generate index colors
    GenerateConeColors();

    //_iterTimer->setInterval(1);
    //_currentIter = 0;
    //_iterTimer->startTimer(1);
}

void GLWidget::NextLloydIteration()
{
    CalculateCones();
    SaveToBitmap();
    UpdateCentroids();
    PrepareCentroids();
    //_currentIter++;
    //if(_currentIter >= SystemParams::max_iter)
    //{
        //_iterTimer->stop();
    //    EndLloydIteration();
    //}
}



void GLWidget::EndLloydIteration()
{
    PrepareCentroids();
}

void GLWidget::PerformTriangulation()
{
	/*
    std::vector <Point> points;
    for(size_t a = 0; a < _centroids.size(); a++)
    {
        points.push_back(Point(_centroids[a].x, _centroids[a].y, 0));
    }

    Delaunay dt(points.begin(), points.end());
    _lines.clear();
    Delaunay::Finite_edges_iterator fiter = dt.finite_edges_begin();
    for(; fiter != dt.finite_edges_end(); fiter++)
    {
        Delaunay::Segment seg = dt.segment( *fiter );
        Delaunay::Point p0 = seg.point(0);
        Delaunay::Point p1 = seg.point(1);

        _lines.push_back(MyLine(p0.x(), p0.y(), p1.x(), p1.y()));
    }
	*/
}


void GLWidget::UpdateCentroids()
{
    // variables for weighted voronoi diagram
    std::vector<float> mArray(_centroids.size());     // weighted moment
    std::vector<float> areaArray(_centroids.size());
    std::vector<float> cxArray(_centroids.size());
    std::vector<float> cyArray(_centroids.size());

    std::vector<float> rArray(_centroids.size());
    std::vector<float> gArray(_centroids.size());
    std::vector<float> bArray(_centroids.size());

    for(size_t a = 0; a < _centroids.size(); a++)
    {
        areaArray[a] = 0;
        mArray[a] = 0;
        cxArray[a] = 0;
        cyArray[a] = 0;

        rArray[a] = 0;
        gArray[a] = 0;
        bArray[a] = 0;
    }

    // step 1 - Iterate over the entire image to calculate _mArray, _cxArray, and _cyArray
    for(size_t a = 0; a < _imgOriginal.height(); a++)
    {
        for(size_t b = 0; b < _imgOriginal.width(); b++)
        {
            QColor col = QColor(_imageBuffer.pixel(b, a));
            int idx = IndexFromColor(col);
            if(idx >= _centroids.size())
            {
                continue;
            }

            QColor rgbColor = QColor(_imgColor.pixel(b, a));
            rArray[idx] += rgbColor.red();
            gArray[idx] += rgbColor.green();
            bArray[idx] += rgbColor.blue();

            float val = qGray(_imgOriginal.pixel(b, a));
            val /= 255.0;
            val = 1.0 - val;

            areaArray[idx]++;
            mArray[idx] += val;
            cxArray[idx] += val * (b + 1.0);
            cyArray[idx] += val * (a + 1.0);
        }
    }

    // step 2 - Update Centroids
    for(size_t a = 0; a < _centroids.size(); a++)
    {        
        if(mArray[a] > std::numeric_limits<float>::epsilon())
        {
        // fixme: rounding error
        _centroids[a].x = (cxArray[a] / mArray[a]);
        _centroids[a].y = (cyArray[a] / mArray[a]);

        _centroidsArea[a] = mArray[a];

        _centroidColors[a] = QColor(rArray[a] / areaArray[a],
                                    gArray[a] / areaArray[a],
                                    bArray[a] / areaArray[a]);

        }
    }

    float sumDisplacement = 0.0;
    for(size_t a = 0; a < _centroids.size(); a++)
    {
        sumDisplacement += _centroids[a].Distance(_prevCentroids[a]);
    }
    _displacement = sumDisplacement / (float)_centroids.size();

    for(size_t a = 0; a < _centroids.size(); a++)
    {
        _prevCentroids[a].x += _centroids[a].x;
        _prevCentroids[a].y += _centroids[a].y;
    }

}

void GLWidget::PrepareCentroids()
{
    if(_centroidsVao.isCreated())
    {
        _centroidsVao.destroy();
    }
    _centroidsVao.create();
    _centroidsVao.bind();

    QVector<VertexData> pointsVertices;

    for(size_t a = 0; a < _centroids.size(); a++)
    {
        int xCenter = _centroids[a].x;
        int yCenter = _centroids[a].y;
        //float radius = 1;
        //float radius = _centroidsArea[a] * 0.2 / 2.0;
        float radius = 0.75 * sqrt(_centroidsArea[a] / M_PI);

        QColor col(0, 0, 0);
        //QColor col = _centroidColors[a];
        QVector3D vecCol = QVector3D((float)col.red() / 255.0, (float)col.green() / 255.0, (float)col.blue() / 255.0);


        pointsVertices.append(VertexData(QVector3D(xCenter, yCenter,  0.0f), QVector2D(), vecCol));
        float addValue = (M_PI * 2.0 / 16);
        for(float a = 0.0; a < M_PI * 2.0; a += addValue)
        {
            float xPt = xCenter + radius * sin(a);
            float yPt = yCenter + radius * cos(a);
            pointsVertices.append(VertexData(QVector3D(xPt, yPt,  0.0f), QVector2D(), vecCol));
        }
        float xPt = xCenter + radius * sin(M_PI * 2.0);
        float yPt = yCenter + radius * cos(M_PI * 2.0);
        pointsVertices.append(VertexData(QVector3D(xPt, yPt,  0.0f), QVector2D(), vecCol));
    }

    _centroidsVbo.create();
    _centroidsVbo.bind();
    _centroidsVbo.allocate(pointsVertices.data(), pointsVertices.size() * sizeof(VertexData));

    // reuse the variable
    quintptr offset = 0;

    int vertexLocation = _shaderProgram->attributeLocation("vert");
    _shaderProgram->enableAttributeArray(vertexLocation);
    _shaderProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);
    offset += sizeof(QVector2D);

    _shaderProgram->enableAttributeArray(_colorLocation);
    _shaderProgram->setAttributeBuffer(_colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    _centroidsVao.release();
}

void GLWidget::CalculateCones()
{
    if(_conesVao.isCreated())
    {
        _conesVao.destroy();
    }

    _conesVao.create();
    _conesVao.bind();

    QVector<VertexData> conesVertices;
    for(size_t iter = 0; iter < _centroids.size(); iter++)
    {

        int xCenter = _centroids[iter].x;
        int yCenter = _centroids[iter].y;
        float radius = 50;

        QColor col = _coneColors[iter];
        QVector3D vecCol = QVector3D((float)col.red() / 255.0, (float)col.green() / 255.0, (float)col.blue() / 255.0);

        conesVertices.append(VertexData(QVector3D(xCenter, yCenter,  10.0f), QVector2D(), vecCol));
        float addValue = (M_PI * 2.0 / (float)SystemParams::cone_slices);
        for(float a = 0.0; a < M_PI * 2.0; a += addValue)
        {
            float xPt = xCenter + radius * sin(a);
            float yPt = yCenter + radius * cos(a);
            conesVertices.append(VertexData(QVector3D(xPt, yPt,  -10), QVector2D(), vecCol));
        }

        float xPt = xCenter + radius * sin(M_PI * 2.0);
        float yPt = yCenter + radius * cos(M_PI * 2.0);
        conesVertices.append(VertexData(QVector3D(xPt, yPt,  -10), QVector2D(), vecCol));
    }

    _conesVbo.create();
    _conesVbo.bind();
    _conesVbo.allocate(conesVertices.data(), conesVertices.size() * sizeof(VertexData));

    // reset offset
    quintptr offset = 0;

    int vertexLocation = _shaderProgram->attributeLocation("vert");
    _shaderProgram->enableAttributeArray(vertexLocation);
    _shaderProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);
    offset += sizeof(QVector2D);

    _shaderProgram->enableAttributeArray(_colorLocation);
    _shaderProgram->setAttributeBuffer(_colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    _conesVao.release();
}

QImage GLWidget::LoadImageAsGrayscale(QString img)
{
    QImage oriImage;
    oriImage.load(img);
    for (int i = 0; i < oriImage.height(); i++)
    {
        uchar* scan = oriImage.scanLine(i);
        int depth = 4;
        for (int jj = 0; jj < oriImage.width(); jj++) {

            QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + jj * depth);
            int gray = qGray(*rgbpixel);
            *rgbpixel = QColor(gray, gray, gray).rgba();
        }
    }
    return oriImage;
}

// Save image to file
void GLWidget::SaveImage(QString filename)
{
    // Save onscreen
    paintGL();
    glFlush();
    QImage image = this->grabFrameBuffer();
    if( !image.save( filename) ) std::cout << "Error saving image\n";
}

void GLWidget::HorizontalScroll(int val) { _scrollOffset.setX(val); }
void GLWidget::VerticalScroll(int val) { _scrollOffset.setY(val); }
void GLWidget::ZoomIn() { this->_zoomFactor += 0.05f; }
void GLWidget::ZoomOut() { this->_zoomFactor -= 0.05f; if(this->_zoomFactor < 0.1f) _zoomFactor = 0.1f; }

void GLWidget::Reset()
{
}

void GLWidget::SaveToBitmap()
{
    // fixme: transparency doesn't work
    glClearColor( 1.0, 1.0, 1.0, 0.0 );

    QGLFramebufferObjectFormat fmt;
    fmt.setAttachment(QGLFramebufferObject::Depth);
    fmt.setInternalTextureFormat(GL_RGBA8);
    QGLFramebufferObject fBuffer(_img_width, _img_height, fmt);

    // bind frame buffer
    fBuffer.bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // calculate a projection which conforms the dimension of the input image
    glViewport(0, 0, _img_width,  _img_height);
    QMatrix4x4 orthoMatrix;
    orthoMatrix.ortho(0.0, (float)_img_width, (float)_img_height, 0.0, -100, 100);
    QMatrix4x4 transformMatrix;
    transformMatrix.setToIdentity();
    _shaderProgram->setUniformValue(_mvpMatrixLocation, orthoMatrix * transformMatrix);

    PaintCones();
    //PaintPoints();

    // unbind frame buffer
    fBuffer.release();

    QImage grabbedImage = fBuffer.toImage();
    //grabbedImage.save("image.png");
    _imageBuffer = grabbedImage;

    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

void GLWidget::SaveToSvg()
{
    std::cout << "void GLWidget::SaveToSvg()\n";

    QSvgGenerator generator;
    generator.setFileName("image.svg");
    generator.setSize(QSize(_img_width, _img_height));
    generator.setViewBox(QRect(0, 0, _img_width, _img_height));
    generator.setTitle(tr("SVG Generator Example Drawing"));
    generator.setDescription(tr("An SVG drawing created by the SVG Generator "
                                 "Example provided with Qt."));

    QPainter painter;
    painter.begin(&generator);

    // draw
    painter.setClipRect(QRect(0, 0, _img_width, _img_height));
    /*
    painter.setPen(Qt::NoPen);

    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));

    for(size_t a = 0; a < _centroids.size(); a++)
    {
        QColor col = _centroidColors[a];


        float radius = 0.75 * sqrt(_centroidsArea[a] / M_PI);
        float diameter = radius * 2;
        int xPt = _centroids[a].x;
        int yPt = _centroids[a].y;
        painter.drawEllipse(xPt - radius, yPt - radius, diameter, diameter);
    }*/


    // triangle art
    /*painter.setPen(QPen(Qt::black, 0.5));
    for(size_t a = 0; a < _lines.size(); a++)
    {
        MyLine aLine = _lines[a];
        painter.drawLine(aLine.XA, aLine.YA, aLine.XB, aLine.YB);
    }
    */

    painter.setPen(QPen(Qt::black, 1.0));
    std::cout << "tsp size: " << _tspPath.size() << "\n";
    for(size_t a = 0; a < _tspPath.size(); a++)
    {
        MyPoint startPt = _centroids[_tspPath[a]];
        MyPoint endPt;

        if(a == _tspPath.size() - 1)
        {
            endPt = _centroids[_tspPath[0]];
        }
        else
        {
            endPt = _centroids[_tspPath[a + 1]];
        }
        painter.drawLine(startPt.x, startPt.y, endPt.x, endPt.y);
    }

    painter.end();
}


void GLWidget::GenerateConeColors()
{
    // generate random color
    std::vector<int> rVector(255);
    std::vector<int> gVector(255);
    std::vector<int> bVector(255);

    for(size_t a = 0; a < 255; a++)
    {
        rVector[a] = a;
        gVector[a] = a;
        bVector[a] = a;
    }

    /*

    0...255 RGB Colors
    0..._numSamples indices

    how to retrieve an index:
        index = (red * 65536) + (green * 256) + (blue);
    */

    _coneColors.clear();
    for(size_t a = 0; a < 256 && _coneColors.size() < _centroids.size(); a++)
    {
        for(size_t b = 0; b < 256 && _coneColors.size() < _centroids.size(); b++)
        {
            for(size_t c = 0; c < 256 && _coneColors.size() < _centroids.size(); c++)
            {
                _coneColors.push_back(QColor(rVector[a], gVector[b], bVector[c]));
            }
        }
    }
}

int GLWidget::IndexFromColor(QColor col)
{
    return (col.red() * 65536) + (col.green() * 256) + (col.blue());
}
