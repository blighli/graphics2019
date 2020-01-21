#ifndef ABSTRACTGLSCENE_H
#define ABSTRACTGLSCENE_H

#include <QOpenGLFunctions_3_3_Core>

class QOpenGLWindow;
class AbstractGLScene : protected QOpenGLFunctions_3_3_Core
{
public:
    AbstractGLScene(QOpenGLWindow *window = 0);
    virtual ~AbstractGLScene();
    QOpenGLWindow* window() const {
        return m_window;
    }
    QOpenGLContext* context();
    const QOpenGLContext* context() const;

    virtual void initialize();
    virtual void paint() = 0;

private:
    QOpenGLWindow* m_window = nullptr;
};

#endif // ABSTRACTGLSCENE_H
