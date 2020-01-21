#ifndef GAME_H
#define GAME_H
#include "abstractglscene.h"
#include "sceneglwindow.h"
#include <QVector2D>
#include "gamelevel.h"
#include "gameobject.h"
#include "ballobject.h"
// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Represents the four possible (collision) directions
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
// Defines a Collision typedef that represents collision data
typedef std::tuple<GLboolean, Direction, QVector2D> Collision; // <collision?, what direction?, difference vector center - closest point>


// Initial size of the player paddle
const QVector2D PLAYER_SIZE(100, 20);
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);
// Initial velocity of the Ball
const QVector2D INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;

class SceneGLWindow;
class Game:public QObject, public AbstractGLScene
{
    Q_OBJECT
public:
    Game(SceneGLWindow* window);
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void initialize();
    void paint();
public:
    // Game state
    GameState              State;
    std::vector<GameLevel> Levels;
    GLuint                 Level;
    // Constructor/Destructor

    // GameLoop
    // complete in sceneglwindow
    void ProcessInput(Qt::Key k);
    void UpdateGame(GLfloat dt);
    void Render();
    void Init();
    void DoCollisions();
    // Reset
    void ResetLevel();
    void ResetPlayer();
private:
    float deltat=0.0075;

};

#endif // GAME_H
