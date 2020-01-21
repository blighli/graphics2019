#include "game.h"
#include "resourcemanager.h"
#include "spriterenderer.h"
#include "ballobject.h"
#include <algorithm>
// Game-related State data
SpriteRenderer  *Renderer;
GameObject      *Player;
BallObject      *Ball;

Game::Game(SceneGLWindow* window):
    AbstractGLScene(window), State(GAME_ACTIVE)
{
    window->setScene(this);
    connect(window, &SceneGLWindow::keySiginal,
            this, &Game::ProcessInput);

}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ball;
}
void Game::initialize()
{
    AbstractGLScene::initialize();
    glViewport(0,0,window()->width(),window()->height());
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    Init();
}
void Game::Init()
{
    ResourceManager::LoadShader("../2D-Breakout/shaders/sprite.vert","../2D-Breakout/shaders/sprite.frag",nullptr,"sprite");
    QMatrix4x4 projection;
    projection.ortho(0,this->window()->width(),this->window()->height(),0,-1,1);
    ResourceManager::GetShader("sprite").bind();
    ResourceManager::GetShader("sprite").setUniformValue("sprite",0);
    ResourceManager::GetShader("sprite").setUniformValue("projection",projection);

    //    ResourceManager::LoadTexture("../2D-Breakout/textures/awesomeface.png",GL_TRUE,"face");
    //    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // Load textures
    ResourceManager::LoadTexture("../2D-Breakout/textures/background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("../2D-Breakout/textures/awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("../2D-Breakout/textures/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("../2D-Breakout/textures/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("../2D-Breakout/textures/paddle.png", true, "paddle");
    // Set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // Load levels
    GameLevel one; one.Load("../2D-Breakout/levels/one.lvl", this->window()->width(), this->window()->height() * 0.5);
    GameLevel two; two.Load("../2D-Breakout/levels/two.lvl", this->window()->width(), this->window()->height() * 0.5);
    GameLevel three; three.Load("../2D-Breakout/levels/three.lvl", this->window()->width(), this->window()->height() * 0.5);
    GameLevel four; four.Load("../2D-Breakout/levels/four.lvl", this->window()->width(), this->window()->height() * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    // Configure game objects
    QVector2D playerPos = QVector2D(this->window()->width() / 2 - PLAYER_SIZE.x() / 2, this->window()->height() - PLAYER_SIZE.y());
    Player = new GameObject(playerPos, PLAYER_SIZE, &ResourceManager::GetTexture("paddle"));

    QVector2D ballPos = playerPos + QVector2D(PLAYER_SIZE.x() / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
                          &ResourceManager::GetTexture("face"));
}
// 使用定时器更新游戏时间
void Game::UpdateGame(GLfloat dt)
{
    Ball->Move(dt, this->window()->width());
    // Check for collisions
    this->DoCollisions();
    // Check loss condition
    if (Ball->Position.y() >= this->window()->height()) // Did ball reach bottom edge?
    {
        qDebug()<<Ball->Position.y();
        this->ResetLevel();
        this->ResetPlayer();
    }
}


void Game::ProcessInput(Qt::Key k)
{
    qDebug()<<k;
    if (this->State == GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_VELOCITY * deltat;
        // Move playerboard
        if (k==Qt::Key_A)
        {
            qDebug()<<Player->Position.x();
            if (Player->Position.x() >= 0)
            {
                Player->Position.setX(Player->Position.x()- velocity);
                if (Ball->Stuck)
                    Ball->Position.setX(Ball->Position.x()-velocity);
            }
        }
        if (k==Qt::Key_D)
        {
            if (Player->Position.x() <= this->window()->width() - Player->Size.x())
            {
                Player->Position.setX( Player->Position.x()+ velocity);
                if (Ball->Stuck)
                    Ball->Position.setX( Ball->Position.x()+ velocity);
            }
        }
        if (k==Qt::Key_Space)
            Ball->Stuck = false;
    }
}
// 使用定时器定时渲染
void Game::paint()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    UpdateGame(0.015);
    Render();
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // Draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), QVector2D(0, 0), QVector2D(this->window()->width(), this->window()->height()), 0.0f);
        // Draw level
        this->Levels[this->Level].Draw(*Renderer);
        // Draw player
        Player->Draw(*Renderer);
        Ball->Draw(*Renderer);
    }
}
QVector2D clamp(QVector2D value, QVector2D min, QVector2D max) ;
GLboolean CheckCollision(GameObject &one, GameObject &two);
Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(QVector2D closest);

GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // Collision x-axis?
    bool collisionX = one.Position.x() + one.Size.x() >= two.Position.x() &&
        two.Position.x() + two.Size.x() >= one.Position.x();
    // Collision y-axis?
    bool collisionY = one.Position.y() + one.Size.y() >= two.Position.y() &&
        two.Position.y() + two.Size.y() >= one.Position.y();
    // Collision only if on both axes
    return collisionX && collisionY;
}

Collision CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
{
    // Get center point circle first
    QVector2D center(one.Position.x() + one.Radius,one.Position.y() + one.Radius);
    // Calculate AABB info (center, half-extents)
    QVector2D aabb_half_extents(two.Size.x() / 2, two.Size.y() / 2);
    QVector2D aabb_center(two.Position.x() + aabb_half_extents.x(), two.Position.y() + aabb_half_extents.y());
    // Get difference vector between both centers
    QVector2D difference = center - aabb_center;
    QVector2D clamped = clamp(difference, -aabb_half_extents, aabb_half_extents);
    // Now that we know the the clamped values, add this to AABB_center and we get the value of box closest to circle
    QVector2D closest = aabb_center + clamped;
    // Now retrieve vector between center circle and closest point AABB and check if length < radius
    difference = closest - center;

    if (difference.length() < one.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
        return std::make_tuple(GL_FALSE, UP, QVector2D(0, 0));
}

// Calculates which direction a vector is facing (N,E,S or W)
Direction VectorDirection(QVector2D target)
{
    QVector2D compass[] = {
        QVector2D(0.0f, 1.0f),	// up
        QVector2D(1.0f, 0.0f),	// right
        QVector2D(0.0f, -1.0f),	// down
        QVector2D(-1.0f, 0.0f)	// left
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        target.normalize();
        GLfloat dot_product = QVector2D::dotProduct(target,compass[i]);//glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}
void Game::DoCollisions()
{
    for (GameObject &box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision)) // If collision is true
            {
                // Destroy block if not solid
                if (!box.IsSolid)
                    box.Destroyed = GL_TRUE;
                // Collision resolution
                Direction dir = std::get<1>(collision);
                QVector2D diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) // Horizontal collision
                {
                    Ball->Velocity.setX(-Ball->Velocity.x()); // Reverse horizontal velocity
                    // Relocate
                    GLfloat penetration = Ball->Radius - std::abs(diff_vector.x());
                    if (dir == LEFT)
                        Ball->Position.setX(Ball->Position.x() + penetration); // Move ball to right
                    else
                        Ball->Position.setX(Ball->Position.x() - penetration); // Move ball to left;
                }
                else // Vertical collision
                {
                    Ball->Velocity.setY(-Ball->Velocity.y()); // Reverse vertical velocity
                    // Relocate
                    GLfloat penetration = Ball->Radius - std::abs(diff_vector.y());
                    if (dir == UP)
                        Ball->Position.setY(Ball->Position.y() - penetration); // Move ball bback up
                    else
                        Ball->Position.setY(Ball->Position.y() + penetration); // Move ball back down
                }
            }
        }
    }
    // Also check collisions for player pad (unless stuck)
    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // Check where it hit the board, and change velocity based on where it hit the board
        GLfloat centerBoard = Player->Position.x() + Player->Size.x() / 2;
        GLfloat distance = (Ball->Position.x() + Ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Player->Size.x() / 2);
        // Then move accordingly
        GLfloat strength = 2.0f;
        QVector2D oldVelocity = Ball->Velocity;
        Ball->Velocity.setX( INITIAL_BALL_VELOCITY.x() * percentage * strength);
        //Ball->Velocity.y = -Ball->Velocity.y;
        Ball->Velocity.normalize();
        Ball->Velocity.setX(Ball->Velocity.x() * oldVelocity.length()); // Keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
        Ball->Velocity.setY(Ball->Velocity.y() * oldVelocity.length());
        // Fix sticky paddle
        Ball->Velocity.setY(-1 * abs(Ball->Velocity.y()));
    }
}
QVector2D clamp(QVector2D value, QVector2D min, QVector2D max) {
    return QVector2D(std::max(min.x(), std::min(max.x(), value.x())),std::max(min.y(), std::min(max.y(), value.y())));
}
void Game::ResetLevel()
{
    if (this->Level == 0)this->Levels[0].Load("../2D-Breakout/levels/one.lvl", this->window()->width(), this->window()->height() * 0.5f);
    else if (this->Level == 1)
        this->Levels[1].Load("../2D-Breakout/levels/two.lvl", this->window()->width(), this->window()->height() * 0.5f);
    else if (this->Level == 2)
        this->Levels[2].Load("../2D-Breakout/levels/three.lvl", this->window()->width(), this->window()->height() * 0.5f);
    else if (this->Level == 3)
        this->Levels[3].Load("../2D-Breakout/levels/four.lvl", this->window()->width(), this->window()->height() * 0.5f);
}

void Game::ResetPlayer()
{
    // Reset player/ball stats
    Player->Size = PLAYER_SIZE;
    Player->Position = QVector2D(this->window()->width() / 2 - PLAYER_SIZE.x() / 2, this->window()->height() - PLAYER_SIZE.y());
    Ball->Reset(Player->Position + QVector2D(PLAYER_SIZE.x() / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}
