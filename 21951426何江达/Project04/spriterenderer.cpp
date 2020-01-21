#include "spriterenderer.h"
#include <QMatrix4x4>
#include <QOpenGLBuffer>
SpriteRenderer::SpriteRenderer(QOpenGLShaderProgram &shader)
{
    this->shader=&shader;
    this->initRenderData();
}
SpriteRenderer::~SpriteRenderer(){
    this->quadVAO.destroy();
}

void SpriteRenderer::DrawSprite(QOpenGLTexture &texture, QVector2D position, QVector2D size , GLfloat rotate , QVector3D color){
    this->shader->bind();
    QMatrix4x4 model;
    model.translate(QVector3D(position,0.0f));

    model.translate(QVector3D(0.5f * size.x(), 0.5f * size.y(), 0.0f));
    model.rotate(rotate,QVector3D(0,0,1));
    model.translate(QVector3D(-.5f*size.x(), -0.5f * size.y(), 0.0f));

    model.scale(QVector3D(size,1));

    this->shader->setUniformValue("model",model);
    this->shader->setUniformValue("spriteColor",color);
    texture.bind(0);
    this->quadVAO.bind();
    glDrawArrays(GL_TRIANGLES,0,6);
    this->quadVAO.release();
}
void SpriteRenderer::initRenderData()
{
    QOpenGLBuffer VBO;
    GLfloat vertices[] = {
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    this->quadVAO.create();
    VBO.create();
    VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    VBO.bind();
    VBO.allocate(vertices,sizeof(vertices));

    this->quadVAO.bind();
    this->shader->enableAttributeArray(0);
    this->shader->setAttributeBuffer(0, GL_FLOAT, 0, 4);
    VBO.release();
    this->quadVAO.release();
}
