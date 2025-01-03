#include "raylib.h"

int main()
{
    // Window setup
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Kiopp Minesweeper");
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        BeginDrawing();

        DrawCircle(screenWidth/2, screenHeight/2, 100, RED);

        EndDrawing(); 
    }
    CloseWindow(); // Close window and OpenGL context
    return 0;
}