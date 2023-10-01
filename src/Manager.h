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
    Font gameFont = LoadFontEx("resources/fonts/CascadiaCode/CascadiaCode.ttf", 20, 0, 250);
    int screenWidth;
    int screenHeight;
    Vector2 topLeft;
    Vector2 bottomRight;
    // game clock -> used for physics
    std::chrono::system_clock::time_point lastUpdateTime;
    std::chrono::system_clock::time_point lastDrawTime;
    EntityMap entities;
    std::vector<Powerup*> powerupsToAdd;
    // formationPositions for enemies;
    std::vector<Vector2*> formationPositions;
    // assigned formations position of enemies
    std::unordered_map<unsigned int, Vector2*> assignedPositionMap;
    // available formation positions
    std::unordered_map<Vector2, bool, Vector2HashFunction, Vector2EqualityFunction> unavailableFormationPositions;
    float formationUpdateRate = 2.0;
    float MAX_X_OFFSET = 100.0;
    float DELTA = 10.0; 
    float timeSinceLastFormationUpdate;
    
    Manager(int _screenWidth, int _screenHeight, Vector2 _topLeft, Vector2 _bottomRight, float _spacing);
    void addEntity(Entity* _entity);
    void deleteEntity(EntityId _id);
    void update();
    void draw();
};

#endif // MANAGER