#define FMT_HEADER_ONLY
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <fmt/core.h>

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
    
    // Load Textures
    //--------------------------------------------------------------------------------------
    Texture2D shipTexture = LoadTexture("../resources/textures/ship2.png");
    const int shipSize = 50;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        bool fireWeapons = false;
        Vector2 oldShipPosition = shipPosition;

        if (IsKeyDown(KEY_RIGHT)) shipPosition.x += 7.0f;
        if (IsKeyDown(KEY_LEFT)) shipPosition.x -= 7.0f;
        if (IsKeyDown(KEY_UP)) shipPosition.y -= 7.0f;
        if (IsKeyDown(KEY_DOWN)) shipPosition.y += 7.0f;
        if (IsKeyDown(KEY_SPACE)) fireWeapons = true;

        if (shipPosition.x <= 0 || shipPosition.x >= screenWidth
            || shipPosition.y <= 0 || shipPosition.y >= screenHeight) {
            shipPosition = oldShipPosition;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        std::string shipPosText = fmt::format("X: {}, Y: {}", shipPosition.x, shipPosition.y);

        DrawText("move the ship with arrow keys", 10, 10, 20, RAYWHITE);
        DrawText(shipPosText.c_str(), 10, 30, 20, RAYWHITE);


        Rectangle shipSourceRec = { 0.0f, 0.0f, (float)shipSize, (float)shipSize };
        Rectangle shipDestRec = { shipPosition.x, shipPosition.x, shipSize, shipSize};
        Vector2 origin = { (float)shipSize/2, (float)shipSize/2 };
        Vector2 xOffset { (float)shipSize / 2, 0};
        Vector2 yOffset{ 0, -(float)shipSize / 2 };

        DrawTexturePro(shipTexture, shipSourceRec, shipDestRec, origin, (float)0, WHITE);
        // DrawTexture(shipTexture, shipPosition.x, shipPosition.y, WHITE);
        DrawRectangleLines(shipPosition.x - origin.x, shipPosition.y - origin.y, shipSize, shipSize, RED);

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