#include <stdlib.h>
#include <glut.h>
#include<iostream>
using namespace std;
GLfloat rotate_v = 0.0;  // 设置旋转速率
GLfloat rotate_angle = 0.0;  // 设置旋转角度
GLfloat anglex = 0.0;  // X 轴旋转
GLfloat angley = 0.0;  // Y 轴旋转
GLfloat anglez = 0.0;  // Z 轴旋转
GLfloat scale = 1.0;  // 缩放系数
GLint WinW = 400;
GLint WinH = 400;
GLfloat oldx;  // 当左键按下时记录鼠标坐标
GLfloat oldy;
void init(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);  // 背景黑色
}
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);  // 画笔红色
    // glLoadIdentity加载单位矩阵 无值无参，将之前的一系列矩阵变换全部归位，用单位矩阵替代
    glLoadIdentity();
    // gluLookAt描述相机
    // [0,1,2]相机的坐标
    // [3,4,5]相机视线方向
    // [6,7,8]相机向上的方向在世界坐标系中的方向
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // glRotatef(angle, x_axis, y_axis, z_axis)
    // x_axis, y_axis, z_axis是布尔类型的，就是说要不是0-false，要不非0-true
    glRotatef(rotate_angle, 0.0f, 1.0f, 0.0f);
    glRotatef(anglex, 1.0, 0.0, 0.0);
    glRotatef(angley, 0.0, 1.0, 0.0);
    glRotatef(anglez, 0.0, 0.0, 1.0);
    glScalef(scale, scale, scale);
    glutSolidTeapot(1);
    rotate_angle += rotate_v;
    // glutSwapBuffers交换缓冲区，用于双缓冲项目中
    glutSwapBuffers();
}
void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            rotate_v = 0;
            rotate_angle = 0;
            oldx = x;  // 当左键按下时记录鼠标坐标
            oldy = y;
            cout << "left" << endl;
        }
    }
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            scale += 0.1f;
            cout << "right" << endl;
        }
    }
}
void keyboard(int key, int x, int y) {
    if (key == GLUT_KEY_DOWN) {
        scale -= 0.1f;
        if (scale < 0) {
            scale = 0.0f;
        }
    }
    if (key == GLUT_KEY_UP) {
        scale += 0.1f;
        if (scale > 2) {
            scale = 2.0f;
        }
    }
}
void motion(int x, int y)
{
    // 小trick: 当鼠标沿x轴运动时，我们希望物体绕y轴转动
    GLint deltax = y - oldy;
    GLint deltay = x - oldx;
    // 根据屏幕上鼠标滑动的距离来设置旋转的角度
    anglex += 360 * (GLfloat)deltax / (GLfloat)WinW;
    angley += 360 * (GLfloat)deltay / (GLfloat)WinH;
    // 不像下面这样处理的话，当度数超过360度，操作会反向
    if (anglex > 360) {
        anglex -= 360;
    }
    if (angley > 360) {
        angley -= 360;
    }
    // anglez += 360 * (GLfloat)deltay / (GLfloat)WinH;
    oldx = x;
    oldy = y;
    // glutPostRedisplay重绘物体
    glutPostRedisplay();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutSpecialFunc(keyboard);
    glutMotionFunc(motion);
    glutIdleFunc(display);
    glutMainLoop();
    return 0;
}