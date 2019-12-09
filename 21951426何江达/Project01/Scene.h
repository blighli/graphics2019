#ifndef Scene_H
#define Scene_H

#include "abstractglscene.h"
#include "sceneglwindow.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class SceneGLWindow;
class Scene: public QObject, public AbstractGLScene
{
    Q_OBJECT
    Q_PROPERTY(qreal val READ getAnimate WRITE setAnimate)

public:
    Scene(SceneGLWindow* window);
    ~Scene();

    void initialize();
    void paint();
    qreal getAnimate()const;
public slots:
    void setAnimate(qreal val);

    void setCamPos(QVector3D pos);
    void setCamFront(QVector3D front);
    void setCamUp(QVector3D up);
    void setZoom(float zoom);
private:
    QOpenGLShaderProgram m_shader;
    qreal m_val;

    GLuint m_VAO, m_VBO;
    QOpenGLTexture* m_texture;

    QVector3D m_cameraPos, m_cameraFront, m_cameraUp;
    float m_zoom = 45;
};

#endif // Scene_H
