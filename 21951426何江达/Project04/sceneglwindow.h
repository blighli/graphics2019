#ifndef SCENEGLWINDOW_H
#define SCENEGLWINDOW_H

#include <QObject>
#include <QOpenGLWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
class AbstractGLScene;
class SceneGLWindow : public QOpenGLWindow
{
    Q_OBJECT
public:
    SceneGLWindow(QWindow *parent = 0);
    AbstractGLScene* scene() const {
        return m_scene;
    }
    void setScene(AbstractGLScene *s);

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
signals:
    void keySiginal(Qt::Key k);
protected:
    void initializeGL();
    void paintGL();
private slots:
    void keyWProcess();
    void keyAProcess();
    void keySProcess();
    void keyDProcess();
private:
    AbstractGLScene *m_scene = nullptr;
    QTimer m_keyBoardTimer;
    bool m_fullScreen = false;
};

#endif // SCENEGLWINDOW_H
