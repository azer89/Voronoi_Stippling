
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

    //glutSolidCone((GLdouble)1, (GLdouble)2, (GLint)50, (GLint)5);
    //glutSolidCone( 1, 1, 1, 1 );
};

// GLWidget
GLWidget::GLWidget(QGLFormat format, QWidget *parent) :
    QGLWidget(format, parent),
    _isMouseDown(false),
    _zoomFactor(1.0),
    _vbo(QOpenGLBuffer::VertexBuffer),
    _vertexArrayObject(this),
    _texture(0),
    _shaderProgram(0),
    _rSampling(0),
    _numSample(10000)
    //_svgPainter(0)
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

    _vertexArrayObject.create();
    _vertexArrayObject.bind();

    _vbo.create();
    _vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    if (!_vbo.bind())
    {
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }

    _shaderProgram->bind();

    _mvpMatrixLocation = _shaderProgram->uniformLocation("mvpMatrix");
    _colorLocation = _shaderProgram->attributeLocation("vertexColor");

    // sampling
    _rSampling = new RejectionSampling();

    // svg
    //_svgPainter = new SvgPainter();
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



void GLWidget::DrawImage()
{
    if(_img_width == 0 && _img_height == 0)
    {
        return;
    }

    int use_color_location = _shaderProgram->uniformLocation("use_color");
    _shaderProgram->setUniformValue(use_color_location, (GLfloat)0.0);
    _texture->bind();

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

   glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

}

void GLWidget::DrawCones()
{



    if(_img_width == 0 && _img_height == 0)
    {
        return;
    }


    int use_color_location = _shaderProgram->uniformLocation("use_color");
    _shaderProgram->setUniformValue(use_color_location, (GLfloat)1.0);
    //SetColor(QColor(0.0, 1.0, 0.0));


    for(size_t iter = 0; iter < _initialPoints.size(); iter++)
    {
        QVector<VertexData> vertices;

        int xCenter = _initialPoints[iter].x;
        int yCenter = _initialPoints[iter].y;
        float radius = 50;
        int slice = 16;
        //bool changeColor = false;

        QColor col = _coneColors[iter];
        QVector3D vecCol = QVector3D((float)col.red() / 255.0, (float)col.green() / 255.0, (float)col.blue() / 255.0);
        //SetColor(QColor(col.red(), col.green(), col.blue()));

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


}

void GLWidget::DrawPoints()
{
    if(_img_width == 0 && _img_height == 0)
    {
        return;
    }

    glPointSize(3.0f);

    int use_color_location = _shaderProgram->uniformLocation("use_color");
    _shaderProgram->setUniformValue(use_color_location, (GLfloat)1.0);
    //SetColor(QColor(255, 0, 0));

    QVector<VertexData> vertices;

    for(size_t a = 0; a < _initialPoints.size(); a++)
    {
        int xPt = _initialPoints[a].x;
        int yPt = _initialPoints[a].y;

        /*
        int rCol = a % 255;
        int gCol = (a >> 8) % 255;
        int bCol = (a >> 16) % 255;*/
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

   glDrawArrays(GL_POINTS, 0, _initialPoints.size());
}

void GLWidget::DrawLine(MyPoint p1, MyPoint p2)
{
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
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, this->width(),  this->height());

    int current_width = width();
    int current_height = height();

    //SetColor(QColor(0.0, 0.0, 0.0));

    // Set orthographic Matrix
    QMatrix4x4 orthoMatrix;

    /*
    orthoMatrix.ortho(0.0 +  _scrollOffset.x(),
                      (float)current_width +  _scrollOffset.x(),
                      (float)current_height + _scrollOffset.y(),
                      0.0 + _scrollOffset.y(),
                      -0.1, 0.1);
                      */

    orthoMatrix.ortho(0.0 +  _scrollOffset.x(),
                      (float)current_width +  _scrollOffset.x(),
                      (float)current_height + _scrollOffset.y(),
                      0.0 + _scrollOffset.y(),
                      -100, 100);

    /*
    orthoMatrix.ortho(QRect(0.0 +  _scrollOffset.x(),
                            (float)current_width +  _scrollOffset.x(),
                            (float)current_height + _scrollOffset.y(),
                            0.0 + _scrollOffset.y()));
    */

    // Translate the view to the middle
    QMatrix4x4 transformMatrix;
    transformMatrix.setToIdentity();
    transformMatrix.scale(_zoomFactor);

    // Bind buffer object
    _shaderProgram->setUniformValue(_mvpMatrixLocation, orthoMatrix * transformMatrix);

    if(_points.size() > 0)
    {
        for(size_t a = 0; a < _points.size(); a++)
        {
            for(size_t b = 0; b < _points[a].size() - 1; b++)
                { DrawLine(_points[a][b], _points[a][b+1]); }
        }

    }

    if(_tempPoints.size() > 2)
    {
        for(size_t a = 0; a < _tempPoints.size() -1 ; a++)
            { DrawLine(_tempPoints[a], _tempPoints[a+1]); }
    }

    DrawCones();

    DrawPoints();

    // draw the input image
    DrawImage();

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

    // testing
    /*
    if(_img_width != 0 && _img_height != 0)
    {
        int grayValue = qGray(_imgOriginal.pixel(dx, dy));
        std::cout << grayValue << "\n";
    }
    */

    _tempPoints.push_back(MyPoint(dx, dy));

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
    // error, need a square image

    this->Reset();
    //_imgOriginal.load(img);
    _imgOriginal = LoadAsGrayscale(img);

    // size
    this->_img_width = _imgOriginal.width();
    this->_img_height = _imgOriginal.height();

    std::cout << "image loaded. width: " << _img_width << ", height: " << _img_height << "\n";

    _texture = new QOpenGLTexture(_imgOriginal);
    _texture->setMinificationFilter(QOpenGLTexture::Nearest);
    _texture->setMagnificationFilter(QOpenGLTexture::Linear);

    _shaderProgram->setAttributeValue("base_texture", _texture->textureId());

    // calculate random sampling    
    _initialPoints.clear();
    _initialPoints = _rSampling->GeneratePoints(GetGrayValues(), _numSample, _img_width, _img_height);
    std::cout << "_initialPoints.size() equals to " << _initialPoints.size() << "\n";


    //for(size_t a = 0; a < _initialPoints.size(); a++)
    //{
    //    std:: cout << "(" << _initialPoints[a].x << ", " << _initialPoints[a].y << ")\n";
    //}
    GenerateConeColors();



}

QImage GLWidget::LoadAsGrayscale(QString img)
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

void GLWidget::TranslateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    _transformMatrix.translate(x, y, z);
}

void GLWidget::RotateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    _transformMatrix.rotate(x, y, z);
}

void GLWidget::ScaleWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    _transformMatrix.scale(x, y, z);
}

void GLWidget::HorizontalScroll(int val) { _scrollOffset.setX(val); }
void GLWidget::VerticalScroll(int val) { _scrollOffset.setY(val); }
void GLWidget::ZoomIn() { this->_zoomFactor += 0.05f; }
void GLWidget::ZoomOut() { this->_zoomFactor -= 0.05f; if(this->_zoomFactor < 0.1f) _zoomFactor = 0.1f; }

void GLWidget::Reset()
{
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

    for(size_t a = 0; a < _initialPoints.size(); a++)
    {
        int xPt = _initialPoints[a].x;
        int yPt = _initialPoints[a].y;
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

    //std::random_shuffle(rVector.begin(), rVector.end());
    //std::random_shuffle(gVector.begin(), gVector.end());
    //std::random_shuffle(bVector.begin(), bVector.end());


    _coneColors.clear();
    for(size_t a = 0; a < 256 && _coneColors.size() < _initialPoints.size(); a++)
    {
        for(size_t b = 0; b < 256 && _coneColors.size() < _initialPoints.size(); b++)
        {
            for(size_t c = 0; c < 256 && _coneColors.size() < _initialPoints.size(); c++)
            {
                _coneColors.push_back(QColor(rVector[a], gVector[b], bVector[c]));
                //std::cout << rVector[a] << " " << gVector[b] << " " << bVector[c] << "\n";
            }
        }
    }
}
