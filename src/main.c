#include "raylib.h"
#include "../inc/Button.h"
#include "../inc/Tile.h"
#include "../inc/Grid.h"

enum gameState {
    s_setup,
    s_playing,
    s_game_over
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
        16, 
        16, 
        48, 
        2, 
        25
        );


    int flag_enable = 0;

    while(!WindowShouldClose()){

        switch (state) {
            case s_setup:
                HandleTextButtonPress(&button);
                break;
            case s_playing:
                if (IsKeyDown(KEY_LEFT_SHIFT)) { 
                    flag_enable = 1; 
                } else {
                    flag_enable = 0;
                }
                HandleGridTileButtons(&grid);
                HandleGridTileButtonClicked(&grid, &textures, flag_enable);
                break;
            case s_game_over:
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
                if (grid.game_over == 1) {
                    state = s_game_over;
                }
                break;
            case s_game_over:
                DrawGameGrid(&grid);
                DrawCircle(screen_width/2, screen_height/2, 100, RED);
            default:
                DrawCircle(screen_width/2, screen_height/2, 100, RED);
                break;
        }
        EndDrawing(); 
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}