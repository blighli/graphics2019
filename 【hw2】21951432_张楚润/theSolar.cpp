#include "windows.h"
#include <gl\glut.h>

// 三者的大小比例
#define SunSize 0.4
#define EarthSize 0.06
#define MoonSize 0.016

GLfloat SpeedMultiplicator = 0.01;// 运行倍数
// 时间比例
GLfloat DaysPerYear = 365.24; //OK, ok... but it is soo slow with 360!
GLfloat DaysPerMoon =  27.321; // 月球公转一圈的天数
 //degrees 累计角度 
GLfloat EarthAroundSun = 0.0;
GLfloat EarthItsSelf = 0.0;
GLfloat moonAroundEarth = 0.0;
GLfloat moonItsSelf = 0.0;
// 半径
GLfloat EarthOrbitRadius = 1.0; // 地球轨道半径
GLfloat MoonOrbitRadius = 0.15;// 月球轨道半径 
// 角速度
GLfloat EarthItsSelfSpeed =  360.0 / 1.0 * SpeedMultiplicator;  // 以一天为单位时间
GLfloat moonAroundEarthSpeed = 360.0 / DaysPerMoon  * SpeedMultiplicator;
GLfloat moonItsSelfSpeed = moonAroundEarthSpeed; // 月球同步自转
GLfloat EarthAroundSunSpeed = 360.0  / DaysPerYear * SpeedMultiplicator; 

void RenderScene(void)
{
    glPushMatrix(); // 压栈1
        gluLookAt(    0.0,3.0,3.0, // 眼睛的位置 z轴+y轴=45%
                    0.0,-1.0,-1.0, // 眼睛的朝向的位置 z轴+y轴=45%
                    0.0,1.0,0.0); // 相片朝上的方向（因为你可以歪着头看同一个物体） y轴
        glColor3f(1.0,1.0,0.5);
        glutWireSphere(SunSize,20,20); // 绘制太阳

        glPushMatrix(); // 压栈2
            glRotatef(EarthAroundSun,0.0,1.0,0.0); // 地球公转的轨道角度 y轴
            glTranslatef(EarthOrbitRadius,0.0,0.0);// 地球轨道半径
            glRotatef(-EarthAroundSun,0.0,1.0,0.0);

            glPushMatrix(); // 压栈3
                glRotatef(EarthItsSelf,0.4348,1.0,0.0);  //  地球自转的轨道角度 
                glColor3f(0.0,0.5,0.8);
                glutWireSphere(EarthSize,8,8);  //Draw earth 绘制地球
                //Draw earth rotation axis 地球自转轴线
                glLineStipple (0.1, 0x0F0F);  // 虚化线 部分1
                glEnable(GL_LINE_STIPPLE); // 虚化线 部分2
                glBegin(GL_LINES); // 直线 
                    glVertex3f(0.1087,0.25,0.0);
                    glVertex3f(-0.1087,-0.25,0.0);
                glEnd();
                glDisable(GL_LINE_STIPPLE);  // 虚化线 部分3

            glPopMatrix(); // 出栈3

            // 恢复到 地球球心的坐标系
            glRotatef(moonAroundEarth,0.0,1.0,0.0); // 月球环绕地球的轨道角度 
            glTranslatef(MoonOrbitRadius,0.0,0.0); //  月球轨道半径 > 地球大小半径
            //The following 2 lines should be combined, but it is better to understand this way
            glRotatef(-moonAroundEarth,0.0,1.0,0.0); // 月球沿着Y轴逆时针自转
            glRotatef(moonItsSelf,0.0,1.0,0.0); // 月球沿着Y轴顺时针自转
            glColor3f(0.8,0.8,0.75); // 设置颜色
            glutWireSphere(MoonSize,8,8);  // 绘制球体
        glPopMatrix();// 出栈2

    // 恢复到 太阳球心的坐标系            
    glPopMatrix();// 出栈1

}

void Init(void)
{
    glClearColor(0.0,0.0,0.0,0.0);// 清除颜色
    glClearDepth(10.0); // 清除深度
    glMatrixMode(GL_MODELVIEW);// 模型矩阵
    glLoadIdentity();// 把视景体截取的图像按特殊处理显示到屏幕上
}

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    RenderScene(); // 渲染
    glFlush();
    glutSwapBuffers();
}

void Reshape(int x, int y)
{
    if (y == 0) return;
    glMatrixMode(GL_PROJECTION); // 投影矩阵
    glLoadIdentity();
    gluPerspective(30.0,(GLdouble)x/(GLdouble)y,0.8,10.0); // 设定投影变换
    glMatrixMode(GL_MODELVIEW); // 模型矩阵
    glViewport(0,0,x,y); // 把视景体截取的图像按特殊处理显示到屏幕上
    Display();
}
static long long times = 0;
void Idle(void)
{
    times++; // 延迟
    if(times>50000)
        times =0;
    if(times% 50000== 0)
    {
        EarthItsSelf += EarthItsSelfSpeed; // 一天的速度
        EarthAroundSun += EarthAroundSunSpeed; // 一年的速度 
        moonItsSelf += moonItsSelfSpeed; // 月球自转的转速
        moonAroundEarth += moonAroundEarthSpeed; // 月球围绕地球的转速
        Display();
    }
}


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800,600);
    glutCreateWindow(argv[0]);
    Init();
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutMainLoop();
    return 0;
}