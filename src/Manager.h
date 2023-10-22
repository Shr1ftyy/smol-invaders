#ifndef MANAGER
#define MANAGER

#include "Bullet.h"
#include "SimpleEnemy.h"
#include "FlyingEnemy.h"
#include "Player.h"
#include "Powerup.h"
#include "raymath.h"

#include <chrono>
#include <vector>
#include <unordered_map>

struct Vector2HashFunction
{
    size_t operator()(const Vector2 vec) const
    {
        size_t rowHash = std::hash<float>()(vec.x);
        size_t colHash = std::hash<float>()(vec.y) << 1;
        return rowHash ^ colHash;
    }
};

struct Vector2EqualityFunction
{
    bool operator()(const Vector2 vec1, const Vector2 vec2) const
    {
        return Vector2Equals(vec1, vec1);
    }
};

struct Manager
{
    // global game font
    Font gameFont = LoadFontEx("resources/fonts/CascadiaCode/CascadiaCode.ttf", 20, 0, 250);
    // screen dimensions
    int screenWidth;
    int screenHeight;
    // top left and bottom right of enemy ship formation
    // TODO: make the formation system more generalizable/better 
    Vector2 topLeft;
    Vector2 bottomRight;
    // game clock -> used for physics
    std::chrono::system_clock::time_point lastUpdateTime;
    std::chrono::system_clock::time_point lastDrawTime;
    // entity map
    EntityMap entities;
    // powerups that need to be spawned in the next frame
    std::vector<std::shared_ptr<Powerup>> powerupsToAdd;
    // formationPositions for enemies;
    std::vector<Vector2*> formationPositions;
    // assigned formations position of enemies
    std::unordered_map<unsigned int, Vector2*> assignedPositionMap;
    // occupied formation positions
    std::unordered_map<Vector2, bool, Vector2HashFunction, Vector2EqualityFunction> unavailableFormationPositions;
    // player score
    int score;
    // enemy texture
    Texture2D enemyTexture;
    // simple enemy fire sound
    Sound simpleEnemyFireSound;
    // enemy explosion sound
    Sound enemyExplosionSound;

    float formationUpdateRate = 2.0;
    float MAX_X_OFFSET = 100.0;
    float DELTA = 10.0; 
    float timeSinceLastFormationUpdate;
    
    Manager(int _screenWidth, int _screenHeight, Vector2 _topLeft, Vector2 _bottomRight, float _spacing);
    void addEntity(std::shared_ptr<Entity> _entity);
    void deleteEntity(EntityId _id);
    void update();
    void draw();
};

#endif // MANAGER
