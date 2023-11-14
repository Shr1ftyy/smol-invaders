#include "Manager.h"
#include "helpers.h"

#include <fmt/core.h>

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
    score = 0;
    lives = 3;

    const char* enemySpriteSheetLocation = "./resources/textures/enemies.png";
 
    const char* enemyExplosionSoundLocation = "./resources/sounds/explosion2.wav";
    const char* simpleEnemyFireSoundLocation = "./resources/sounds/simpleEnemy_shoot.wav";
 
    const char* powerupSoundLocation = "./resources/sounds/powerUp.wav";
 
    enemyTexture = LoadTexture(enemySpriteSheetLocation);
 
    Wave simpleEnemyFireWave = LoadWave(simpleEnemyFireSoundLocation);
    simpleEnemyFireSound = LoadSoundFromWave(simpleEnemyFireWave);
 
    Wave enemyExplosionWave = LoadWave(enemyExplosionSoundLocation);
    enemyExplosionSound = LoadSoundFromWave(enemyExplosionWave);
    gameOver = false;
}

// TODO: why do some formation positions end up not being filled?
void Manager::addEntity(std::shared_ptr<Entity> _entity)
{    
    if (_entity->type == EntityType::ENEMY_TYPE)
    {
        if(assignedPositionMap.size() < formationPositions.size())
        {
            for(int p = 0; p < formationPositions.size(); p++)
            {
                
                if(!unavailableFormationPositions[p])
                {
                    // add enemy into formation
                    assignedPositionMap[_entity->id] = p;
                    unavailableFormationPositions[p] = true;
                    _entity->position = (*formationPositions[p]);
                    
                    entities[_entity->id] = _entity;
                    // std::cout << "assigned position to enemy!!!" << std::endl;
                    break;
                }
            }
            // std::cout << "position unavailable... #1" << std::endl;
        }
        // std::cout << "position unavailable... #2" << std::endl;
    } else 
    {
        // std::cout << "diff entity... #3" << std::endl;
        entities[_entity->id] = _entity;
    }
}

// TODO: why do some formation positions end up not being filled?
void Manager::deleteEntity(EntityId _id)
{
    // remove enemy from formation
    auto entity = entities[_id];
    if (entity->type == EntityType::ENEMY_TYPE)
    {
        int pos = assignedPositionMap[_id];
        unavailableFormationPositions.erase(pos);
        assignedPositionMap.erase(_id);
        unavailableFormationPositions[pos] = false;
    }
    
    entities.erase(_id);
}

void Manager::update()
{
    // search for player entity
    std::shared_ptr<Player> player;
    for (auto entry: entities)
    {
        auto entity = entry.second;
        if (entity->type == EntityType::PLAYER_TYPE)
        {
            player = std::static_pointer_cast<Player>(entity);
        }
    }
    
    // basic respawning loop
    if ((assignedPositionMap.size() < formationPositions.size()) && (float)rand()/RAND_MAX <= 0.05)
    {
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count() << std::endl;
        std::cout << "spawning more" << std::endl;
        float xChance = ((float)rand()/RAND_MAX);
        int randX = xChance <= 0.5 ? 0 + 5 : screenWidth - 5;
        float yChance = ((float)rand()/RAND_MAX);
        int randY = yChance * 0.5 * screenHeight - 5;
     
        std::shared_ptr<SimpleEnemy> enemy = std::shared_ptr<SimpleEnemy>(new SimpleEnemy
        (
         enemyTexture, 
         enemyExplosionSound, 
         simpleEnemyFireSound,
         {0.0, 0.0},
         {96, 0},
         {32.0,32.0},
         {32.0, 32.0}, 
         {0.0, 1024.0}, 
         {32.0, 0.0}, 
         3, 
         2.0, 
         {32.0, 32.0}, 
         {50.0, 50.0}, 
         {32.0, 32.0}, 
         {50.0, 50.0}, 
         3, 
         {32.0, 0}, 
         6.0, 
         {50.0, 50.0}, 
         {(float)randX, (float)randY}, 
         10,
         50,
         0.2
        ));
     
        std::shared_ptr<FlyingEnemy> flyingEnemy = std::shared_ptr<FlyingEnemy>(new FlyingEnemy
        (
         enemyTexture, 
         enemyExplosionSound, 
         simpleEnemyFireSound,
         {0.0, 32.0},
         {96, 32.0},
         {32.0,32.0},
         {32.0, 32.0}, 
         {0.0, 1152.0}, 
         {32.0, 0.0}, 
         3, 
         2.0, 
         {32.0, 32.0}, 
         {50.0, 50.0}, 
         {32.0, 32.0}, 
         {50.0, 50.0}, 
         3, 
         {32.0, 0}, 
         6.0, 
         {50.0, 50.0}, 
         {(float)randX, (float)randY}, 
         10,
         50,
         0.2
        ));
     
        enemy->resettingPosition = true;
        flyingEnemy->resettingPosition = true;
        std::shared_ptr<Entity> newEnemy = std::static_pointer_cast<Entity>(enemy);
        std::shared_ptr<Entity> newFlyingEnemy = std::static_pointer_cast<Entity>(flyingEnemy);
     
        if((float)rand()/RAND_MAX <= 0.5)
        {   
            std::cout << "--==enemy==--" << std::endl;
            addEntity(newEnemy);
            newEnemy->position = {(float)randX, (float)randY};
        }
        else
        {
            std::cout << "--==flying==--" << std::endl;
            addEntity(newFlyingEnemy);
            newFlyingEnemy->position = {(float)randX, (float)randY};
        }
    }

    // decrease life of player if hit by enemy bullet or enemy 
    if(!player->respawning)
    {
        for (auto entry : entities)
        {
            auto entity = entry.second;
            if (entity->type == EntityType::ENEMY_TYPE || entity->type == EntityType::ENEMY_BULLET)
            {
                    Rectangle collidingHitbox =
                    {
                        entity->position.x - (entity->hitboxDims.x) / 2,
                        entity->position.y - (entity->hitboxDims.y) / 2,
                        entity->hitboxDims.x,
                        entity->hitboxDims.y
                    };
                    Rectangle playerHitbox =
                    {
                        player->position.x - (player->hitboxDims.x) / 2,
                        player->position.y - (player->hitboxDims.y) / 2,
                        player->hitboxDims.x,
                        player->hitboxDims.y
                    };

                    if (CheckCollisionRecs(collidingHitbox, playerHitbox))
                    {
                        lives--;
                        if (lives <= 0) gameOver = true;
                        player->respawning = true;
                        player->respawnTimeLeft = player->respawnTime;
                        break;
                    }
            }
        }
    }

    // decrease hp of enemies if colloding with player bullet and more
    for (auto entry : entities)
    {
        auto entity = entry.second;


        if (entity->type == EntityType::ENEMY_TYPE)
        {
            std::shared_ptr<Enemy> enemy = std::static_pointer_cast<Enemy>(entity);

            Rectangle enemyHitbox =
            {
                enemy->position.x - (enemy->hitboxDims.x) / 2,
                enemy->position.y - (enemy->hitboxDims.y) / 2,
                enemy->hitboxDims.x,
                enemy->hitboxDims.y
            };

            // debugging tooltip -> check enemy info
            if (DEBUG)
            {
                float topLeft = screenHeight * 0.75;
                Vector2 mpos = GetMousePosition();

                if (CheckCollisionPointRec(mpos, enemyHitbox))
                {
                    auto posText = fmt::format("pos : {},{}", enemy->position.x, enemy->position.y);
                    auto formationPosition = fmt::format("form : {},{}", (*formationPositions[assignedPositionMap[enemy->id]]).x, (*formationPositions[assignedPositionMap[enemy->id]]).y);
                    auto resettingText = fmt::format("resetting: {}", enemy->resettingPosition);
                    auto attackingText = fmt::format("attacking: {}", enemy->attacking);
                    DrawTextEx(gameFont, posText.c_str(), {10, topLeft}, 20, 1, YELLOW);
                    DrawTextEx(gameFont, formationPosition.c_str(), {10, topLeft + 20}, 20, 1, YELLOW);
                    DrawTextEx(gameFont, resettingText.c_str(), {10, topLeft + 40}, 20, 1, YELLOW);
                    DrawTextEx(gameFont, attackingText.c_str(), {10, topLeft + 60}, 20, 1, YELLOW);
                }
            }
            
            // check if colliding with player bullets
            for (auto e : entities)
            {
                auto b = e.second;
                if (b->type == EntityType::PLAYER_BULLET)
                {
                    std::shared_ptr<Bullet> bullet = std::static_pointer_cast<Bullet>(b);
                    
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
            std::shared_ptr<Powerup> powerup = std::static_pointer_cast<Powerup>(entity);
            
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
    
    // delete entities if they are in the delete queue
    for(auto it = deleteQueue.rbegin(); it != deleteQueue.rend(); ++it)
    {
        deleteEntity((**it).id);
        deleteQueue.pop_back();
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
                for(int i = 0; i < formationPositions.size(); i++)
                {
                    formationPositions[i]->x += DELTA;   
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
                for (int i = 0; i < formationPositions.size(); i++)
                {
                    formationPositions[i]->x += DELTA;   
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
            std::shared_ptr<Player> player = std::static_pointer_cast<Player>(entity);
            player->update(this, screenWidth, screenHeight, dt);
        }
        else if (entity->type == EntityType::PLAYER_BULLET
                 || entity->type == EntityType::ENEMY_BULLET)
        {
            std::shared_ptr<Bullet> bullet = std::static_pointer_cast<Bullet>(entity);
            bullet->update(this, dt);
        }
        else if (entity->type == EntityType::ENEMY_TYPE)
        {
            std::shared_ptr<Enemy> enemy = std::static_pointer_cast<Enemy>(entity);
            
            // if (!enemy->attacking && !enemy->resettingPosition)
            // {
            //     enemy->position = (*assignedPositionMap[enemy->id]);
            // }
            
            if (enemy->enemyType == EnemyType::SIMPLE)
            {
                std::shared_ptr<SimpleEnemy> simpleEnemy = std::static_pointer_cast<SimpleEnemy>(enemy);
                std::shared_ptr<Powerup> powerup = simpleEnemy->update(this, dt);
                if (powerup)
                {
                    powerupsToAdd.push_back(powerup);
                }   
            }
            else if (enemy->enemyType == EnemyType::FLYING)
            {
                std::shared_ptr<FlyingEnemy> flyingEnemy = std::static_pointer_cast<FlyingEnemy>(enemy);
                std::shared_ptr<Powerup> powerup = flyingEnemy->update(this, dt);
                if (powerup)
                {
                    powerupsToAdd.push_back(powerup);
                }   
            }
            
        }
        else if (entity->type == EntityType::POWERUP_TYPE)
        {
            std::shared_ptr<Powerup> powerup = std::static_pointer_cast<Powerup>(entity);
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
            std::shared_ptr<Bullet> bullet = std::static_pointer_cast<Bullet>(entity);
            bullet->draw(this, dt);
        }
        else if (entity->type == EntityType::ENEMY_TYPE)
        {
            std::shared_ptr<Enemy> enemy = std::static_pointer_cast<Enemy>(entity);
            enemy->draw(this, dt);
        }
        else if (entity->type == EntityType::PLAYER_TYPE)
        {
            std::shared_ptr<Player> p = std::static_pointer_cast<Player>(entity);
            p->draw();
        }
        else if (entity->type == EntityType::POWERUP_TYPE)
        {
            entity->draw();
        }
    }
}
