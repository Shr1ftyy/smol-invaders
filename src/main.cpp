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
#define BULLET_PER_SECOND 5

//------------------------------------------------------------------------------------
// fire weapons
//------------------------------------------------------------------------------------
void fireWeapons(Vector2 shipOrigin, int shipSize, std::vector<Rectangle*>* bullets) {
    Rectangle* newBullet = new Rectangle;
    newBullet->x = shipOrigin.x - BULLET_WIDTH / 2;
    newBullet->y = shipOrigin.y - (shipSize / 2) - BULLET_HEIGHT;
    newBullet->width = BULLET_WIDTH;
    newBullet->height = BULLET_HEIGHT;
    
    bullets->push_back(newBullet);
}

//------------------------------------------------------------------------------------
// Render bullets
//------------------------------------------------------------------------------------
//void renderBullets(std::vector<Rectangle*>* bullets, int screenWidth, int screenHeight) {
//    std::vector<Rectangle*>::iterator iter = bullets->begin();
//    int i = 0;  // counter.
//
//    bullets->erase(std::remove_if(
//        bullets->begin(), 
//        bullets->end(),
//        [screenWidth, screenHeight](Rectangle* bullet)->bool {
//            Vector2 bulletPos = { bullet->x, bullet->y };
//            return outOfBounds(bulletPos, screenWidth, screenHeight);
//        }), 
//        bullets->end()
//	);
//
//    if (bullets->size() <= 0) {
//        return;
//    }
//
//    for (iter; iter != bullets->end(); ++iter, i++) {
//        auto bullet = *iter;
//        DrawRectangle(bullet->x, bullet->y, bullet->width, bullet->height, RED);
//        bullet->y -= BULLET_VEL;
//    }
//}

//------------------------------------------------------------------------------------
// Render enemies
//------------------------------------------------------------------------------------


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

    //Texture2D shipTexture = LoadTexture("./resources/textures/ship.png");
    //Texture2D enemySpriteSheet = LoadTexture("./resources/textures/enemies.png");

    const char* shipTextureLocation = "./resources/textures/ship.png";
    const char* enemySpriteSheetLocation = "./resources/textures/enemies.png";
    const char* defaultBulletSpriteSheetLocation = "./resources/textures/yellow_bullets.png";

    Texture2D shipTexture = LoadTexture(shipTextureLocation);
    Texture2D defaultBulletSheet = LoadTexture(defaultBulletSpriteSheetLocation);
	Font hackNerdFontRegular = LoadFontEx("resources/fonts/HackNerdFontMono/HackNerdFontMono-Regular.ttf", 20, 0, 250);

    Player* player = new Player(shipTexture, defaultBulletSheet, { 0, 0 }, { 0, 0 }, 1, 0, { shipSize, shipSize }, shipPosition, 0.5, 0.01, 0.0001, 9.8, BULLET_PER_SECOND);
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

        //Rectangle shipSourceRec = { 0.0f, 0.0f, (float)shipSize, (float)shipSize };
        //Rectangle shipDestRec = { shipPosition.x, shipPosition.y, shipSize, shipSize};
        //Vector2 origin = { (float)shipSize/2, (float)shipSize/2 };
        //Vector2 xOffset { (float)shipSize / 2, 0};
        //Vector2 yOffset{ 0, -(float)shipSize / 2 };

        //Rectangle enemySourceRec = { 0.0f, 0.0f, (float)enemySpriteSize, (float)enemySpriteSize };
        //Rectangle enemyDestRec = { 125, 125, enemySpriteSize, enemySpriteSize };
        //Vector2 enemyOrigin = { (float)enemySpriteSize /2, (float)enemySpriteSize /2 };

        // draw ship
        //DrawTexturePro(shipTexture, shipSourceRec, shipDestRec, origin, (float)0, WHITE);
        //// draw enemies
        //DrawTexturePro(enemySpriteSheet, enemySourceRec, enemyDestRec, enemyOrigin, (float)0, WHITE);
        // draw bullets
        //renderBullets(&bullets, screenWidth, screenHeight);
        // DrawTexture(shipTexture, shipPosition.x - origin.x, shipPosition.y - origin.y, WHITE);
        //if(showHitboxes) DrawRectangleLines(shipPosition.x - origin.x, shipPosition.y - origin.y, shipSize, shipSize, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}