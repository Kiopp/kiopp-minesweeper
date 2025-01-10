#include "raylib.h"
#include "../inc/Button.h"

enum gameState {
    s_setup,
    s_playing
};

int main()
{
    // Window setup
    int screen_width = 1280;
    int screen_height = 720;
    InitWindow(screen_width, screen_height, "Kiopp Minesweeper");
    SetTargetFPS(60);

    // Game setup
    enum gameState state = s_setup;
    char btn_text[32] = "Start";
    Button button = CreateButton( screen_width, screen_height, btn_text, 50);

    while(!WindowShouldClose()){

        HandleButtonPress(&button);
    /*
        Have a keyboard shortcut for placing flags left shift has to be held down and is not a toggle.

        As well as a toggle button that can be toggled by clicking on it with the mouse.

        Need Tile struct that contains an enum representing it's state as well as the number of mines around it.
        Need Grid struct containing a grid of tiles.

        Need to split the tilemap texture into individual parts that can be rendered several times independantly.
    */
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (state) 
        {
            case s_setup:
                DrawButton(&button);
                if (button.button_pressed) {
                    button.button_pressed = 0;
                    state = s_playing;
                }
                break;
            case s_playing:
                DrawCircle(screen_width/2, screen_height/2, 100, GREEN);
                break;
            default:
                DrawCircle(screen_width/2, screen_height/2, 100, RED);
                break;
        }
        EndDrawing(); 
    }
    CloseWindow(); // Close window and OpenGL context

    /* Example scaled texture draw
    Rectangle destRec = (Rectangle){ 0, 0, tileMap.width * scale, tileMap.height * scale};
    DrawTexturePro(
        tileMap, 
        testRec, 
        destRec, 
        (Vector2){0, 0}, 
        0, 
        WHITE);
    */
    return 0;
}