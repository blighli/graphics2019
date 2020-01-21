#include "gamelevel.h"
#include <fstream>
#include <sstream>

GameLevel::GameLevel()
{

}

void GameLevel::Load(QString file, GLuint levelWidth, GLuint levelHeight)
{
    this->Bricks.clear();
    GLuint tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file.toStdString());
    std::vector<std::vector<GLuint>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // Read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<GLuint> row;
            while (sstream >> tileCode) // Read each word seperated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}
void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
}

GLboolean GameLevel::IsCompleted()
{
    for (GameObject &tile : this->Bricks)
        if (!tile.IsSolid && !tile.Destroyed)
            return GL_FALSE;
    return GL_TRUE;
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
    // Calculate dimensions
    GLuint height = tileData.size();
    GLuint width = tileData[0].size(); // Note we can index vector at [0] since this function is only called if height > 0
    GLfloat unit_width = levelWidth*1.0 / static_cast<GLfloat>(width), unit_height = levelHeight*1.0 / height;
    // Initialize level tiles based on tileData
    for (GLuint y = 0; y < height; ++y)
    {
        for (GLuint x = 0; x < width; ++x)
        {
            // Check block type from level data (2D level array)
            if (tileData[y][x] == 1) // Solid
            {
                QVector2D pos(unit_width * x, unit_height * y);
                QVector2D size(unit_width, unit_height);
                GameObject obj(pos, size, &ResourceManager::GetTexture("block_solid"), QVector3D(0.8f, 0.8f, 0.7f));
                obj.IsSolid = GL_TRUE;
                this->Bricks.push_back(obj);
            }
            else if (tileData[y][x] > 1)	// Non-solid; now determine its color based on level data
            {
                QVector3D color = QVector3D(1.0f,1,1); // original: white
                if (tileData[y][x] == 2)
                    color = QVector3D(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = QVector3D(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4)
                    color = QVector3D(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = QVector3D(1.0f, 0.5f, 0.0f);

                QVector2D pos(unit_width * x, unit_height * y);
                QVector2D size(unit_width, unit_height);
                this->Bricks.push_back(GameObject(pos, size, &ResourceManager::GetTexture("block"), color));
            }
        }
    }
}
