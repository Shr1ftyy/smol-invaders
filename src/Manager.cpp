#include "Manager.h"

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
    
    // decrease hp of enemies if colloding with player bullet
    for (auto entry : entities) 
    {
        auto entity = entry.second;
        if(entity->type == EntityType::ENEMY_TYPE)
        {
            Enemy* enemy = static_cast<Enemy*>(entity);

            // check if colliding with player bullets
            for (auto e: entities) {
                auto b = e.second;
                if(b->type == EntityType::PLAYER_BULLET) {
                    Bullet* bullet = static_cast<Bullet*>(b);
                    
                    Rectangle bulletHitbox = 
                    {
                        bullet->position.x - (bullet->hitboxDims.x)/2, 
                        bullet->position.y - (bullet->hitboxDims.y)/2,
                        bullet->hitboxDims.x,
                        bullet->hitboxDims.y
                    };
                    Rectangle enemyHitbox = 
                    {
                        enemy->position.x - (enemy->hitboxDims.x)/2, 
                        enemy->position.y - (enemy->hitboxDims.y)/2,
                        enemy->hitboxDims.x,
                        enemy->hitboxDims.y
                    };
                    
                    
                    if(CheckCollisionRecs(bulletHitbox, enemyHitbox))
                    {
                        enemy->hp -= bullet->dmg;
                    }
                }
            }
        }
    }

    for (auto entry : entities) {
        auto entity = entry.second;

        if (entity->type == EntityType::PLAYER_TYPE)
        {
            Player* player = static_cast<Player*>(entity);
            player->update(this, screenWidth, screenHeight, dt);
        }
        else if (entity->type == EntityType::PLAYER_BULLET)
        {
            Bullet* bullet = static_cast<Bullet*>(entity);
            bullet->update(this);
        }
        else if (entity->type == EntityType::ENEMY_TYPE)
        {
            Enemy* enemy = static_cast<Enemy*>(entity);
            enemy->update(this);
        }
        else
        {
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
