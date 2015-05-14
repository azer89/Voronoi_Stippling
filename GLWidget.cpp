
#include "stdafx.h"
#include "GLWidget.h"

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

GLWidget::GLWidget(QGLFormat format, QWidget *parent) :
    QGLWidget(format, parent),
    _isMouseDown(false),
    _zoomFactor(1.0),
    mVBO(QOpenGLBuffer::VertexBuffer),
    mVertexArrayObject(this),
    mTexture(0),
    mProgram(0)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    std::cout << "Qt version >= 5.1.0\n";
#endif
}


GLWidget::~GLWidget()
{
    if(mTexture) delete mTexture;
    if(mProgram) delete mProgram;
}


void GLWidget::initializeGL()
{
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers())
        { std::cerr << "Could not enable sample buffers." << std::endl; return; }

    glShadeModel(GL_SMOOTH);
    glClearColor( 0.4, 0.4, 0.4, 0.0 );
    glEnable(GL_DEPTH_TEST);

    mProgram = new QOpenGLShaderProgram();
    if (!mProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "shader.vert"))
        { std::cerr << "Cannot load vertex shader." << std::endl; return; }

    if (!mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "shader.frag"))
        { std::cerr << "Cannot load fragment shader." << std::endl; return; }

    if ( !mProgram->link() )
        { std::cerr << "Cannot link shaders." << std::endl; return; }

    mVertexArrayObject.create();
    mVertexArrayObject.bind();

    mVBO.create();
    mVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);

    if (!mVBO.bind())
    {
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }

    mProgram->bind();

    mProgram->enableAttributeArray("vert");
    mProgram->setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    mMvpMatrixLocation = mProgram->uniformLocation("mvpMatrix");
    mColorLocation = mProgram->uniformLocation("frag_color");
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
    mProgram->setUniformValue(mColorLocation, col.red(), col.green(), col.blue());
}


void GLWidget::DrawImage()
{
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, Texture);
//    // Set our "myTextureSampler" sampler to user Texture Unit 0
//    glUniform1i(TextureID, 0);

    /*
    const GLfloat rectVertices[] =
    {
        0.0, 0.0, 0.0,
        _img_width, 0.0, 0.0,
        _img_width, _img_height, 0.0,
        0.0, _img_height, 0.0
    };

    const GLfloat uvData[] =
    {
        0, 1,
        1, 1,
        1, 0,
        0, 0
    };*/




    //mVBO.allocate(rectVertices, 12 * sizeof(float));
    //glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
}

void GLWidget::DrawLine(MyPoint p1, MyPoint p2)
{
    /*
    glLineWidth(2.0);

    QVector<GLfloat> vertData;
    vertData.append(p1.x);
    vertData.append(p1.y);
    vertData.append(0.0);
    //vertData.append(0.0);
    vertData.append(p2.x);
    vertData.append(p2.y);
    vertData.append(0.0);
    //vertData.append(0.0);

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    glDrawArrays(GL_LINES, 0, 2);*/


    glLineWidth(5.0);

    VertexData vertices[] = {
            // Vertex data for face 0
            {QVector3D(p1.x, p1.y,  0.0f), QVector2D()},  // v0
            {QVector3D(p2.x, p2.y,  0.0f), QVector2D()} // v1
    };

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(vertices, 2 * sizeof(VertexData));

    // Offset for position
   quintptr offset = 0;

   // Tell OpenGL programmable pipeline how to locate vertex position data
   int vertexLocation = mProgram->attributeLocation("vert");
   mProgram->enableAttributeArray(vertexLocation);
   mProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

   // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
   int texcoordLocation = mProgram->attributeLocation("uv");
   mProgram->enableAttributeArray(texcoordLocation);
   mProgram->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

   glDrawArrays(GL_LINES, 0, 2);
   // Draw cube geometry using indices from VBO 1
   //glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, 0);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, this->width(),  this->height());

    int current_width = width();
    int current_height = height();

    SetColor(QColor(0.0, 0.0, 0.0));

    // Set orthographic Matrix
    QMatrix4x4 orthoMatrix;

    orthoMatrix.ortho(0.0 +  _scrollOffset.x(),
                      (float)current_width +  _scrollOffset.x(),
                      (float)current_height + _scrollOffset.y(),
                      0.0 + _scrollOffset.y(),
                      -0.1, 0.1);


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
    mProgram->setUniformValue(mMvpMatrixLocation, orthoMatrix * transformMatrix);

    if(points.size() > 0)
    {
        for(size_t a = 0; a < points.size(); a++)
        {
            for(size_t b = 0; b < points[a].size() - 1; b++)
                { DrawLine(points[a][b], points[a][b+1]); }
        }

    }

    if(tempPoints.size() > 2)
    {
        for(size_t a = 0; a < tempPoints.size() -1 ; a++)
            { DrawLine(tempPoints[a], tempPoints[a+1]); }
    }

    // draw the input image
    DrawImage();
}


// Mouse is pressed
void GLWidget::mousePressEvent(int x, int y)
{
    _isMouseDown = true;

    double dx = x + _scrollOffset.x();
    dx /= _zoomFactor;

    double dy = y + _scrollOffset.y();
    dy /= _zoomFactor;


    tempPoints.push_back(MyPoint(dx, dy));

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
        tempPoints.push_back(MyPoint(dx, dy));
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

    if(tempPoints.size() >= 2)
    {
        tempPoints.push_back(MyPoint(dx, dy));
        std::vector<MyPoint> tempPoints2;
        for(size_t a = 0; a < tempPoints.size(); a++)
        {
            tempPoints2.push_back(tempPoints[a]);
        }
        points.push_back(tempPoints2);
        tempPoints.clear();
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
    this->Reset();
    _imgOriginal.load(img);

    // size
    this->_img_width = _imgOriginal.width();
    this->_img_height = _imgOriginal.height();

    // calculating power-of-two (pow) size
    int xpow = (int) std::pow(2.0, std::ceil( std::log10((double)_img_width )/std::log10(2.0) ) );
    int ypow = (int) std::pow(2.0, std::ceil( std::log10((double)_img_height )/std::log10(2.0) ) );

    xpow = my_max(xpow, ypow);	// the texture should be square too
    xpow = my_min(xpow, 1024);	// shrink if the size is too big
    ypow = xpow;

    // transform the image to square pow size
    _imgGL = _imgOriginal.scaled(xpow, ypow, Qt::IgnoreAspectRatio);
    _imgGL = QGLWidget::convertToGLFormat(_imgGL);

    glGenTextures(1, &_imgID);
    glBindTexture( GL_TEXTURE_2D, _imgID );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _imgGL.width(), _imgGL.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _imgGL.bits());

    std::cout << "bind texture\n";

    this->updateGL(); // Update !
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

    return mPerspMatrix * vMatrix * mTransformMatrix;
}

void GLWidget::TranslateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.translate(x, y, z);
}

void GLWidget::RotateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.rotate(x, y, z);
}

void GLWidget::ScaleWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.scale(x, y, z);
}

void GLWidget::HorizontalScroll(int val) { _scrollOffset.setX(val); }
void GLWidget::VerticalScroll(int val) { _scrollOffset.setY(val); }
void GLWidget::ZoomIn() { this->_zoomFactor += 0.05f; }
void GLWidget::ZoomOut() { this->_zoomFactor -= 0.05f; if(this->_zoomFactor < 0.1f) _zoomFactor = 0.1f; }

void GLWidget::Reset()
{
}
