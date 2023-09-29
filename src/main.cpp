#define FMT_HEADER_ONLY

#include "Manager.h"
#include "Player.h"
#include "SimpleEnemy.h"

#include "raylib.h"
#include "raymath.h"
#include <chrono>
#include <fmt/core.h>
#include <random>
#include <vector>

#define BULLET_HEIGHT 10
#define BULLET_WIDTH 5
#define BULLET_VEL 0.5
#define BULLET_PER_SECOND 3

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
            (float)screenHeight / 2.0f,
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
    
    const char* powerupSoundLocation = "./resources/sounds/powerUp.wav";

    Texture2D shipTexture = LoadTexture(shipTextureLocation);
    Texture2D defaultBulletSheet = LoadTexture(defaultBulletSpriteSheetLocation);
    Texture2D enemyTexture = LoadTexture(enemySpriteSheetLocation);

    Wave defaultFireWave = LoadWave(defaultFireSoundLocation);
    Sound defaultFireSound = LoadSoundFromWave(defaultFireWave);

    Wave enemyExplosionWave = LoadWave(enemyExplosionSoundLocation);
    Sound enemyExplosionSound = LoadSoundFromWave(enemyExplosionWave);
    
    Wave powerupWave = LoadWave(powerupSoundLocation);
    Sound powerupSound = LoadSoundFromWave(powerupWave);

    Font hackNerdFontRegular = LoadFontEx("resources/fonts/HackNerdFontMono/HackNerdFontMono-Regular.ttf", 20, 0, 250);

    int min_value = 250;
    int max_value = 500;

    // Initialize a random number generator
    std::random_device rd;  // Seed for the random number generator
    std::mt19937 rng(rd()); // Mersenne Twister pseudo-random generator

    Player* player = new Player(
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
    );
    
    Entity* newEntity = static_cast<Entity*>(player);

    gameManager.addEntity(newEntity);

    for (int i = 0; i < 37; i++)
    {
        // Define the distribution for the random integer within the range
        std::uniform_int_distribution<int> distribution(min_value, max_value);

        // Generate a random integer within the specified range
        int X = distribution(rng);
        int Y = distribution(rng);

        SimpleEnemy* enemy = new SimpleEnemy
        (
            enemyTexture, 
            enemyExplosionSound, 
            {0.0, 0.0}, 
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
            10
        );
        
        Entity* newEnemy = static_cast<Entity*>(enemy);

        gameManager.addEntity(newEnemy);
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
        // Update
        //----------------------------------------------------------------------------------
        gameManager.update();
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground({8, 36, 52, 255});

        gameManager.draw();

        std::string shipPosText = fmt::format("X: {}, Y: {}", player->position.x, player->position.y);

        int bulletCount = 0;
        for (auto kv : gameManager.entities)
        {
            if (kv.second->type == EntityType::PLAYER_BULLET)
            {
                bulletCount++;
            }
        }

        std::string numBulletsText = fmt::format("# of entities: {}, # of bullets: {}", gameManager.entities.size(), bulletCount);
        std::string velocityText = fmt::format("ship velocity: X: {}, y: {}", player->currentVelocity.x, player->currentVelocity.y);

        DrawTextEx(hackNerdFontRegular, "move the ship with arrow keys or WASD", {10, 10}, 20, 1, RAYWHITE);
        DrawTextEx(hackNerdFontRegular, shipPosText.c_str(), {10, 30}, 20, 1, RAYWHITE);
        DrawTextEx(hackNerdFontRegular, numBulletsText.c_str(), {10, 50}, 20, 1, RAYWHITE);
        DrawTextEx(hackNerdFontRegular, velocityText.c_str(), {10, 70}, 20, 1, RAYWHITE);
        DrawFPS(10, 90);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}