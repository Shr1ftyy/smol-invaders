#include "Manager.h"

#include <algorithm>
#include <chrono>

Manager::Manager(int _screenWidth, int _screenHeight)
{
    screenWidth = _screenWidth;
    screenHeight = _screenHeight;
    lastUpdateTime = std::chrono::system_clock::now();
    lastDrawTime = std::chrono::system_clock::now();
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
    // search for player entity
    Player* player;
    for(auto entry: entities)
    {
        auto entity = entry.second;
        if (entity->type == EntityType::PLAYER_TYPE)
        {
            player = static_cast<Player*>(entity);
        }
    }
    
    // decrease hp of enemies if colloding with player bullet
    for (auto entry : entities)
    {
        auto entity = entry.second;
        if (entity->type == EntityType::ENEMY_TYPE)
        {
            Enemy* enemy = static_cast<Enemy*>(entity);

            // check if colliding with player bullets
            for (auto e : entities)
            {
                auto b = e.second;
                if (b->type == EntityType::PLAYER_BULLET)
                {
                    Bullet* bullet = static_cast<Bullet*>(b);

                    Rectangle bulletHitbox =
                        {
                            bullet->position.x - (bullet->hitboxDims.x) / 2,
                            bullet->position.y - (bullet->hitboxDims.y) / 2,
                            bullet->hitboxDims.x,
                            bullet->hitboxDims.y
                        };
                    Rectangle enemyHitbox =
                        {
                            enemy->position.x - (enemy->hitboxDims.x) / 2,
                            enemy->position.y - (enemy->hitboxDims.y) / 2,
                            enemy->hitboxDims.x,
                            enemy->hitboxDims.y
                        };

                    if (CheckCollisionRecs(bulletHitbox, enemyHitbox))
                    {
                        enemy->hp -= bullet->dmg;
                        bullet->exploding = true;
                        bullet->hitboxDims = {0, 0};
                    }
                }
            }
        }
    }
    
    // activate powerups if colliding with player
    for(auto entry : entities)
    {
        auto entity = entry.second;
        if(entity->type ==  EntityType::POWERUP_TYPE)
        {
            Powerup* powerup = static_cast<Powerup*>(entity);
            
            Rectangle powerupHitbox = 
            {
                powerup->position.x - (powerup->hitboxDims.x) / 2,
                powerup->position.y - (powerup->hitboxDims.y) / 2,
                powerup->hitboxDims.x,
                powerup->hitboxDims.y
            };
            Rectangle playerHitbox = 
            {
                player->position.x - (player->hitboxDims.x) / 2,
                player->position.y - (player->hitboxDims.y) / 2,
                player->hitboxDims.x,
                player->hitboxDims.y
            };
            
            if (CheckCollisionRecs(powerupHitbox, playerHitbox))
            {
                player->powerUp(powerup);
                powerup->destroyed = true;
            }
        }
    }

    // check for destroyed bullets and enemies - I NEED MORE BULLETS
    for (auto it = begin(entities); it != end(entities);)
    {
        Entity* entity = it->second;
        if (entity->type == EntityType::PLAYER_BULLET)
        {
            Bullet* bullet = static_cast<Bullet*>(entity);
            if (bullet->destroyed)
            {
                entities.erase(it++); // previously this was something like m_map.erase(it++);
            }
            else
                ++it;
            continue;
        }
        else if (entity->type == EntityType::ENEMY_TYPE)
        {
            Enemy* enemy = static_cast<Enemy*>(entity);
            if (enemy->destroyed)
            {
                entities.erase(it++); // previously this was something like m_map.erase(it++);
            }
            else
                ++it;
            continue;
        } else if (entity->type ==  EntityType::POWERUP_TYPE)
        {
            if (entity->destroyed)
            {
                entities.erase(it++); // previously this was something like m_map.erase(it++);
            }
            else
                ++it;
            continue;
        }
        
        ++it;
    }
    
    auto now = std::chrono::system_clock::now();
    auto elapsed = now - lastUpdateTime;
    float dt = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    lastUpdateTime = now;

    for (auto entry : entities)
    {
        auto entity = entry.second;

        if (entity->type == EntityType::PLAYER_TYPE)
        {
            Player* player = static_cast<Player*>(entity);
            player->update(this, screenWidth, screenHeight, dt);
        }
        else if (entity->type == EntityType::PLAYER_BULLET)
        {
            Bullet* bullet = static_cast<Bullet*>(entity);
            bullet->update(this, dt);
        }
        else if (entity->type == EntityType::ENEMY_TYPE)
        {
            Enemy* enemy = static_cast<Enemy*>(entity);
            
            if (enemy->enemyType == EnemyType::SIMPLE)
            {
                SimpleEnemy* simpleEnemy = static_cast<SimpleEnemy*>(enemy);
                Powerup* powerup = simpleEnemy->update(this);
                if (powerup)
                {
                    powerupsToAdd.push_back(powerup);
                }   
            }
            
        }
        else if (entity->type == EntityType::POWERUP_TYPE)
        {
            Powerup* powerup = static_cast<Powerup*>(entity);
            powerup->update(this, dt);
        }
        else
        {
            entity->update();
        }
    }
    
    // spawn powerups 
    while(!powerupsToAdd.empty())
    {
        addEntity(powerupsToAdd[powerupsToAdd.size()-1]);
        powerupsToAdd.pop_back();
    }
    
}

void Manager::draw()
{
    auto now = std::chrono::system_clock::now();
    auto elapsed = now - lastDrawTime;
    float dt = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    lastDrawTime = now;

    for (auto entry : entities)
    {
        auto entity = entry.second;
        if (entity->type == EntityType::PLAYER_BULLET)
        {
            Bullet* bullet = static_cast<Bullet*>(entity);
            bullet->draw(dt);
        }
        else if (entity->type == EntityType::ENEMY_TYPE)
        {
            Enemy* enemy = static_cast<Enemy*>(entity);
            enemy->draw(dt);
        }
        else if (entity->type == EntityType::PLAYER_TYPE)
        {
            entity->draw();
        }
        else if (entity->type == EntityType::POWERUP_TYPE)
        {
            entity->draw();
        }
    }
}
