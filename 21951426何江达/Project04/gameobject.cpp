#include "gameobject.h"

GameObject::GameObject()
    : Position(0, 0), Size(1, 1), Velocity(0.0f,0.0f), Color(1.0f,1.0f,1.0f), Rotation(0.0f), Sprite(nullptr), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(QVector2D pos, QVector2D size, QOpenGLTexture* sprite, QVector3D color, QVector2D velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false) { }
GameObject::~GameObject(){

}
void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(*this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}
