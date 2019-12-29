#ifndef SCENEGLWINDOW_H
#define SCENEGLWINDOW_H

#include <QObject>
#include <QOpenGLWindow>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QVector3D>
#include "Camera.h"
#include <QTime>
#include <QTimer>

class AbstractGLScene;
class SceneGLWindow : public QOpenGLWindow
{
    Q_OBJECT
public:
    SceneGLWindow(QWindow* parent = 0);
    AbstractGLScene* scene() const
    {
        return m_scene;
    }
    void setScene(AbstractGLScene* s);

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent* event);
signals:
    void camPosChanged(QVector3D pos);
    void camFrontChanged(QVector3D front);
    void camUpChanged(QVector3D up);
    void camZoomChanged(float zoom);
    void picking(int xx, int yy);
private slots:
    void keyWProcess();
    void keyAProcess();
    void keySProcess();
    void keyDProcess();
protected:
    void initializeGL();
    void paintGL();
private:
    AbstractGLScene* m_scene = nullptr;
    Camera m_cam;
    bool m_fullScreen = false;
    QTimer m_keyBoardTimer;
    float m_delta;
};

#endif // SCENEGLWINDOW_H
