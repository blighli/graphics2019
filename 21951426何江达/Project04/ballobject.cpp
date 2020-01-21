#include "ballobject.h"

BallObject::BallObject()
    : GameObject(), Radius(12.5f), Stuck(true)  { }

BallObject::BallObject(QVector2D pos, GLfloat radius, QVector2D velocity, QOpenGLTexture* sprite)
    :  GameObject(pos, QVector2D(radius * 2, radius * 2), sprite, QVector3D(1.0f,1,1), velocity), Radius(radius), Stuck(true) { }

QVector2D BallObject::Move(GLfloat dt, GLuint window_width)
{
    // If not stuck to player board
    if (!this->Stuck)
    {
        // Move the ball
        this->Position += this->Velocity * dt;
        // Then check if outside window bounds and if so, reverse velocity and restore at correct position
        if (this->Position.x() <= 0.0f)
        {
            this->Velocity.setX(-this->Velocity.x());
            this->Position.setX(0.0f);
        }
        else if (this->Position.x() + this->Size.x() >= window_width)
        {
            this->Velocity.setX(-this->Velocity.x());
            this->Position.setX(window_width - this->Size.x());
        }
        if (this->Position.y() <= 0.0f)
        {
            this->Velocity.setY(-this->Velocity.y());
            this->Position.setY(0.0f);
        }
    }
    return this->Position;
}

// Resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(QVector2D position, QVector2D velocity)
{
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
}
