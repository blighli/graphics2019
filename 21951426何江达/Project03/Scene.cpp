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

    connect(window, &SceneGLWindow::objRot,
            this, &Scene::setObjRot);
    connect(window, &SceneGLWindow::objChange,
            this, &Scene::setObjTran);

    connect(window, &SceneGLWindow::objScale,
            this, &Scene::setObjScal);

    connect(this, &Scene::objMod,
            window, &SceneGLWindow::setObjMod);
//    QTimer* timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, [ = ]() {animate();});
//    timer->start(150);
}

Scene::~Scene()
{

}
void Scene::createGeometry(QString path, object& s)
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
void Scene::drawNode(const QMatrix4x4& model, const Node* node, QMatrix4x4 parent, object& s)
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
//        qDebug() << "has tex" << m.material->hasTexture;
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
//                qDebug() << texture.path;
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

    nanosuit.shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/earth.vert");
    nanosuit.shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/earth.frag");
    nanosuit.shader.link();


    picking.shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/selection.vert");
    picking.shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/selection.frag");
    picking.shader.link();

    sun.shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/sun.vert");
    sun.shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/sun.frag");
    sun.shader.link();

    moon.shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/sun.vert");
    moon.shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/sun.frag");
    moon.shader.link();

    earth.shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/sun.vert");
    earth.shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/sun.frag");
    earth.shader.link();

    createGeometry("../Project03/res/sun1.obj", sun);
    createGeometry("../Project03/res/sphere.obj", picking);
    createGeometry("../Project03/nanosuit/nanosuit.obj", nanosuit);//with texture
    createGeometry("../Project03/res/moon1.obj", moon);
    createGeometry("../Project03/res/earth1.obj", earth);

    glEnable(GL_DEPTH_TEST);

    m_cameraPos   = QVector3D(0.0f, 0.0f,  3.0f);
    m_cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    m_cameraUp    = QVector3D(0.0f, 1.0f,  0.0f);

    sunModel.translate(QVector3D(.5, .5, .5));
    sunModel.scale(QVector3D(.2, .2, .2));
    lightmod = 0;

    moonModel.translate(QVector3D(-.5, .5, .5));
    moonModel.scale(QVector3D(.2, .2, .2));

    earthModel.translate(QVector3D(0, -.5, .5));
    earthModel.scale(QVector3D(.2, .2, .2));
}


void Scene::paint()
{
    glViewport(0, 0, window()->width(), window()->height());
    glClearColor(0.0f, 0.0f, 0.0f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sun.shader.bind();
    sun.shader.setUniformValue("viewPos", m_cameraPos);
    sun.VAO.bind();

    drawNode(sunModel, sun.loader.getNodeData().data(), QMatrix4x4(), sun);

    sun.VAO.release();
    sun.shader.release();

    moon.shader.bind();
    moon.shader.setUniformValue("viewPos", m_cameraPos);
    moon.VAO.bind();
    drawNode(moonModel, moon.loader.getNodeData().data(), QMatrix4x4(), moon);

    moon.VAO.release();
    moon.shader.release();

    earth.shader.bind();
    earth.shader.setUniformValue("viewPos", m_cameraPos);
    earth.VAO.bind();
    drawNode(earthModel, earth.loader.getNodeData().data(), QMatrix4x4(), earth);

    earth.VAO.release();
    earth.shader.release();

    emit objMod(bool(nanosuit.picked));

    nanosuit.shader.bind();
//    nanosuit.shader.setUniformValue("lightmod", lightmod);
    nanosuit.shader.setUniformValue("moon", moon.picked);
    nanosuit.shader.setUniformValue("sun", sun.picked);
    nanosuit.shader.setUniformValue("earth", earth.picked);

    nanosuit.shader.setUniformValue("viewPos", m_cameraPos);
//    nanosuit.shader.setUniformValue("viewPos", m_cameraPos);
    nanosuit.shader.setUniformValue("pointLight.position", QVector3D(-.5f, .5f, .5f)/*m_cameraPos*/);
    // 3250	1.0	0.0014	0.000007
    nanosuit.shader.setUniformValue("pointLight.constant", 1.f);
    nanosuit.shader.setUniformValue("pointLight.linear", 0.0014f);
    nanosuit.shader.setUniformValue("pointLight.quadratic", 0.000007f);

    nanosuit.shader.setUniformValue("pointLight.ambient", QVector3D(.2f, .2f, .2f));
    nanosuit.shader.setUniformValue("pointLight.diffuse", QVector3D(.7f, .7f, .7f));
    nanosuit.shader.setUniformValue("pointLight.specular", QVector3D(1.f, 1.f, 1.f));

    nanosuit.shader.setUniformValue("dirLight.direction", QVector3D(-.5, -.5, -.5));
    nanosuit.shader.setUniformValue("dirLight.ambient", QVector3D(.2f, .2f, .2f));
    nanosuit.shader.setUniformValue("dirLight.diffuse", QVector3D(.7f, .7f, .7f));
    nanosuit.shader.setUniformValue("dirLight.specular", QVector3D(1.f, 1.f, 1.f));

    nanosuit.shader.setUniformValue("spotLight.position", m_cameraPos);
    nanosuit.shader.setUniformValue("spotLight.direction", m_cameraFront);
    nanosuit.shader.setUniformValue("spotLight.constant", 1.f);
    nanosuit.shader.setUniformValue("spotLight.linear", 0.0014f);
    nanosuit.shader.setUniformValue("spotLight.cutOff", cosf(radians(12.5)));
    nanosuit.shader.setUniformValue("spotLight.outerCutOff", cosf(radians(17.5)));
    nanosuit.shader.setUniformValue("spotLight.quadratic", 0.000007f);
    nanosuit.shader.setUniformValue("spotLight.ambient", QVector3D(.2f, .2f, .2f));
    nanosuit.shader.setUniformValue("spotLight.diffuse", QVector3D(.7f, .7f, .7f));
    nanosuit.shader.setUniformValue("spotLight.specular", QVector3D(1.f, 1.f, 1.f));

    nanosuit.VAO.bind();
    drawNode(nanoModel, nanosuit.loader.getNodeData().data(), QMatrix4x4(), nanosuit);

    nanosuit.VAO.release();
    nanosuit.shader.release();

//    qDebug() << glGetError() ;
    if (!nanosuit.picked)
    {
        QCursor c = window()->cursor();
        c.setPos(window()->mapToGlobal(QPoint(window()->width() / 2, window()->height() / 2)));
        //    c.setShape(Qt::BlankCursor);
        window()->setCursor(c);
    }
}
void Scene::animate()
{

    //! wolf自转
    nanoModel.rotate(10, QVector3D(0.f, 1.f, 0.f));
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
    drawNode(nanoModel, picking.loader.getNodeData().data(), QMatrix4x4(), picking);

    picking.shader.setUniformValue("code", 2);
    drawNode(sunModel, picking.loader.getNodeData().data(), QMatrix4x4(), picking);

    picking.shader.setUniformValue("code", 3);
    drawNode(moonModel, picking.loader.getNodeData().data(), QMatrix4x4(), picking);

    picking.shader.setUniformValue("code", 4);
    drawNode(earthModel, picking.loader.getNodeData().data(), QMatrix4x4(), picking);

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
            nanosuit.picked = 0;
            break;
        case 1:
            qDebug() << "nanosuit Picked";
            nanosuit.picked = nanosuit.picked ? 0 : 1;
            break;
        case 2:
            qDebug() << "sun Picked";
            sun.picked = sun.picked ? 0 : 1;
            break;
        case 3:
            qDebug() << "moon Picked";
            moon.picked = moon.picked ? 0 : 1;
            break;
        case 4:
            qDebug() << "earth Picked";
            earth.picked = earth.picked ? 0 : 1;
            break;
    }
}
void Scene::setObjRot(float pitch, float yaw)
{
    static float lastp = 0, lasty = 0;
    nanoModel.rotate(pitch - lastp, QVector3D(1, 0, 0));
    nanoModel.rotate(yaw - lasty, QVector3D(0, 1, 0));
    lastp = pitch;
    lasty = yaw;
//    qDebug() << "rot";
}
void Scene::setObjTran(float x, float y, float z)
{
    nanoModel.translate(x, y, z);
//    lastStatus = nanoModel;
}
void Scene::setObjScal(float scal)
{
    nanoModel.scale(scal > 0 ? scal * 1.5 : -scal * 0.5);
    lastStatus = nanoModel;
//    qDebug() << "scale" << scal;
}
