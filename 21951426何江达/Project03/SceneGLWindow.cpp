#include "SceneGLWindow.h"
#include "AbstractGLScene.h"
#include <QCoreApplication>
#include "Scene.h"
SceneGLWindow::SceneGLWindow(QWindow* parent) : QOpenGLWindow(NoPartialUpdate, parent), m_delta(0.003),
    m_cam(QVector3D(0.f, 0.f, 3.f))
{
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [ = ]() {update();});
    timer->start(15);
    m_keyBoardTimer.start(15);

}

void SceneGLWindow::setScene(AbstractGLScene* s)
{
    m_scene = s;
}

void SceneGLWindow::initializeGL()
{
    if (m_scene)
    {
        m_scene->initialize();
    }
}
void SceneGLWindow::paintGL()
{
    if (m_scene)
    {
        m_scene->paint();
    }
}
void SceneGLWindow::keyPressEvent(QKeyEvent* event)
{
    float delta = 0.015;
    if (!event->isAutoRepeat())
    {
        switch (event->key())
        {
            case Qt::Key_W:
                connect(&m_keyBoardTimer, &QTimer::timeout, this, &SceneGLWindow::keyWProcess);
                break;
            case Qt::Key_S:
                connect(&m_keyBoardTimer, &QTimer::timeout, this, &SceneGLWindow::keySProcess);
                break;
            case Qt::Key_A:
                connect(&m_keyBoardTimer, &QTimer::timeout, this, &SceneGLWindow::keyAProcess);
                break;
            case Qt::Key_D:
                connect(&m_keyBoardTimer, &QTimer::timeout, this, &SceneGLWindow::keyDProcess);
                break;
            case Qt::Key_F:
                m_fullScreen = !m_fullScreen;
                m_fullScreen ? setVisibility(FullScreen) : setVisibility(Windowed);
                break;
            case Qt::Key_Escape:
                QCoreApplication::exit();
        }
    }
    event->accept();
}
void SceneGLWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat())
    {
        switch (event->key())
        {
            case Qt::Key_W:
                disconnect(&m_keyBoardTimer, &QTimer::timeout, this, &SceneGLWindow::keyWProcess);
                break;
            case Qt::Key_S:
                disconnect(&m_keyBoardTimer, &QTimer::timeout, this, &SceneGLWindow::keySProcess);
                break;
            case Qt::Key_A:
                disconnect(&m_keyBoardTimer, &QTimer::timeout, this, &SceneGLWindow::keyAProcess);
                break;
            case Qt::Key_D:
                disconnect(&m_keyBoardTimer, &QTimer::timeout, this, &SceneGLWindow::keyDProcess);
                break;
        }
        //        m_keyBoardTimer.disconnect();
    }
}
void SceneGLWindow::mouseMoveEvent(QMouseEvent* event)
{
    static bool flg = false;
    float xoffset = event->x() - width() / 2;
    float yoffset = event->y() - height() / 2;

    if (m_fullScreen == false)
    {
        flg = false;
    }
    if (m_fullScreen && flg == false)
    {
        flg = true;
    }
    else if (!m_objectMod)
        m_cam.ProcessMouseMovement(xoffset, -yoffset);

    if (m_objectMod)
    {
        emit objRot(-yoffset, xoffset);
    }
    emit camUpChanged(m_cam.Up);
    emit camFrontChanged(m_cam.Front);

    event->accept();
}
void SceneGLWindow::mousePressEvent(QMouseEvent* event)
{
    emit picking(event->x(), event->y());
}
void SceneGLWindow::wheelEvent(QWheelEvent* event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    QPoint numSteps = numDegrees / 15;
    if (!m_objectMod)
    {
        m_cam.ProcessMouseScroll(numSteps.y());
        emit camZoomChanged(m_cam.Zoom);
    }
    else
    {
        emit objScale(numSteps.y());
    }
    event->accept();
}
void SceneGLWindow::keyWProcess()
{
    if (!m_objectMod)
    {
        m_cam.ProcessKeyboard(FORWARD, m_delta);
        emit camPosChanged(m_cam.Position);
    }
    else
    {
        emit objChange(0, 0, -m_delta);
    }
}
void SceneGLWindow::keyAProcess()
{
    if (!m_objectMod)
    {
        m_cam.ProcessKeyboard(LEFT, m_delta);
        emit camPosChanged(m_cam.Position);
    }
    else
    {
        emit objChange(-m_delta, 0, 0);
    }
}
void SceneGLWindow::keySProcess()
{
    if (!m_objectMod)
    {
        m_cam.ProcessKeyboard(BACKWARD, m_delta);
        emit camPosChanged(m_cam.Position);

    }
    else
    {
        emit objChange(0, 0, m_delta);
    }
}
void SceneGLWindow::keyDProcess()
{
    if (!m_objectMod)
    {
        emit camPosChanged(m_cam.Position);
        m_cam.ProcessKeyboard(RIGHT, m_delta);
    }
    else
    {
        emit objChange(m_delta, 0, 0);
    }
}
void SceneGLWindow::setObjMod(bool objmod)
{
    m_objectMod = objmod;
//    qDebug() << "objmod" << objmod;
}
