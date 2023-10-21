#include "Manager.h"
#include "helpers.h"

#include <algorithm>
#include <chrono>
#include <iostream>

Manager::Manager(int _screenWidth, int _screenHeight, Vector2 _topLeft, Vector2 _bottomRight, float _spacing)
{
    screenWidth = _screenWidth;
    screenHeight = _screenHeight;
    lastUpdateTime = std::chrono::system_clock::now();
    lastDrawTime = std::chrono::system_clock::now();
    
    topLeft = _topLeft;
    bottomRight = _bottomRight;
    formationPositions = generateMeshGrid(_topLeft, _bottomRight, _spacing);
    
    timeSinceLastFormationUpdate = 0;
}

void Manager::addEntity(Entity* _entity)
{    
    if (_entity->type == EntityType::ENEMY_TYPE)
    {
        if(assignedPositionMap.size() < formationPositions.size())
        {
            for(Vector2* pos : formationPositions)
            {
                
                if(unavailableFormationPositions.find((*pos)) == unavailableFormationPositions.end())
                {
                    // add enemy into formation
                    assignedPositionMap[_entity->id] = pos;
                    unavailableFormationPositions[(*pos)] = true;
                    // TODO: DO NOT FORCE POSITION WHEN SPAWNING?!!?!??!!?!?!
                    _entity->position = (*pos);
                    
                    entities[_entity->id] = _entity;
                    break;
                }
            }
        }
    } else 
    {
        entities[_entity->id] = _entity;
    }
}

// TODO: WIP
void Manager::deleteEntity(EntityId _id)
{
    // remove enemy from formation
    auto entity = entities[_id];
    if (entity->type == EntityType::ENEMY_TYPE)
    {
        Vector2* pos = assignedPositionMap[_id];
        unavailableFormationPositions.erase((*pos));
        assignedPositionMap.erase(_id);
    }
    
    eraseFromMap<EntityMap*, EntityId>(&entities, _id);
    // entities.erase(_id);
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
    
    // basic respawning loop
    // if((assignedPositionMap.size() < formationPositions.size()) && (float)rand()/RAND_MAX <= 0.05)
    // {
    //     std::cout << "spawning more enemies..." << std::endl;
    //     const char* enemySpriteSheetLocation = "./resources/textures/enemies.png";
        
    //     const char* enemyExplosionSoundLocation = "./resources/sounds/explosion2.wav";
    //     const char* simpleEnemyFireSoundLocation = "./resources/sounds/simpleEnemy_shoot.wav";
        
    //     const char* powerupSoundLocation = "./resources/sounds/powerUp.wav";
        
    //     Texture2D enemyTexture = LoadTexture(enemySpriteSheetLocation);
        
    //     Wave simpleEnemyFireWave = LoadWave(simpleEnemyFireSoundLocation);
    //     Sound simpleEnemyFireSound = LoadSoundFromWave(simpleEnemyFireWave);
        
    //     Wave enemyExplosionWave = LoadWave(enemyExplosionSoundLocation);
    //     Sound enemyExplosionSound = LoadSoundFromWave(enemyExplosionWave);
        
    //     float xChance = ((float)rand()/RAND_MAX);
    //     int randX = xChance <= 0.5 ? 0 : screenWidth;
    //     float yChance = ((float)rand()/RAND_MAX);
    //     int randY = yChance * 0.5 * screenHeight;
        
    //     std::cout << randX << "," << randY << std::endl;
        
    //     SimpleEnemy* enemy = new SimpleEnemy
    //     (
    //      enemyTexture, 
    //      enemyExplosionSound, 
    //      simpleEnemyFireSound,
    //      {0.0, 0.0},
    //      {96, 0},
    //      {32.0,32.0},
    //      {32.0, 32.0}, 
    //      {0.0, 1024.0}, 
    //      {32.0, 0.0}, 
    //      3, 
    //      2.0, 
    //      {32.0, 32.0}, 
    //      {50.0, 50.0}, 
    //      {32.0, 32.0}, 
    //      {50.0, 50.0}, 
    //      3, 
    //      {32.0, 0}, 
    //      6.0, 
    //      {50.0, 50.0}, 
    //      {(float)randX, (float)randY}, 
    //      10,
    //      50,
    //      0.2
    //     );
        
    //     FlyingEnemy* flyingEnemy = new FlyingEnemy
    //     (
    //      enemyTexture, 
    //      enemyExplosionSound, 
    //      simpleEnemyFireSound,
    //      {0.0, 32.0},
    //      {96, 32.0},
    //      {32.0,32.0},
    //      {32.0, 32.0}, 
    //      {0.0, 1152.0}, 
    //      {32.0, 0.0}, 
    //      3, 
    //      2.0, 
    //      {32.0, 32.0}, 
    //      {50.0, 50.0}, 
    //      {32.0, 32.0}, 
    //      {50.0, 50.0}, 
    //      3, 
    //      {32.0, 0}, 
    //      6.0, 
    //      {50.0, 50.0}, 
    //      {(float)randX, (float)randY}, 
    //      10,
    //      50,
    //      0.2
    //     );
        
    //     enemy->resettingPosition = true;
    //     flyingEnemy->resettingPosition = true;
    //     Entity* newEnemy = static_cast<Entity*>(enemy);
    //     Entity* newFlyingEnemy = static_cast<Entity*>(flyingEnemy);
        
    //     if((float)rand()/RAND_MAX <= 0.5)
    //     {
    //         addEntity(newEnemy);
    //         entities[newEnemy->id]->position = {(float)randX, (float)randY};
    //     }
    //     else
    //     {
    //         addEntity(newFlyingEnemy);
    //         entities[newFlyingEnemy->id]->position = {(float)randX, (float)randY};
    //     }
    // }
    
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
                    Rectangle prevBulletHitbox =
                    {
                        bullet->prevPosition.x - (bullet->hitboxDims.x) / 2,
                        bullet->prevPosition.y - (bullet->hitboxDims.y) / 2,
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
                    
                    if (
                        !CheckCollisionRecs(prevBulletHitbox, enemyHitbox) && 
                        CheckCollisionRecs(bulletHitbox, enemyHitbox)
                        )
                    {
                        enemy->hp -= bullet->dmg;
                        bullet->exploding = true;
                        for(auto p: player->activePowerups)
                        {
                            auto powerupType = p.first;
                            
                            // check if player has piercing powerup active
                            if(powerupType == PowerupType::PIERCING)
                            {
                                bullet->exploding=false;
                            }
                        }
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
    

    // TODO: use smart pointers instead of raw pointers going forward
    // TODO: (eventually) rewrite everything to support smart pointers :)
    // check for destroyed bullets and enemies - I NEED MORE BULLETS
    for (auto it = begin(entities); it != end(entities);)
    {
        Entity* entity = it->second;
        if (entity->type == EntityType::PLAYER_BULLET
            || entity->type == EntityType::ENEMY_BULLET)
        {
            Bullet* bullet = static_cast<Bullet*>(entity);
            if (bullet->destroyed)
            {
                entities.erase(it++);
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
                entities.erase(it++);
                Vector2* pos = assignedPositionMap[entity->id];
                unavailableFormationPositions.erase((*pos));
                assignedPositionMap.erase(entity->id);
            }
            else
                ++it;
            continue;
        } else if (entity->type ==  EntityType::POWERUP_TYPE)
        {
            if (entity->destroyed)
            {
                entities.erase(it++);
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
    timeSinceLastFormationUpdate += dt;
    
    if (timeSinceLastFormationUpdate > (1000.0/formationUpdateRate))
    {
        timeSinceLastFormationUpdate = 0;
        // TODO: FIND A BETTER WAY TO DO THIS
        if(DELTA <= 0)
        {
            if(formationPositions[0]->x > topLeft.x - MAX_X_OFFSET)
            {
                for(Vector2* pos: formationPositions)
                {
                    unavailableFormationPositions.erase((*pos));
                    pos->x += DELTA;   
                    unavailableFormationPositions[(*pos)] = true;
                }
                
            }
            else
            {
                DELTA = -DELTA;
            }
        }
        else
        {
            Vector2* maxPos = new Vector2;
            maxPos->x = 0; 
            maxPos->y = 0;
            
            for(auto p : formationPositions)
            {
                if(p->x > maxPos->x)
                {
                    maxPos = p;
                }
            }
            
            if(maxPos->x <= bottomRight.x + MAX_X_OFFSET)
            {
                for(Vector2* pos : formationPositions)
                {
                    unavailableFormationPositions.erase((*pos));
                    pos->x += DELTA;   
                    unavailableFormationPositions[(*pos)] = true;
                }
                
            }
            else
            {
                DELTA = -DELTA;
            }
        }
        
    }
    
    for (auto entry : entities)
    {
        auto entity = entry.second;
        
        if (entity->type == EntityType::PLAYER_TYPE)
        {
            Player* player = static_cast<Player*>(entity);
            player->update(this, screenWidth, screenHeight, dt);
        }
        else if (entity->type == EntityType::PLAYER_BULLET
                 || entity->type == EntityType::ENEMY_BULLET)
        {
            Bullet* bullet = static_cast<Bullet*>(entity);
            bullet->update(this, dt);
        }
        else if (entity->type == EntityType::ENEMY_TYPE)
        {
            Enemy* enemy = static_cast<Enemy*>(entity);
            
            // if (!enemy->attacking && !enemy->resettingPosition)
            // {
            //     enemy->position = (*assignedPositionMap[enemy->id]);
            // }
            
            if (enemy->enemyType == EnemyType::SIMPLE)
            {
                SimpleEnemy* simpleEnemy = static_cast<SimpleEnemy*>(enemy);
                Powerup* powerup = simpleEnemy->update(this, dt);
                if (powerup)
                {
                    powerupsToAdd.push_back(powerup);
                }   
            }
            else if (enemy->enemyType == EnemyType::FLYING)
            {
                FlyingEnemy* flyingEnemy = static_cast<FlyingEnemy*>(enemy);
                Powerup* powerup = flyingEnemy->update(this, dt);
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
        if (entity->type == EntityType::PLAYER_BULLET
            || entity->type == EntityType::ENEMY_BULLET)
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
