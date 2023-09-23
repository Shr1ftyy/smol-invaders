#define FMT_HEADER_ONLY
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <fmt/core.h>
#include <chrono>
#include <vector>

#define BULLET_HEIGHT 10
#define BULLET_WIDTH 5
#define BULLET_VEL 5
#define BULLET_PER_SECOND 2

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
// Out-Of-Bounds check
//------------------------------------------------------------------------------------
bool outOfBounds(Vector2 entity, int screenWidth, int screenHeight) {
    if (entity.x <= 0 || entity.x >= screenWidth
        || entity.y <= 0 || entity.y >= screenHeight) {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------------
// Render bullets
//------------------------------------------------------------------------------------
void renderBullets(std::vector<Rectangle*>* bullets, int screenWidth, int screenHeight) {
    std::vector<Rectangle*>::iterator iter = bullets->begin();
    int i = 0;  // counter.

    bullets->erase(std::remove_if(
        bullets->begin(), 
        bullets->end(),
        [screenWidth, screenHeight](Rectangle* bullet)->bool {
            Vector2 bulletPos = { bullet->x, bullet->y };
            return outOfBounds(bulletPos, screenWidth, screenHeight);
        }), 
        bullets->end()
	);

    if (bullets->size() <= 0) {
        return;
    }

    for (iter; iter != bullets->end(); ++iter, i++) {
        auto bullet = *iter;
        DrawRectangle(bullet->x, bullet->y, bullet->width, bullet->height, RED);
        bullet->y -= BULLET_VEL;
    }
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 768;
    const int screenHeight = 1024;

    InitWindow(screenWidth, screenHeight, "smol-invaders");

    Vector2 shipPosition = 
    { 
        (float)screenWidth / 2.0f,
        (float)screenHeight / 2.0f,
    };
    
    // Load Textures + Create Entities
    //--------------------------------------------------------------------------------------
    Texture2D shipTexture = LoadTexture("./resources/textures/ship.png");
    const int shipSize = 50;

    // vector of bullets
    std::vector<Rectangle*> bullets;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
	bool showHitboxes = false;
    std::chrono::steady_clock::time_point lastShotTime;

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        Vector2 oldShipPosition = shipPosition;

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) shipPosition.x += 7.0f;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) shipPosition.x -= 7.0f;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) shipPosition.y -= 7.0f;
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) shipPosition.y += 7.0f;
        if (IsKeyDown(KEY_H)) showHitboxes = !showHitboxes;

        if (outOfBounds(shipPosition, screenWidth, screenHeight)) {
            shipPosition = oldShipPosition;
        }

        if (IsKeyDown(KEY_SPACE)) {
            auto now = std::chrono::high_resolution_clock::now();
            auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastShotTime);
            if ( timeElapsed.count() >= (float)1000 / BULLET_HEIGHT) {
				fireWeapons(shipPosition, shipSize, &bullets);
                lastShotTime = now;
            }
        }

        // fire weapon(s)
        if (IsKeyDown(KEY_SPACE)) {
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        std::string shipPosText = fmt::format("X: {}, Y: {}", shipPosition.x, shipPosition.y);
        std::string numBulletsText = fmt::format("# of Bullets: {}", bullets.size());

        DrawText("move the ship with arrow keys", 10, 10, 20, RAYWHITE);
        DrawText(shipPosText.c_str(), 10, 30, 20, RAYWHITE);
        DrawText(numBulletsText.c_str(), 10, 50, 20, RAYWHITE);
        

        Rectangle shipSourceRec = { 0.0f, 0.0f, (float)shipSize, (float)shipSize };
        Rectangle shipDestRec = { shipPosition.x, shipPosition.y, shipSize, shipSize};
        Vector2 origin = { (float)shipSize/2, (float)shipSize/2 };
        Vector2 xOffset { (float)shipSize / 2, 0};
        Vector2 yOffset{ 0, -(float)shipSize / 2 };

        // draw ship
        DrawTexturePro(shipTexture, shipSourceRec, shipDestRec, origin, (float)0, WHITE);
        // draw bullets
        renderBullets(&bullets, screenWidth, screenHeight);
        // DrawTexture(shipTexture, shipPosition.x - origin.x, shipPosition.y - origin.y, WHITE);
        if(showHitboxes) DrawRectangleLines(shipPosition.x - origin.x, shipPosition.y - origin.y, shipSize, shipSize, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(shipTexture);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}