#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLVertexArrayObject>
class SpriteRenderer
{
public:
    SpriteRenderer(QOpenGLShaderProgram &shader);
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(QOpenGLTexture &texture, QVector2D position, QVector2D size = QVector2D(10, 10), GLfloat rotate = 0.0f, QVector3D color = QVector3D(1.0f,1.0f,1.0f));
private:
    QOpenGLShaderProgram* shader;
    QOpenGLVertexArrayObject quadVAO;
    void initRenderData();
};

#endif // SPRITERENDERER_H
