#ifndef BALLOBJECT_H
#define BALLOBJECT_H
#include <QOpenGLTexture>
#include "spriterenderer.h"
#include "gameobject.h"
#include <QVector2D>
#include <QVector3D>

// BallObject holds the state of the Ball object inheriting
// relevant state data from GameObject. Contains some extra
// functionality specific to Breakout's ball object that
// were too specific for within GameObject alone.
class BallObject : public GameObject
{
public:
    // Ball state
    GLfloat   Radius;
    GLboolean Stuck;
    // Constructor(s)
    BallObject();
    BallObject(QVector2D pos, GLfloat radius, QVector2D velocity, QOpenGLTexture* sprite);
    // Moves the ball, keeping it constrained within the window bounds (except bottom edge); returns new position
    QVector2D Move(GLfloat dt, GLuint window_width);
    // Resets the ball to original state with given position and velocity
    void      Reset(QVector2D position, QVector2D velocity);
};

#endif // BALLOBJECT_H
