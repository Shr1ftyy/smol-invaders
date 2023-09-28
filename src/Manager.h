#ifndef MANAGER
#define MANAGER

#include "Bullet.h"
#include "SimpleEnemy.h"
#include "Player.h"
#include "Powerup.h"

#include <chrono>
#include <vector>

struct Manager
{
    int screenWidth;
    int screenHeight;
    // game clock -> used for physics
    std::chrono::system_clock::time_point lastUpdateTime;
    std::chrono::system_clock::time_point lastDrawTime;
    EntityMap entities;
    std::vector<Powerup*> powerupsToAdd;
    // EventMap events;

    Manager(int _screenWidth, int _screenHeight);
    void addEntity(Entity* _entity);
    void deleteEntity(EntityId _id);
    void update();
    void draw();
};

#endif // MANAGER