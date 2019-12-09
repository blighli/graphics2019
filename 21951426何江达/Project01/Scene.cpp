#include "Scene.h"
#include <QOpenGLWindow>

Scene::Scene(SceneGLWindow* window):
    AbstractGLScene(window)
{
    window->setScene(this);
    connect(window, &SceneGLWindow::camPosChanged,
            this, &Scene::setCamPos);
    connect(window, &SceneGLWindow::camFrontChanged,
            this, &Scene::setCamFront);
    connect(window, &SceneGLWindow::camUpChanged,
            this, &Scene::setCamUp);
    connect(window, &SceneGLWindow::camZoomChanged, this, &Scene::setZoom);
}

Scene::~Scene()
{
    m_shader.release();
}

void Scene::initialize()
{
    AbstractGLScene::initialize();

    m_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader.vert");
    m_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader.frag");
    m_shader.link();

    m_texture = new QOpenGLTexture(QImage(":/Zhejiang_University_Logo.png").mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

    GLfloat vertices[] =
    {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    m_cameraPos   = QVector3D(0.0f, 0.0f,  3.0f);
    m_cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    m_cameraUp    = QVector3D(0.0f, 1.0f,  0.0f);

    m_shader.bind();
    m_shader.setUniformValue("texture", 0);
    m_texture->bind(0);

    m_shader.release();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Scene::paint()
{
    glViewport(0, 0, window()->width(), window()->height());
    glClearColor(.2f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.bind();

    QMatrix4x4 model;
    model.rotate(m_val, QVector3D(1, 1, 0));
    m_shader.setUniformValue("model", model);

    QMatrix4x4 view;
    view.lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    m_shader.setUniformValue("view", view);

    QMatrix4x4 projection;
    projection.perspective(m_zoom, float(window()->width()) / window()->height(), 0.1f, 100.f);
    m_shader.setUniformValue("projection", projection);

    glBindVertexArray(m_VAO);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    QCursor c = window()->cursor();
    c.setPos(window()->mapToGlobal(QPoint(window()->width() / 2, window()->height() / 2)));
    c.setShape(Qt::BlankCursor);
    window()->setCursor(c);
}
qreal Scene::getAnimate() const
{
    return m_val;
}
void Scene::setAnimate(qreal val)
{
    m_val = val;
    window()->update();
}
void Scene::setCamPos(QVector3D pos)
{
    m_cameraPos = pos;
}
void Scene::setCamFront(QVector3D front)
{
    m_cameraFront = front;
}
void Scene::setCamUp(QVector3D up)
{
    m_cameraUp = up;
}
void Scene::setZoom(float zoom)
{
    m_zoom = zoom;
}
