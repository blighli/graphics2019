#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include "gameobject.h"
#include "spriterenderer.h"
#include "resourcemanager.h"
#include <QVector>

class GameLevel
{
public:
    GameLevel();
    QVector<GameObject> Bricks;
    // Loads level from file
    void Load(QString file, GLuint levelWidth, GLuint levelHeight);
    // Render level
    void Draw(SpriteRenderer &renderer);
    // Check if the level is completed (all non-solid tiles are destroyed)
    GLboolean IsCompleted();
private:
    // Initialize level from tile data
    void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif // GAMELEVEL_H
