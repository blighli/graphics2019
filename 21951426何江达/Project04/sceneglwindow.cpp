#include "sceneglwindow.h"
#include "abstractglscene.h"
#include <QCoreApplication>

SceneGLWindow::SceneGLWindow(QWindow *parent) : QOpenGLWindow(NoPartialUpdate, parent)
{
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [ = ]() {update();});
    timer->start(15);
    m_keyBoardTimer.start(15);
}

void SceneGLWindow::setScene(AbstractGLScene *s)
{
    m_scene = s;
}

void SceneGLWindow::initializeGL() {
    if(m_scene) {
        m_scene->initialize();
    }
}
void SceneGLWindow::paintGL() {
    if(m_scene) {
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
        case Qt::Key_Space:
            emit keySiginal(Qt::Key_Space);
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
    }
}
void SceneGLWindow::keyWProcess()
{
    emit keySiginal(Qt::Key_W);
}
void SceneGLWindow::keyAProcess()
{
    emit keySiginal(Qt::Key_A);
}
void SceneGLWindow::keySProcess()
{
    emit keySiginal(Qt::Key_S);
}
void SceneGLWindow::keyDProcess()
{
    emit keySiginal(Qt::Key_D);
}
