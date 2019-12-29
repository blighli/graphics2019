#include "GLWidget.h"
#include <qmath.h>

#define PI          3.14
#define ROT_DELTA   0.5f

void qgluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    const GLdouble ymax = zNear * tan(qDegreesToRadians(fovy) / 2.0);
    const GLdouble ymin = -ymax;
    const GLdouble xmin = ymin * aspect;
    const GLdouble xmax = ymax * aspect;
    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
    , xRot(0.0f)
    , yRot(0.0f)
    , zRot(0.0f)
{
    // 设置画面的双缓冲和深度缓存
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
}

GLWidget::~GLWidget()
{

}

void GLWidget::initializeGL()
{
    // 启用阴影平滑
    glShadeModel(GL_SMOOTH);
    // 白色背景
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // 设置深度缓存
    glClearDepth(1.0);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 所作深度测试的类型
    glDepthFunc(GL_LEQUAL);
    // 告诉系统对透视进行修正
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    // 启用2D纹理映射
    glEnable(GL_TEXTURE_2D);
    // 加载纹理
    loadGLTextures();
}

void GLWidget::paintGL()
{
    renderScene();
    update();
}

void GLWidget::resizeGL(int width, int height)
{
    // 防止窗口大小变为0
    if ( height == 0 )
    {
        height = 1;
    }
    // 重置当前的视口
    glViewport(0, 0, (GLint)width, (GLint)height);
    // 选择投影矩阵
    glMatrixMode(GL_PROJECTION);
    // 重置投影矩阵
    glLoadIdentity();
    // 设置视口的大小
    qgluPerspective(45.0, (GLdouble)width / (GLdouble)height, 0.1, 100.0);
    // 选择模型观察矩阵
    glMatrixMode(GL_MODELVIEW);
    // 重置投影矩阵
    glLoadIdentity();
}

// 绘制立方体
void GLWidget::drawCube()
{
    glBegin(GL_QUAD_STRIP);         //填充凸多边形
    glColor3f(1, 0, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(1, 1, 0);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0, 1, 0);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glColor3f(0, 1, 1);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glColor3f(1, 0, 0);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glColor3f(1, 1, 0);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glColor3f(0, 1, 0);
    glVertex3f(0.0f, 0.0f, -1.0f);
    glColor3f(0, 1, 1);
    glVertex3f(0.0f, 1.0f, -1.0f);
    glColor3f(1, 0, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(1, 1, 0);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    glColor3f(0, 0, 1);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(1, 0, 1);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glColor3f(0, 1, 0);
    glVertex3f(0.0f, 0.0f, -1.0f);
    glColor3f(1, 0, 0);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glColor3f(1, 1, 0);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(1, 0, 1);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glColor3f(0, 0, 1);
    glVertex3f(0.0f, 1.0f, -1.0f);
    glColor3f(1, 0, 0);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glEnd();
}

// 绘制圆形
void GLWidget::drawCircle()
{
    glBegin(GL_TRIANGLE_FAN);           //扇形连续填充三角形串
    glVertex3f(0.0f, 0.0f, 0.0f);
    int i = 0;
    for (i = 0; i <= 360; i += 15)
    {
        float p = i * 3.14 / 180;
        glColor3f(sin(p), cos(p), tan(p));
        glVertex3f(sin(p), cos(p), 0.0f);
    }
    glEnd();
}

// 绘制圆柱体
void GLWidget::drawCylinder()
{
    // 利用三角形和四边形等基本图元绘制底面圆圆心在坐标原点， 半径为 r，高为 h，方向沿 z 轴方向的圆柱；
    // 侧面用多个四边形,底面用多个三角形来表示
    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    int i = 0;
    for (i = 0; i <= 360; i += 15)
    {
        float p = i * 3.14 / 180;
        glVertex3f(sin(p), cos(p), 1.0f);
        glVertex3f(sin(p), cos(p), 0.0f);
    }
    glEnd();
    //bottom circle
    glColor3f(1, 0, 0);
    drawCircle();
    glTranslatef(0, 0, 1);
    //top circle
    glColor3f(0, 0, 1);
    drawCircle();
    glColor3f(0, 1, 0);
}

// 绘制圆锥体
void GLWidget::drawCone()
{
    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    int i = 0;
    for (i = 0; i <= 360; i += 15)
    {
        float p = i * 3.14 / 180;
        glColor3f(sin(p), cos(p), 1.0f);
        glVertex3f(0, 0, 1.0f);
        glVertex3f(sin(p), cos(p), 0.0f);
    }
    glEnd();
    //bottom circle
    glColor3f(0, 1, 1);
    drawCircle();
}

// 绘制四面体等
void GLWidget::drawTetrahedron()
{
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1, -1, 0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-1, 1, 0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(1, 1, 0);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(1, -1, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glNormal3f(0, -1, 0.707);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-1, -1, 0);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(1, -1, 0);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(0, 0, 1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
    glNormal3f(1, 0, 0.707);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(1, -1, 0);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(1, 1, 0);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(0, 0, 1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 1, 0.707);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(1, 1, 0);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-1, 1, 0);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(0, 0, 1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
    glNormal3f(-1, 0, 0.707);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-1, 1, 0);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-1, -1, 0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0, 0, 1.2);
    glEnd();
}

// 绘制球体
// 球心坐标为（x，y，z），球的半径为radius，M，N分别表示球体的横纵向被分成多少份
void GLWidget::drawSphere(GLfloat xx, GLfloat yy, GLfloat zz,
                          GLfloat radius, GLfloat M, GLfloat N)
{
    // 选择使用的纹理
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    float step_z = PI / M;
    float step_xy = 2 * PI / N;
    float x[4], y[4], z[4];

    float angle_z = 0.0;
    float angle_xy = 0.0;
    int i = 0, j = 0;
    glBegin(GL_QUADS);
    for(i = 0; i < M; i++)
    {
        angle_z = i * step_z;

        for(j = 0; j < N; j++)
        {
            angle_xy = j * step_xy;

            x[0] = radius * sin(angle_z) * cos(angle_xy);
            y[0] = radius * sin(angle_z) * sin(angle_xy);
            z[0] = radius * cos(angle_z);

            x[1] = radius * sin(angle_z + step_z) * cos(angle_xy);
            y[1] = radius * sin(angle_z + step_z) * sin(angle_xy);
            z[1] = radius * cos(angle_z + step_z);

            x[2] = radius * sin(angle_z + step_z) * cos(angle_xy + step_xy);
            y[2] = radius * sin(angle_z + step_z) * sin(angle_xy + step_xy);
            z[2] = radius * cos(angle_z + step_z);

            x[3] = radius * sin(angle_z) * cos(angle_xy + step_xy);
            y[3] = radius * sin(angle_z) * sin(angle_xy + step_xy);
            z[3] = radius * cos(angle_z);
            for(int k = 0; k < 4; k++)
            {
                glColor3f(sin(angle_z), cos(angle_z), tan(angle_z));
                //glTexCoord2f(0.1f, 0.1f);
                glVertex3f(xx + x[k], yy + y[k], zz + z[k]);
            }
        }
    }
    glEnd();
}

// 绘制圆环
// 大半径Radius，小半径TubeRadius，边数Sides， 环数Rings
void GLWidget::DrawTorus(double Radius, double TubeRadius, int Sides, int Rings)
{
    double sideDelta = 2.0 * PI / Sides;
    double ringDelta = 2.0 * PI / Rings;
    double theta = 0;
    double cosTheta = 1.0;
    double sinTheta = 0.0;

    double phi, sinPhi, cosPhi;
    double dist;
    glColor3f(1, 0, 0);
    for (int i = 0; i < Rings; i++)
    {
        double theta1 = theta + ringDelta;
        double cosTheta1 = cos(theta1);
        double sinTheta1 = sin(theta1);

        glBegin(GL_QUAD_STRIP);
        phi = 0;
        for (int j = 0; j <= Sides; j++)
        {
            phi = phi + sideDelta;
            cosPhi = cos(phi);
            sinPhi = sin(phi);
            dist = Radius + (TubeRadius * cosPhi);

            glNormal3f(cosTheta * cosPhi, sinTheta * cosPhi, sinPhi);
            glColor3f(cosTheta, sinTheta, sinPhi);
            glVertex3f(cosTheta * dist, sinTheta * dist, TubeRadius * sinPhi);

            glNormal3f(cosTheta1 * cosPhi, sinTheta1 * cosPhi, sinPhi);
            glColor3f(cosTheta1, sinTheta1, sinPhi);
            glVertex3f(cosTheta1 * dist, sinTheta1 * dist, TubeRadius * sinPhi);
        }
        glEnd();
        theta = theta1;
        cosTheta = cosTheta1;
        sinTheta = sinTheta1;
    }
}

// 加载纹理
void GLWidget::loadGLTextures()
{
    QImage tex1, buf1;
    QImage tex2, buf2;
    if (!buf1.load("D:/workplace/opengl/OpenGLTest2/Data/logo.jpg","jpg"))
    {
        // 如果载入不成功，自动生成一个128*128的32位色的绿色图片。
        qWarning("Could not read image file 1!");
        QImage dummy(128, 128, QImage::Format_RGB32);
        dummy.fill(Qt::green);
        buf1 = dummy;
    }

    if (!buf2.load("D:/workplace/opengl/OpenGLTest2/Data/logo.jpg","jpg"))
    {
        // 如果载入不成功，自动生成一个128*128的32位色的绿色图片。
        qWarning("Could not read image file 2!");
        QImage dummy(128, 128, QImage::Format_RGB32);
        dummy.fill(Qt::green);
        buf2 = dummy;
    }
    //***********************************************//
    // 纹理0：qt-logo
    //***********************************************//
    //转换成纹理类型
    tex1 = QGLWidget::convertToGLFormat(buf1);
    // 创建纹理
    glGenTextures(1, &texture[0]);
    // 使用来自位图数据生成的典型纹理,将纹理名字texture[0]绑定到纹理目标上
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    // WRAP参数：纹理坐标超出[0,0]到[1,1]的范围该怎么处理呢？
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    // Filter参数：纹理坐标映射到纹素位置(127.34,255.14)该怎么办?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 纹理环境
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    // 将纹素数组从CPU传至GPU并且设置为当前纹理。
    // 在处理单一纹理时，你可以用，负责效率非常低。
    // 多纹理时可以参见纹理绑定。
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex1.width(), tex1.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, tex1.bits());

    //***********************************************//
    // 纹理0：qt-logo
    //***********************************************//
    tex2 = QGLWidget::convertToGLFormat(buf2);
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex2.width(), tex2.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, tex2.bits());
    // 使用纹理
    // 首先调用glEnable( GL_TEXTURE_2D )，来启用2D纹理;
    // 然后绘制图形，并且为每个顶点指定ST坐标;
    // 最后调用glDisable( GL_TEXTURE_2D ).
}

// 场景渲染
void GLWidget::renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();
    renderBasicShape();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3, 0, 0);
    renderTextureCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3, 0, 0);
    renderTextureCylinder();
    glPopMatrix();
}

// 渲染基本图形
void GLWidget::renderBasicShape()
{
    static float fRotAngle = 0.0f;
    fRotAngle += ROT_DELTA;
    if (fRotAngle > 360)
        fRotAngle = 0;

    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(-3, 3, -12);
    glRotatef(fRotAngle, 1, 1 , 1);
    drawCylinder();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(0, 3, -12);
    glRotatef(fRotAngle, 1, 1 , 1);
    drawTetrahedron();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(3, 3, -12);
    glRotatef(fRotAngle, 1, 1, 1);
    drawCircle();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(0, 0, -15);
    glRotatef(fRotAngle, 0, 1, 0);
    drawSphere();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 1, 1);
    glTranslatef(-3, -3, -12);
    glRotatef(fRotAngle, 1, 1, 1);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 1);
    glTranslatef(0, -3, -12);
    glRotatef(fRotAngle, 1, 1, 1);
    DrawTorus();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 1, 0);
    glTranslatef(3, -3, -12);
    glRotatef(fRotAngle, 1, 1, 1);
    drawCone();
    glPopMatrix();
}

// 渲染纹理
void GLWidget::renderTextureCube()
{
    // 纹理映射
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0f, 0.0f, -12.0f);
    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(yRot, 0.0, 1.0, 0.0);
    glRotatef(zRot, 0.0, 0.0, 1.0);
    // 使用来自位图数据生成的典型纹理,将纹理名字texture[0]绑定到纹理目标上
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin( GL_QUADS );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( -1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 );
    glVertex3f(  1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 );
    glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 1.0 );
    glVertex3f( -1.0,  1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 );
    glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 1.0 );
    glVertex3f( -1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 );
    glVertex3f(  1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f(  1.0, -1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 );
    glVertex3f( -1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( -1.0,  1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 );
    glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 );
    glVertex3f(  1.0,  1.0, -1.0 );
    glTexCoord2f( 1.0, 1.0 );
    glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 );
    glVertex3f(  1.0, -1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f(  1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 );
    glVertex3f( -1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 );
    glVertex3f(  1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 1.0 );
    glVertex3f(  1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 );
    glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f(  1.0, -1.0,  1.0 );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 0.0 );
    glVertex3f( -1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 );
    glVertex3f( -1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 1.0 );
    glVertex3f( -1.0,  1.0, -1.0 );
    glEnd();
    xRot += ROT_DELTA;
    if (xRot > 360) xRot = 0;
    yRot += ROT_DELTA;
    if (yRot > 360) yRot = 0;
    zRot += ROT_DELTA;
    if (zRot > 360) zRot = 0;
}

void GLWidget::renderTextureCylinder()
{
    // 纹理映射
    glTranslatef(0.0f, 0.0f, -12.0f);
    glRotatef(xRot, 1.0, 0.0, 0.0);
    //glRotatef(yRot, 0.0, 1.0, 0.0);
    //glRotatef(zRot, 0.0, 0.0, 1.0);
    // 选择使用的纹理
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    // 利用三角形和四边形等基本图元绘制底面圆圆心在坐标原点， 半径为 r，高为 h，方向沿 z 轴方向的圆柱；
    // 侧面用多个四边形,底面用多个三角形来表示
    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    int i = 0;
    for (i = 0; i <= 360; i += 15)
    {
        float p = i * 3.14 / 180;
        //p和圆周是相对应的， 这里让纹理的横坐标随圆周扫过的角度一起改变，就能够将纹理图“刷”上去了，
        //而纵坐标设置为图像的高度和纹理高度的对应，这里合适的参数是根据实际测试得到的
        glTexCoord2f(p / 10, 0.1f);
        glVertex3f(sin(p), cos(p), 1.0f);   //这个 1.0f指定的是高度h
        glTexCoord2f(p / 10, 0.0f);
        glVertex3f(sin(p), cos(p), 0.0f);
    }
    glEnd();
    //bottom circle
    glBegin(GL_TRIANGLE_FAN);           //扇形连续填充三角形串
    glTexCoord2f(0.0f, 0.0f);           //将纹理图(0, 0)映射到圆心
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (i = 0; i <= 360; i += 15)
    {
        float p = i * 3.14 / 180;
        glTexCoord2f(1.0f, 0.0f);       //将纹理图(1, 0)映射到圆周
        glVertex3f(sin(p), cos(p), 0.0f);
    }
    glEnd();
    glTranslatef(0, 0, 1);              //设定高度为1，画上底面
    //top circle
    glBegin(GL_TRIANGLE_FAN);           //扇形连续填充三角形串
    glTexCoord2f(0.0f, 0.0f);           //将纹理图(0, 0)映射到圆心
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (i = 0; i <= 360; i += 15)
    {
        float p = i * 3.14 / 180;
        glTexCoord2f(1.0f, 0.0f);       //将纹理图(1, 0)映射到圆周
        glVertex3f(sin(p), cos(p), 0.0f);
    }
    glEnd();

    xRot += ROT_DELTA;
    if (xRot > 360) xRot = 0;
    yRot += ROT_DELTA;
    if (yRot > 360) yRot = 0;
    zRot += ROT_DELTA;
    if (zRot > 360) zRot = 0;
}
