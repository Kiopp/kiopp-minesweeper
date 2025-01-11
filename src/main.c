#include "raylib.h"
#include "../inc/Button.h"
#include "../inc/Tile.h"

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
    TileMapTexture tile_textures = SplitTileMap(tilesheet);

    Tile tile = CreateTile(screen_width, screen_height, tilesheet.width/2, tilesheet.height/2, tile_textures.tileMap[0], empty);

    while(!WindowShouldClose()){

        HandleTextButtonPress(&button);
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
                DrawTextButton(&button);
                if (button.button_pressed) {
                    button.button_pressed = 0;
                    state = s_playing;
                }
                break;
            case s_playing:
                // Draw the individual tiles (example)
                DrawTile(&tile);
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