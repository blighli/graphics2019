#include <QtGui>
#include <QtOpenGL>
#include <cmath>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

#include "Cube.h"

Cube::Cube(QWidget *parent)
    : QGLWidget(parent)
{
    //调用setFormat（）使OpenGL的显示描述表支持饭走样。
    setFormat(QGLFormat(QGL::SampleBuffers));

    //初始化私有变量
    rotationX = -38.0;
    rotationY = -58.0;
    rotationZ = 0.0;
    scaling = 1.0;

    //设置填充背景的QRadialGradient
    createGradient();
    //创建OpenGL立方体对象
    createGLObject();

    setAutoBufferSwap( false );
    setAutoFillBackground( false );
}

Cube::~Cube()
{
    makeCurrent();
    //删除构造函数创建的OpenGL立方体对象
    glDeleteLists(glObject, 1);
}

//绘制,在paintEvent()中创建一个QPainter,在进行纯OpenGL操作时
//保存和恢复其状态。
//QPainter的构造函数(或者QPainter::begin())自动调用glClear
//除非预先调用窗口部件的setAutoFillBackground(false)
//QPainter的析构函数（或者QPainter::end()）自动调用QGLWidget::swapBuffers()
//切换可见缓存和离屏缓存，除非预先调用setAutoBufferSwap(false).
//当QPainter被激活，我们可以交叉使用纯OpenGL命令，只要在执行纯OpenGL命令之前保存OpenGL状态，之后恢复OpenGL状态
void Cube::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter( this );
    //绘制背景
    drawBackground( &painter );
    //背景绘制结束
    painter.end( );
    //绘制立方体
    drawCube();
    //绘制开始
    painter.begin(this);
    painter.end( );
    swapBuffers( );
}

//鼠标按下事件
void Cube::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

//鼠标移动事件
void Cube::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();

    if (event->buttons() & Qt::LeftButton) {
        rotationX += 180 * dy;
        rotationY += 180 * dx;
        update();
    } else if (event->buttons() & Qt::RightButton) {
        rotationX += 180 * dy;
        rotationZ += 180 * dx;
        update();
    }
    lastPos = event->pos();
}

//鼠标滚动的事件，调用update
void Cube::wheelEvent(QWheelEvent *event)
{
    double numDegrees = -event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= std::pow(1.125, numSteps);
    update();
}

//使用蓝色渐变色设置QRadialGradient
void Cube::createGradient()
{
    //确保指定的中心和焦点坐标根据窗口部件大小进行调整
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    //位置用0和1之间的浮点数表示，0对应焦点坐标，1对应圆的边缘
    gradient.setCenter(0.45, 0.50);
    gradient.setFocalPoint(0.40, 0.45);
    gradient.setColorAt(0.0, QColor(105, 146, 182));
    gradient.setColorAt(0.4, QColor(81, 113, 150));
    gradient.setColorAt(0.8, QColor(16, 56, 121));
}

//创建OpenGL列表，该列表保存绘制的立方体的边。
void Cube::createGLObject()
{
    makeCurrent();

    glShadeModel(GL_FLAT);

    glObject = glGenLists(1);
    glNewList(glObject, GL_COMPILE);
    qglColor(QColor(255, 239, 191));
    glLineWidth(1.0);

    glBegin(GL_LINES);
    glVertex3f(+1.0, +1.0, -1.0);
    glVertex3f(-1.0, +1.0, -1.0);
    glVertex3f(+1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(+1.0, -1.0, +1.0);
    glVertex3f(-1.0, -1.0, +1.0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(+1.0, +1.0, +1.0);
    glVertex3f(+1.0, +1.0, -1.0);
    glVertex3f(+1.0, -1.0, -1.0);
    glVertex3f(+1.0, -1.0, +1.0);
    glVertex3f(+1.0, +1.0, +1.0);
    glVertex3f(-1.0, +1.0, +1.0);
    glVertex3f(-1.0, +1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, +1.0);
    glVertex3f(-1.0, +1.0, +1.0);
    glEnd();

    glEndList();
}

void Cube::drawBackground(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(gradient);
    painter->drawRect(rect());
}

//绘制立方体
void Cube::drawCube()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    GLfloat x = 3.0 * GLfloat(width()) / height();
    glOrtho(-x, +x, -3.0, +3.0, 4.0, 15.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glScalef(scaling, scaling, scaling);

    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    //设置反走样
    glEnable(GL_MULTISAMPLE);

    //绘制立方体
    glCallList(glObject);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();
}
