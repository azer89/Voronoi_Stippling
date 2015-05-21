
#include "stdafx.h"
#include "GLWidget.h"

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
    //_vertexArrayObject(new QOpenGLVertexArrayObject( this )),
    _texture(0),
    _shaderProgram(0),
    _rSampling(0),

    _numSample(10),
    _coneSlice (32),
    _verticesPerCone (34) // _coneSlice + 2

{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    std::cout << "Qt version >= 5.1.0\n";
#endif
}


GLWidget::~GLWidget()
{
    if(_texture) delete _texture;
    if(_shaderProgram) delete _shaderProgram;
    if(_rSampling) delete _rSampling;
    //if(_svgPainter) delete _svgPainter;
}


void GLWidget::initializeGL()
{
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers())
        { std::cerr << "Could not enable sample buffers." << std::endl; return; }

    glShadeModel(GL_SMOOTH);
    glClearColor( 0.4, 0.4, 0.4, 0.0 );
    glEnable(GL_DEPTH_TEST);

    _shaderProgram = new QOpenGLShaderProgram();
    if (!_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "../WVS/shader.vert"))
        { std::cerr << "Cannot load vertex shader." << std::endl; return; }

    if (!_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "../WVS/shader.frag"))
        { std::cerr << "Cannot load fragment shader." << std::endl; return; }

    if ( !_shaderProgram->link() )
        { std::cerr << "Cannot link shaders." << std::endl; return; }

    //_vertexArrayObject.create();
    //_vertexArrayObject.bind();
    //_vbo.create();
    //_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    //if (!_vbo.bind())
    //{
    //    std::cerr << "could not bind vertex buffer to the context." << std::endl;
    //    return;
    //}

    _shaderProgram->bind();

    _mvpMatrixLocation = _shaderProgram->uniformLocation("mvpMatrix");
    _colorLocation = _shaderProgram->attributeLocation("vertexColor");

    // sampling
    _rSampling = new RejectionSampling();
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

    /*
    QVector<VertexData> vertices;
    vertices.append(VertexData(QVector3D(0.0,        0.0,          0.0f), QVector2D(0, 0)));
    vertices.append(VertexData(QVector3D(_img_width, 0.0,          0.0f), QVector2D(1, 0)));
    vertices.append(VertexData(QVector3D(_img_width, _img_height,  0.0f), QVector2D(1, 1)));
    vertices.append(VertexData(QVector3D(0.0,        _img_height,  0.0f), QVector2D(0, 1)));

    _vbo.create();
    _vbo.bind();
    _vbo.allocate(vertices.constData(), 4 * sizeof(VertexData));

    // Offset for position
   quintptr offset = 0;

   // Tell OpenGL programmable pipeline how to locate vertex position data
   int vertexLocation = _shaderProgram->attributeLocation("vert");
   _shaderProgram->enableAttributeArray(vertexLocation);
   _shaderProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

   offset += sizeof(QVector3D);

   // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
   int texcoordLocation = _shaderProgram->attributeLocation("uv");
   _shaderProgram->enableAttributeArray(texcoordLocation);
   _shaderProgram->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
   */

   _imageVao.bind();
   glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
   _imageVao.bind();

}

void GLWidget::PaintCones()
{
    if(_img_width == 0 && _img_height == 0)
    {
        return;
    }

    int use_color_location = _shaderProgram->uniformLocation("use_color");
    _shaderProgram->setUniformValue(use_color_location, (GLfloat)1.0);

    /*
    for(size_t iter = 0; iter < _initialPoints.size(); iter++)
    {
        QVector<VertexData> vertices;

        int xCenter = _initialPoints[iter].x;
        int yCenter = _initialPoints[iter].y;
        float radius = 50;
        int slice = 16;

        QColor col = _coneColors[iter];
        QVector3D vecCol = QVector3D((float)col.red() / 255.0, (float)col.green() / 255.0, (float)col.blue() / 255.0);

        vertices.append(VertexData(QVector3D(xCenter, yCenter,  10.0f), QVector2D(), vecCol));
        for(float a = 0.0; a < M_PI * 2.0; a += (M_PI * 2.0 / (float)slice))
        {
            float xPt = xCenter + radius * sin(a);
            float yPt = yCenter + radius * cos(a);
            vertices.append(VertexData(QVector3D(xPt, yPt,  -10), QVector2D(), vecCol));
        }

        float xPt = xCenter + radius * sin(M_PI * 2.0);
        float yPt = yCenter + radius * cos(M_PI * 2.0);
        vertices.append(VertexData(QVector3D(xPt, yPt,  -10), QVector2D(), vecCol));

        _vbo.create();
        _vbo.bind();
        _vbo.allocate(vertices.constData(), vertices.size() * sizeof(VertexData));

         quintptr offset = 0;

        int vertexLocation = _shaderProgram->attributeLocation("vert");
        _shaderProgram->enableAttributeArray(vertexLocation);
        _shaderProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

        offset += sizeof(QVector3D);
        offset += sizeof(QVector2D);

        _shaderProgram->enableAttributeArray(_colorLocation);
        _shaderProgram->setAttributeBuffer(_colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));


        glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
    }
    */

    //_conesVao.bind();
    //glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
    //_conesVao.release();

    _conesVao.bind();
    for(size_t a = 0; a < _centroids.size(); a++)
    {
        glDrawArrays(GL_TRIANGLE_FAN, a * _verticesPerCone, _verticesPerCone);
    }
    _conesVao.release();
}

void GLWidget::PaintPoints()
{
    if(_img_width == 0 && _img_height == 0)
    {
        return;
    }

    glPointSize(5.0f);

    int use_color_location = _shaderProgram->uniformLocation("use_color");
    _shaderProgram->setUniformValue(use_color_location, (GLfloat)1.0);
    //SetColor(QColor(255, 0, 0));

    /*
    QVector<VertexData> vertices;

    for(size_t a = 0; a < _initialPoints.size(); a++)
    {
        int xPt = _initialPoints[a].x;
        int yPt = _initialPoints[a].y;

        float rCol = rand() % 255;
        float gCol = rand() % 255;
        float bCol = rand() % 255;

        vertices.append(VertexData(QVector3D(xPt, yPt,  15.0f), QVector2D(), QVector3D(rCol / 255.0, gCol / 255.0, bCol / 255.0)));
    }

    _vbo.create();
    _vbo.bind();
    _vbo.allocate(vertices.constData(), _initialPoints.size() * sizeof(VertexData));

    quintptr offset = 0;

   int vertexLocation = _shaderProgram->attributeLocation("vert");
   _shaderProgram->enableAttributeArray(vertexLocation);
   _shaderProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(VertexData));

   offset += sizeof(QVector3D);
   offset += sizeof(QVector2D);

   _shaderProgram->enableAttributeArray(_colorLocation);
   _shaderProgram->setAttributeBuffer(_colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));
   */
    _pointsVao.bind();
    glDrawArrays(GL_POINTS, 0, _centroids.size());
    _pointsVao.release();
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

void GLWidget::paintGL()
{
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

    PaintCones();

    //PaintPoints();

    // draw the input image
    //PaintImage();

    // draw initial points

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
    int idx = IndexFromColor(col);
    std::cout << idx << "\n";
    //std::cout << col.red() << " " << col.green() << " " << col.blue() << " " << "\n";

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

void GLWidget::
SetImage(QString img)
{
    // fixme, need a square image
    this->Reset();
    //_imgOriginal.load(img);
    _imgOriginal = LoadImageAsGrayscale(img);

    // size
    this->_img_width = _imgOriginal.width();
    this->_img_height = _imgOriginal.height();

    std::cout << "image loaded. width: " << _img_width << ", height: " << _img_height << "\n";

    _texture = new QOpenGLTexture(_imgOriginal);
    _texture->setMinificationFilter(QOpenGLTexture::Nearest);
    _texture->setMagnificationFilter(QOpenGLTexture::Linear);

    _shaderProgram->setAttributeValue("base_texture", _texture->textureId());

    // calculate random sampling
    _centroids.clear();
    _centroids = _rSampling->GeneratePoints(GetGrayValues(), _numSample, _img_width, _img_height);
    std::cout << "# centroid is " << _centroids.size() << "\n";

    //for(size_t a = 0; a < _initialPoints.size(); a++)
    //{
    //    std:: cout << "(" << _initialPoints[a].x << ", " << _initialPoints[a].y << ")\n";
    //}

    // ~~~ generate index colors ~~~
    GenerateConeColors();

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

    // ~~~ create vao for the points ~~~
    // fixme: it's static
    _pointsVao.create();
    _pointsVao.bind();

    QVector<VertexData> pointsVertices;

    for(size_t a = 0; a < _centroids.size(); a++)
    {
        int xPt = _centroids[a].x;
        int yPt = _centroids[a].y;

        float rCol = 0;
        float gCol = 0;
        float bCol = 0;

        pointsVertices.append(VertexData(QVector3D(xPt, yPt,  15.0f), QVector2D(), QVector3D(rCol / 255.0, gCol / 255.0, bCol / 255.0)));
    }

    _pointsVbo.create();
    _pointsVbo.bind();
    _pointsVbo.allocate(pointsVertices.data(), _centroids.size() * sizeof(VertexData));

    // reuse the variable
    offset = 0;

    //int vertexLocation = _shaderProgram->attributeLocation("vert");
    _shaderProgram->enableAttributeArray(vertexLocation);
    _shaderProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);
    offset += sizeof(QVector2D);

    _shaderProgram->enableAttributeArray(_colorLocation);
    _shaderProgram->setAttributeBuffer(_colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    _pointsVao.release();

    // ~~~ create vao for the cones ~~~
    // fixme: it's static
    _conesVao.create();
    _conesVao.bind();

    QVector<VertexData> conesVertices;
    for(size_t iter = 0; iter < _centroids.size(); iter++)
    {

        int xCenter = _centroids[iter].x;
        int yCenter = _centroids[iter].y;
        float radius = 50;
        //int slice = 16;

        QColor col = _coneColors[iter];
        QVector3D vecCol = QVector3D((float)col.red() / 255.0, (float)col.green() / 255.0, (float)col.blue() / 255.0);
        //float rCol = rand() % 255;
        //float gCol = rand() % 255;
        //float bCol = rand() % 255;
        //QVector3D vecCol = QVector3D(rCol / 255.0, gCol / 255.0, bCol / 255.0);

        conesVertices.append(VertexData(QVector3D(xCenter, yCenter,  10.0f), QVector2D(), vecCol));
        float addValue = (M_PI * 2.0 / (float)_coneSlice);
        for(float a = 0.0; a < M_PI * 2.0; a += addValue)
        {
            float xPt = xCenter + radius * sin(a);
            float yPt = yCenter + radius * cos(a);
            conesVertices.append(VertexData(QVector3D(xPt, yPt,  -10), QVector2D(), vecCol));
        }

        float xPt = xCenter + radius * sin(M_PI * 2.0);
        float yPt = yCenter + radius * cos(M_PI * 2.0);
        conesVertices.append(VertexData(QVector3D(xPt, yPt,  -10), QVector2D(), vecCol));
        //glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
    }

    _conesVbo.create();
    _conesVbo.bind();
    _conesVbo.allocate(conesVertices.data(), conesVertices.size() * sizeof(VertexData));

    // reset offset
    offset = 0;

    //int vertexLocation = _shaderProgram->attributeLocation("vert");
    _shaderProgram->enableAttributeArray(vertexLocation);
    _shaderProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);
    offset += sizeof(QVector2D);

    _shaderProgram->enableAttributeArray(_colorLocation);
    _shaderProgram->setAttributeBuffer(_colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    _conesVao.release();

    // draw buffer
    SaveToBitmap();
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

/*
QMatrix4x4 GLWidget::GetCameraMatrix()
{
    // Todo: Ask if we want to keep this.
    QMatrix4x4 vMatrix;

    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, 20.0);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    return _perspMatrix * vMatrix * _transformMatrix;
}
*/

//void GLWidget::TranslateWorld(float x, float y, float z) {
//    // Todo: Ask if we want to keep this.
//    _transformMatrix.translate(x, y, z);
//}

//void GLWidget::RotateWorld(float x, float y, float z) {
//    // Todo: Ask if we want to keep this.
//    _transformMatrix.rotate(x, y, z);
//}

//void GLWidget::ScaleWorld(float x, float y, float z) {
//    // Todo: Ask if we want to keep this.
//    _transformMatrix.scale(x, y, z);
//}

void GLWidget::HorizontalScroll(int val) { _scrollOffset.setX(val); }
void GLWidget::VerticalScroll(int val) { _scrollOffset.setY(val); }
void GLWidget::ZoomIn() { this->_zoomFactor += 0.05f; }
void GLWidget::ZoomOut() { this->_zoomFactor -= 0.05f; if(this->_zoomFactor < 0.1f) _zoomFactor = 0.1f; }

void GLWidget::Reset()
{
}

void GLWidget::SaveToBitmap()
{
    QGLFramebufferObjectFormat fmt;
    fmt.setAttachment(QGLFramebufferObject::Depth);
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
    grabbedImage.save("image.png");
    _imageBuffer = grabbedImage;
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
    painter.setPen(Qt::NoPen);

    //painter.setPen(QPen(Qt::black));
    // draw points (they are rectangles)
    /*
    for(size_t a = 0; a < 200; a += 10)
    {
        for(size_t b = 0; b < 200; b += 10)
        {
            painter.drawPoint(QPoint(a, b));
        }
    }
    */

    /*
    for(size_t a = 0; a < _initialPoints.size(); a++)
    {
        painter.drawPoint(QPoint(_initialPoints[a].x, _initialPoints[a].y));
        //std:: cout << "(" << _initialPoints[a].x << ", " << _initialPoints[a].y << ")   ";
    }*/


    size_t circleDiameter = 4;
    size_t circleOffset = circleDiameter / 2;
    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
    /*
    for(size_t a = 0; a < 200; a += 10)
    {
        for(size_t b = 0; b < 200; b += 10)
        {
            painter.drawEllipse(a - circleOffset, b - circleOffset, circleDiameter, circleDiameter);
        }
    }
    */

    for(size_t a = 0; a < _centroids.size(); a++)
    {
        int xPt = _centroids[a].x;
        int yPt = _centroids[a].y;
        painter.drawEllipse(xPt - circleOffset, yPt - circleOffset, circleDiameter, circleDiameter);

        //painter.drawPoint(QPoint(_initialPoints[a].x, _initialPoints[a].y));
        //std:: cout << "(" << _initialPoints[a].x << ", " << _initialPoints[a].y << ")   ";
    }

//    QRectF rectangle(10.0, 20.0, 80.0, 60.0);
//    QPainter painter(this);
//    painter.drawEllipse(rectangle);

    //painter.drawLine(QLine(0, 35, 200, 35));
    //painter.drawLine(QLine(0, 165, 200, 165));

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
