from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *


def drawFunc1():
    glClear(GL_COLOR_BUFFER_BIT)
    glutWireTeapot(0.5)
    glFlush()


def drawFunc():
    glClear(GL_COLOR_BUFFER_BIT)
    glRotatef(0.1, 5, 5, 0)
    glutWireTeapot(0.5)
    # 刷新显示
    glFlush()


def keyboardFunc(key, x, y):
    """
    w和s表示上下移动
    a和d表示左右移动
    t和g表示前后翻转
    f和h表示左右翻转
    r和表示上下翻转
    """
    if key == b'W' or key == b'w':
        glTranslate(0, 0.1, 0)
    elif key == b'S' or key == b's':
        glTranslate(0, -0.1, 0)
    elif key == b'A' or key == b'a':
        glTranslate(-0.1, 0, 0)
    elif key == b'D' or key == b'd':
        glTranslate(0.1, 0, 0)
    elif key == b'T' or key == b't':
        glRotate(1.0, 1, 0, 0)
    elif key == b'g' or key == b'G':
        glRotate(-1.0, 1, 0, 0)
    elif key == b'f' or key == b'F':
        glRotate(1.0, 0, 1, 0)
    elif key == b'h' or key == b'H':
        glRotate(-1.0, 0, 1, 0)
    elif key == b'R' or key == b'r':
        glRotate(-1.0, 0, 0, 1)
    elif key == b'y' or key == b'y':
        glRotate(1.0, 0, 0, 1)


if __name__ == '__main__':
    glutInit()
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
    glutInitWindowSize(600, 600)
    glutCreateWindow(b"First HomeWork")
    glutDisplayFunc(drawFunc)
    glutIdleFunc(drawFunc)
    glutKeyboardFunc(keyboardFunc)
    glutMainLoop()
