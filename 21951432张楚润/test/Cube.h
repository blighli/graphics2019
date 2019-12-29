#ifndef CUBE_H
#define CUBE_H

#include <QGLWidget>
#include <QRadialGradient>

//使用OpenGL绘制立方体，使用QPainter绘制背景
//的渐变，接着使用renderText（）绘制立方体角上的8个
//元音字母，最后使用QPainter和QTextDocument绘制图例。
//用户可以单击并拖动鼠标来旋转立方体，并且可以使用鼠标滚轮进行放大或缩小
class Cube : public QGLWidget
{
    Q_OBJECT

public:
    Cube(QWidget *parent = 0);
    ~Cube();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void createGradient();
    void createGLObject();
    void drawBackground(QPainter *painter);
    void drawCube();

    GLuint glObject;
    QRadialGradient gradient;
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    GLfloat scaling;
    QPoint lastPos;
};

#endif
