#ifndef MANAGER
#define MANAGER
#include "Player.h"
#include <string>

struct Manager
{
    int screenWidth;
    int screenHeight;
    // game clock -> used for physics
    std::chrono::steady_clock::time_point lastUpdateTime;
    std::chrono::steady_clock::time_point lastDrawTime;
    EntityMap entities;
    // EventMap events;

    Manager(int _screenWidth, int _screenHeight);
    void addEntity(Entity* _entity);
    void deleteEntity(EntityId _id);
    void update();
    void draw();
};

#endif // MANAGER