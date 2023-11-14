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
    // entities to delete on next update()
    std::vector<Entity*> deleteQueue;
    // powerups that need to be spawned in the next frame
    std::vector<std::shared_ptr<Powerup>> powerupsToAdd;
    // formationPositions for enemies;
    std::vector<Vector2*> formationPositions;
    // assigned formations position of enemies
    std::unordered_map<EntityId, int> assignedPositionMap;
    // occupied formation positions
    std::unordered_map<int, bool> unavailableFormationPositions;
    // player score
    int score;
    // player lives
    int lives;
    // enemy texture
    Texture2D enemyTexture;
    // simple enemy fire sound
    Sound simpleEnemyFireSound;
    // enemy explosion sound
    Sound enemyExplosionSound;
    // is the game over?
    bool gameOver;
    // is debugging
    bool DEBUG;

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
