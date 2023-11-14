#define FMT_HEADER_ONLY
#define _USE_MATH_DEFINES

#define BULLET_HEIGHT 10
#define BULLET_WIDTH 5
#define BULLET_VEL 0.5
#define BULLET_PER_SECOND 3

#include "Manager.h"
#include "Player.h"
#include "SimpleEnemy.h"
#include "FlyingEnemy.h"

#include "raylib.h"
#include "raymath.h"
#include <chrono>
#include <fmt/core.h>
#include <random>
#include <vector>

#ifndef LOADEDFONT
#define LOADEDFONT LoadFontEx("resources/fonts/CascadiaCode/CascadiaCode.ttf", 20, 0, 500)
#endif

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 720;
    const int screenHeight = 960;
    
    InitWindow(screenWidth, screenHeight, "smol-invaders");
    InitAudioDevice();
    SetMasterVolume(0.2);
    
    // create game manager instance - a simple ECS
    Manager gameManager = Manager
    (
     screenWidth,
     screenHeight,
     {200, 200},
     {480, 480},
     48.0
     );
    
    Vector2 shipPosition =
    {
        (float)screenWidth / 2.0f,
        8 * (float)screenHeight / 10.0f,
    };
    
    // Load Textures + Create Entities
    //--------------------------------------------------------------------------------------
    const int shipSize = 64;
    // const int enemySpriteSize = 50;
    
    // Resources - Textures, Sounds, etc.
    const char* shipTextureLocation = "./resources/textures/ship6.png";
    const char* defaultBulletSpriteSheetLocation = "./resources/textures/yellow_bullets.png";
    const char* enemySpriteSheetLocation = "./resources/textures/enemies.png";
    
    const char* defaultFireSoundLocation = "./resources/sounds/shoot4.wav";
    const char* enemyExplosionSoundLocation = "./resources/sounds/explosion2.wav";
    const char* simpleEnemyFireSoundLocation = "./resources/sounds/simpleEnemy_shoot.wav";
    
    
    const char* powerupSoundLocation = "./resources/sounds/powerUp.wav";
    
    Texture2D shipTexture = LoadTexture(shipTextureLocation);
    Texture2D defaultBulletSheet = LoadTexture(defaultBulletSpriteSheetLocation);
    Texture2D enemyTexture = LoadTexture(enemySpriteSheetLocation);
    
    Wave defaultFireWave = LoadWave(defaultFireSoundLocation);
    Sound defaultFireSound = LoadSoundFromWave(defaultFireWave);
    
    Wave simpleEnemyFireWave = LoadWave(simpleEnemyFireSoundLocation);
    Sound simpleEnemyFireSound = LoadSoundFromWave(simpleEnemyFireWave);
    
    Wave enemyExplosionWave = LoadWave(enemyExplosionSoundLocation);
    Sound enemyExplosionSound = LoadSoundFromWave(enemyExplosionWave);
    
    Wave powerupWave = LoadWave(powerupSoundLocation);
    Sound powerupSound = LoadSoundFromWave(powerupWave);
    
    int min_value = 250;
    int max_value = 500;
    
    // Initialize a random number generator
    std::random_device rd;  // Seed for the random number generator
    std::mt19937 rng(rd()); // Mersenne Twister pseudo-random generator
    
    std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(
                                shipTexture,
                                defaultFireSound,
                                powerupSound,
                                defaultBulletSheet,
                                {0, 0},
                                {0, 0},
                                1, 0.0,
                                {(float)shipTexture.width,
                                    (float)shipTexture.height},
                                {shipSize, shipSize},
                                {shipSize, shipSize},
                                shipPosition,
                                0.5,
                                0.01,
                                0.0001,
                                9.8,
                                BULLET_PER_SECOND,
                                3.0
                                ));
    
    std::shared_ptr<Entity> newEntity = std::static_pointer_cast<Entity>(player);
    
    gameManager.addEntity(newEntity);
    
    for (int i = 0; i < 37; i++)
    {
        // Define the distribution for the random integer within the range
        std::uniform_int_distribution<int> distribution(min_value, max_value);
        
        // Generate a random integer within the specified range
        int X = distribution(rng);
        int Y = distribution(rng);
        
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
         {(float)X, (float)Y}, 
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
         {(float)X, (float)Y}, 
         10,
         50,
         0.2
         ));
        
        std::shared_ptr<Entity> newEnemy = std::static_pointer_cast<Entity>(enemy);
        std::shared_ptr<Entity> newFlyingEnemy = std::static_pointer_cast<Entity>(flyingEnemy);
        
        gameManager.addEntity(newEnemy);
        gameManager.addEntity(newFlyingEnemy);
    }
    
    // vector of bullets
    std::vector<Rectangle*> bullets;
    
    SetTargetFPS(120); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    // Game Stats + Misc.
    //--------------------------------------------------------------------------------------
    bool showHitboxes = false;
    std::chrono::steady_clock::time_point lastShotTime;
    
    int score = 0;
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        
        
        BeginDrawing();
        
        ClearBackground({8, 36, 52, 255});

        if(!gameManager.gameOver)
        {
            // Update
            //----------------------------------------------------------------------------------
            gameManager.update();
            //----------------------------------------------------------------------------------
        }

        // Draw
        //----------------------------------------------------------------------------------
        gameManager.draw();

        if (gameManager.gameOver)
        {
            DrawTextEx(gameManager.gameFont, "GAME OVER", {(float)gameManager.screenWidth/2 - 200, (float)gameManager.screenHeight/2 - 100}, 80, 1, RED);
        }

        //----------------------------------------------------------------------------------

        std::string shipPosText = fmt::format("X: {}, Y: {}", player->position.x, player->position.y);
        std::string formationPositionsText = fmt::format("total: {}, assigned: {}", gameManager.formationPositions.size(), gameManager.assignedPositionMap.size());
        
        int bulletCount = 0;
        for (auto kv : gameManager.entities)
        {
            if (kv.second->type == EntityType::PLAYER_BULLET)
            {
                bulletCount++;
            }
        }
        

        std::string scoreText = fmt::format("score : {}", gameManager.score);
        std::string numBulletsText = fmt::format("# of entities: {}, # of bullets: {}", gameManager.entities.size(), bulletCount);
        std::string velocityText = fmt::format("ship velocity: X: {}, y: {}", player->currentVelocity.x, player->currentVelocity.y);
        std::string livesText = fmt::format("lives : {}", gameManager.lives);
        
        DrawTextEx(gameManager.gameFont, scoreText.c_str(), {10, 10}, 20, 1, RAYWHITE);
        DrawTextEx(gameManager.gameFont, "move the ship with arrow keys or WASD", {10, 30}, 20, 1, RAYWHITE);
        DrawTextEx(gameManager.gameFont, shipPosText.c_str(), {10, 50}, 20, 1, RAYWHITE);
        DrawTextEx(gameManager.gameFont, numBulletsText.c_str(), {10, 70}, 20, 1, RAYWHITE);
        DrawTextEx(gameManager.gameFont, velocityText.c_str(), {10, 90}, 20, 1, RAYWHITE);
        DrawTextEx(gameManager.gameFont, livesText.c_str(), {10, 130}, 20, 1, RAYWHITE);
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    
    return 0;
}
