#ifndef Scene_H
#define Scene_H

#include "AbstractGLScene.h"
#include "SceneGLWindow.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "modelloader.h"
struct object
{
    QOpenGLVertexArrayObject VAO;
    QOpenGLBuffer VBO, NBO, IBO, texCoordBO;
    ModelLoader loader;
    QOpenGLShaderProgram shader;
    int picked;
    object(): VBO(QOpenGLBuffer::VertexBuffer),
        NBO(QOpenGLBuffer::VertexBuffer),
        IBO(QOpenGLBuffer::IndexBuffer),
        texCoordBO(QOpenGLBuffer::VertexBuffer),
        picked(0)
    {

    }
};
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
signals:
    void objMod(bool objmod);
public slots:
    void setAnimate(qreal val);

    void setCamPos(QVector3D pos);
    void setCamFront(QVector3D front);
    void setCamUp(QVector3D up);
    void setZoom(float zoom);
    void processSelection(int xx, int yy);
    void setObjRot(float pitch, float yaw);
    void setObjTran(float x, float y, float z);
    void setObjScal(float scal);
private:
    void createGeometry(QString path, object& s);
    void drawNode(const QMatrix4x4& model, const Node* node, QMatrix4x4 parent, object& s);
    void animate();
    void renderSelection(void);
    inline float radians(float ang)
    {
        return ang / 180.0 * M_PI;
    }
//    void processSelection(int xx, int yy);
    qreal m_val;

    QVector3D m_cameraPos, m_cameraFront, m_cameraUp;
    float m_zoom = 45;

    object nanosuit;
    object picking;
    object sun;
    object moon;
    object earth;

    QMatrix4x4 nanoModel, sunModel, moonModel, earthModel;

    int lightmod;
//    QOpenGLShaderProgram m_selectionShader;
//    QOpenGLVertexArrayObject m_selectionVAO;
};

#endif // Scene_H
