#define FMT_HEADER_ONLY
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <fmt/core.h>
#include <chrono>
#include <vector>

#include "Manager.h"
#include "Player.h"

#define BULLET_HEIGHT 10
#define BULLET_WIDTH 5
#define BULLET_VEL 5
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

    // create game manager instance - a simple ECS
    Manager gameManager = Manager(screenWidth, screenHeight);

    Vector2 shipPosition = 
    { 
        (float)screenWidth / 2.0f,
        (float)screenHeight / 2.0f,
    };
    
    // Load Textures + Create Entities
    //--------------------------------------------------------------------------------------
    const int shipSize = 50;
    const int enemySpriteSize = 48;

    const char* shipTextureLocation = "./resources/textures/ship2.png";
    const char* enemySpriteSheetLocation = "./resources/textures/enemies.png";
    const char* defaultBulletSpriteSheetLocation = "./resources/textures/yellow_bullets.png";

    Texture2D shipTexture = LoadTexture(shipTextureLocation);
    Texture2D defaultBulletSheet = LoadTexture(defaultBulletSpriteSheetLocation);
    Font hackNerdFontRegular = LoadFontEx("resources/fonts/HackNerdFontMono/HackNerdFontMono-Regular.ttf", 20, 0, 250);

    Player* player = new Player(shipTexture, defaultBulletSheet, { 0, 0 }, { 0, 0 }, 1, 0, { shipSize, shipSize }, shipPosition, 0.5, 0.01, 0.0001, 9.8, BULLET_PER_SECOND, 3);
    Entity* newEntity = static_cast<Entity*>(player);
    gameManager.addEntity(newEntity);

    // vector of bullets
    std::vector<Rectangle*> bullets;

    SetTargetFPS(120);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Game Stats + Misc.
    //--------------------------------------------------------------------------------------
    bool showHitboxes = false;
    std::chrono::steady_clock::time_point lastShotTime;

    int score = 0;
    //--------------------------------------------------------------------------------------
     
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        gameManager.update();
        //----------------------------------------------------------------------------------
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        gameManager.draw();

        std::string shipPosText = fmt::format("X: {}, Y: {}", player->position.x, player->position.y);

        int bulletCount = 0;
        for (auto kv : gameManager.entities) {
            if (kv.second->type == EntityType::PLAYER_BULLET) {
                bulletCount++;
            }
        }

        std::string numBulletsText = fmt::format("# of entities: {}, # of bullets: {}", gameManager.entities.size(), bulletCount);
        std::string velocityText = fmt::format("ship velocity: X: {}, y: {}", player->currentVelocity.x, player->currentVelocity.y);

        DrawTextEx(hackNerdFontRegular, "move the ship with arrow keys", { 10, 10}, 20, 1, RAYWHITE);
        DrawTextEx(hackNerdFontRegular, shipPosText.c_str(), { 10, 30 }, 20, 1, RAYWHITE);
        DrawTextEx(hackNerdFontRegular, numBulletsText.c_str(), { 10, 50 }, 20, 1, RAYWHITE);
        DrawTextEx(hackNerdFontRegular, velocityText.c_str(), { 10, 70 }, 20, 1, RAYWHITE);
        DrawFPS(10, 90);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}