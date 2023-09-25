#include "Manager.h"
#include "Bullet.h"

Manager::Manager(int _screenWidth, int _screenHeight)
{
    screenWidth = _screenWidth;
    screenHeight = _screenHeight;
}

void Manager::addEntity(Entity* _entity)
{
    entities[_entity->id] = _entity;
}

void Manager::deleteEntity(EntityId _id)
{
    entities.erase(_id);
}

void Manager::update()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = now - lastUpdateTime;
    int dt = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    lastUpdateTime = now;

    for (auto entry : entities) {
        auto entity = entry.second;

        if (entity->type == EntityType::PLAYER_TYPE) {
            Player* player = static_cast<Player*>(entity);
            player->update(this, screenWidth, screenHeight, dt);
        }
        else if (entity->type == EntityType::PLAYER_BULLET) {
            Bullet* bullet = static_cast<Bullet*>(entity);
            bullet->update(this);
        }
        else {
            entity->update();
        }
    }
}

void Manager::draw()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = now - lastDrawTime;
    int dt = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    lastDrawTime = now;

    for (auto entry : entities) {
        auto entity = entry.second;
        if (entity->type == EntityType::PLAYER_BULLET) {
            Bullet* bullet = static_cast<Bullet*>(entity);
            bullet->draw(dt);
        }
        else {
            entity->draw();
        }
    }
}
