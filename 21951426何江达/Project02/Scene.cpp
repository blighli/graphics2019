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

    connect(window, &SceneGLWindow::picking, this, &Scene::processSelection);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [ = ]() {animate();});
    timer->start(15);
}

Scene::~Scene()
{

}
void Scene::createGeometry(QString path, sphere& s)
{

    if (!s.loader.Load(   path, ModelLoader::AbsolutePath))
    {
        qDebug() << "ModelLoader failed to load model" << s.shader.log();
        exit(1);
    }

    QVector<GLfloat>* v, *n, *texCoords;
    QVector<GLuint>* i;
    s.loader.getBufferData(&v, &n, &i, &texCoords);

    s.VAO.create();
    s.VAO.bind();


    s.VBO.create();
    s.VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    s.VBO.bind();
    s.VBO.allocate(&(*v)[0], v->size()*sizeof((*v)[0]));

    s.shader.enableAttributeArray(0);
    s.shader.setAttributeArray(0, GL_FLOAT, 0, 3);

    s.NBO.create();
    s.NBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    s.NBO.bind();
    s.NBO.allocate(&(*n)[0], n->size()*sizeof((*n)[0]));

    s.shader.enableAttributeArray(1);
    s.shader.setAttributeBuffer(1, GL_FLOAT, 0, 3);

    s.IBO.create();
    s.IBO.setUsagePattern(QOpenGLBuffer::StaticDraw);

    s.IBO.bind();
    s.IBO.allocate(&(*i)[0], i->size()*sizeof((*i)[0]));
    if (texCoords->size())
    {
        s.texCoordBO.create();
        s.texCoordBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
        s.texCoordBO.bind();
        s.texCoordBO.allocate(&(*texCoords)[0], texCoords->size()*sizeof((*texCoords)[0]));
        s.shader.enableAttributeArray(2);
        s.shader.setAttributeBuffer(2, GL_FLOAT, 0, 2);
    }

    s.VAO.release();
}
void Scene::drawNode(const QMatrix4x4& model, const Node* node, QMatrix4x4 parent, sphere& s)
{
    // Prepare matrices
    QMatrix4x4 local = parent * node->transformation;

    QMatrix4x4 view;
    view.lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

    QMatrix4x4 m = model * local;
    QMatrix4x4 mv = view * model * local;

    QMatrix4x4 projection;
    projection.perspective(m_zoom, float(window()->width()) / window()->height(), 0.1f, 100.f);

    s.shader.setUniformValue("M",  m);
    s.shader.setUniformValue("N",   m.normalMatrix());
    s.shader.setUniformValue("MVP", projection * mv);
    s.shader.setUniformValue("picked", s.picked);
    // Draw each mesh in this node
    for (int i = 0; i < node->meshes.size(); ++i)
    {
        const Mesh& m = *node->meshes[i];

        if (m.material->hasTexture == true)
        {
            // bind appropriate textures
            unsigned int diffuseNr  = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr   = 1;
            unsigned int heightNr   = 1;
            int h = 0;
            for (auto texture : m.material->tex)
            {
                QString number;
                QString name = texture.type;
                if (name == "texture_diffuse")
                    number = QString::number(diffuseNr++);
                else if (name == "texture_specular")
                    number = QString::number(specularNr++); // transfer unsigned int to stream
                else if (name == "texture_normal")
                    number = QString::number(normalNr++); // transfer unsigned int to stream
                else if (name == "texture_height")
                    number = QString::number(heightNr++); // transfer unsigned int to stream
                s.shader.setUniformValue((name + number).toStdString().c_str(), h);
                texture.texture->bind(h++);
//                qDebug() << (name + number).toStdString().c_str();
//                qDebug() << h;
            }
        }
        if (m.material->Name == QString("DefaultMaterial"))
        {
            s.shader.setUniformValue("material.Ka",        QVector3D(  0.05f, 0.2f, 0.05f ));
            s.shader.setUniformValue("material.Kd",        QVector3D(  0.3f,  0.5f, 0.3f  ));
            s.shader.setUniformValue("material.Ks",        QVector3D(  0.6f,  0.6f, 0.6f  ));
            s.shader.setUniformValue("material.shininess", 50.f);
        }
        else
        {
            s.shader.setUniformValue("material.Ka",        m.material->Ambient);
            s.shader.setUniformValue("material.Kd",        m.material->Diffuse);
            s.shader.setUniformValue("material.Ks",        m.material->Specular);
            s.shader.setUniformValue("material.shininess", m.material->Shininess);
        }

        glDrawElements(GL_TRIANGLES, m.indexCount, GL_UNSIGNED_INT, (const GLvoid*)(m.indexOffset * sizeof(GLuint)));
    }

    // Recursively draw this nodes children nodes
    for (int i = 0; i < node->nodes.size(); ++i)
        drawNode(model, &node->nodes[i], local, s);
}


void Scene::initialize()
{
    AbstractGLScene::initialize();

    sun.shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/sun.vert");
    sun.shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/sun.frag");
    sun.shader.link();

    earth.shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/earth.vert");
    earth.shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/earth.frag");
    earth.shader.link();

    moon.shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/moon.vert");
    moon.shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/moon.frag");
    moon.shader.link();

    picking.shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/selection.vert");
    picking.shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/selection.frag");
    picking.shader.link();

    createGeometry("../Project02/res/sun1.obj", sun);
    createGeometry("../Project02/res/earth1.obj", earth);
    createGeometry("../Project02/res/moon1.obj", moon);
    createGeometry("../Project02/res/sphere.obj", picking);

    glEnable(GL_DEPTH_TEST);

    m_cameraPos   = QVector3D(0.0f, 3.0f,  6.0f);
    m_cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    m_cameraUp    = QVector3D(0.0f, 1.0f,  0.0f);

}


void Scene::paint()
{
    glViewport(0, 0, window()->width(), window()->height());
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sun.shader.bind();
    sun.shader.setUniformValue("viewPos", m_cameraPos);
    sun.VAO.bind();

    drawNode(sunModel, sun.loader.getNodeData().data(), QMatrix4x4(), sun);

    sun.VAO.release();
    sun.shader.release();

    earth.shader.bind();
    earth.shader.setUniformValue("viewPos", m_cameraPos);
    earth.shader.setUniformValue("pointLight.position", QVector3D(0.f, 0.f, 0.f));
    // 3250	1.0	0.0014	0.000007
    earth.shader.setUniformValue("pointLight.constant", 1.f);
    earth.shader.setUniformValue("pointLight.linear", 0.0014f);
    earth.shader.setUniformValue("pointLight.quadratic", 0.000007f);

    earth.shader.setUniformValue("pointLight.ambient", QVector3D(.2f, .2f, .2f));
    earth.shader.setUniformValue("pointLight.diffuse", QVector3D(.5f, .5f, .5f));
    earth.shader.setUniformValue("pointLight.specular", QVector3D(1.f, 1.f, 1.f));
    earth.VAO.bind();

    drawNode(earthModel, earth.loader.getNodeData().data(), QMatrix4x4(), earth);

    earth.VAO.release();
    earth.shader.release();

    moon.shader.bind();
    moon.shader.setUniformValue("viewPos", m_cameraPos);
    moon.shader.setUniformValue("pointLight.position", QVector3D(0.f, 0.f, 0.f));
    // 3250	1.0	0.0014	0.000007
    moon.shader.setUniformValue("pointLight.constant", 1.f);
    moon.shader.setUniformValue("pointLight.linear", 0.0014f);
    moon.shader.setUniformValue("pointLight.quadratic", 0.000007f);

    moon.shader.setUniformValue("pointLight.ambient", QVector3D(.2f, .2f, .2f));
    moon.shader.setUniformValue("pointLight.diffuse", QVector3D(.5f, .5f, .5f));
    moon.shader.setUniformValue("pointLight.specular", QVector3D(1.f, 1.f, 1.f));
    moon.VAO.bind();

    drawNode(moonModel, moon.loader.getNodeData().data(), QMatrix4x4(), moon);

    moon.VAO.release();
    moon.shader.release();

//    qDebug() << glGetError() ;

    QCursor c = window()->cursor();
    c.setPos(window()->mapToGlobal(QPoint(window()->width() / 2, window()->height() / 2)));
//    c.setShape(Qt::BlankCursor);
    window()->setCursor(c);
}
void Scene::animate()
{
    //! 近似：一年 360天 12个月 每月 30 天
    static float earthRotationAngle = 0, earthRevolutionAngle = 0;
    static float moonRotationAngle = 0, moonRevolutionAngle = 0;
    //! 太阳自转
    sunModel.rotate(10, QVector3D(0.f, 1.f, 0.f));

    //! 地球自转
    earthModel.setToIdentity();
    earthRotationAngle += 36;
    earthModel.rotate(earthRotationAngle, QVector3D(0.f, 1.f, 0.f));
    //! 地球公转
    QMatrix4x4 a;
    earthRevolutionAngle += 0.1;
    a.rotate(earthRevolutionAngle, QVector3D(0.f, 1.f, 0.f));
    a.translate(QVector3D(3.f, 0.f, 0.f));

    earthModel = a * earthModel;
    earthModel.scale(.5, .5, .5);

    //! 月亮自转
    moonModel.setToIdentity();
    moonRotationAngle += 1.2;
    moonModel.rotate(moonRotationAngle, QVector3D(0.f, 1.f, 0.f));
    //! 月亮公转
    QMatrix4x4 b;
    b = a;
    moonRevolutionAngle += 1.2;
    b.rotate(moonRevolutionAngle, QVector3D(0.f, 1.f, 0.f));
    b.translate(QVector3D(0.5f, 0.f, 0.f));

    moonModel = b * moonModel;
    moonModel.scale(.25, .25, .25);
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
// Rendering in selection mode

void Scene::renderSelection(void)
{

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    picking.shader.bind();

    picking.VAO.bind();
    QMatrix4x4 view;
    view.lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    picking.shader.setUniformValue("m_v", view);

    QMatrix4x4 projection;
    projection.perspective(m_zoom, float(window()->width()) / window()->height(), 0.1f, 100.f);
    picking.shader.setUniformValue("m_p", projection);

    picking.shader.setUniformValue("code", 1);
    drawNode(sunModel, picking.loader.getNodeData().data(), QMatrix4x4(), picking);

    picking.shader.setUniformValue("code", 2);

    drawNode(earthModel, picking.loader.getNodeData().data(), QMatrix4x4(), picking);

    picking.shader.setUniformValue("code", 3);
    drawNode(moonModel, picking.loader.getNodeData().data(), QMatrix4x4(), picking);

    picking.VAO.release();
    picking.shader.release();
    glClearColor(.0f, .0f, .0f, 1.f);
}
void Scene::processSelection(int xx, int yy)
{

    unsigned char res[4];
    GLint viewport[4];

    renderSelection();

    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(xx, viewport[3] - yy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);
    switch (res[0])
    {
        case 0:
            qDebug() << "Nothing Picked";
            sun.picked = 0;
            earth.picked = 0;
            moon.picked = 0;
            break;
        case 1:
            qDebug() << "Sun Picked";
            sun.picked = sun.picked ? 0 : 1;
            break;
        case 2:
            qDebug() << "Earth Picked";
            earth.picked = earth.picked ? 0 : 1;
            break;
        case 3:
            qDebug() << "Moon Picked";
            moon.picked = moon.picked ? 0 : 1;
            break;
    }

}
