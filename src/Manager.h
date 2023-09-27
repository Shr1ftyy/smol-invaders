#ifndef MANAGER
#define MANAGER

#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include <chrono>

struct Manager
{
    int screenWidth;
    int screenHeight;
    // game clock -> used for physics
    std::chrono::system_clock::time_point lastUpdateTime;
    std::chrono::system_clock::time_point lastDrawTime;
    EntityMap entities;
    // EventMap events;

    Manager(int _screenWidth, int _screenHeight);
    void addEntity(Entity* _entity);
    void deleteEntity(EntityId _id);
    void update();
    void draw();
};

#endif // MANAGER