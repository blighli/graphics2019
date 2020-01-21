#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <QOpenGLTexture>
#include <spriterenderer.h>
#include <QVector2D>
#include <QVector3D>

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:

    QVector2D Position, Size, Velocity;
    QVector3D   Color;
    GLfloat     Rotation;
    GLboolean   IsSolid;
    GLboolean   Destroyed;

    QOpenGLTexture* Sprite;

    GameObject();
    ~GameObject();
    GameObject(QVector2D pos, QVector2D size, QOpenGLTexture* sprite, QVector3D color = QVector3D(1.0f,1.0f,1.0f), QVector2D velocity = QVector2D(0.0f, 0.0f));
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer);
};

#endif // GAMEOBJECT_H
