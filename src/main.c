#include "raylib.h"
#include "../inc/Button.h"
#include "../inc/Tile.h"
#include "../inc/Grid.h"

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
    TextButton button = CreateTextButton( screen_width, screen_height, btn_text, 50);

    // Tile
    Texture2D tilesheet = LoadTexture("../assets/kiopp_minesweeper_tilesheet.png");
    TileMapTexture textures = SplitTileMap(tilesheet);

    GameGrid grid = CreateGrid(
        screen_width, 
        screen_height, 
        tilesheet.width/2, 
        &textures, 
        5, 
        5, 
        5, 
        4, 
        40
        );

    while(!WindowShouldClose()){

        switch (state) {
            case s_setup:
                HandleTextButtonPress(&button);
                break;
            case s_playing:
                HandleGridTileButtons(&grid);
                HandleGridTileButtonClicked(&grid, &textures);
                break;
            default:
                break;
        }
        
        
    /*
        Have a keyboard shortcut for placing flags left shift has to be held down and is not a toggle.

        As well as a toggle button that can be toggled by clicking on it with the mouse.
    */

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (state) 
        {
            case s_setup:
                DrawTextButton(&button);
                if (button.button_pressed) {
                    button.button_pressed = 0;
                    state = s_playing;
                }
                break;
            case s_playing:
                DrawGameGrid(&grid);
                break;
            default:
                DrawCircle(screen_width/2, screen_height/2, 100, RED);
                break;
        }
        EndDrawing(); 
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}