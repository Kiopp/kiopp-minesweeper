#include "raylib.h"
#include "Button.h"
#include "Tile.h"
#include "Grid.h"

enum gameState {
    s_setup,
    s_playing,
    s_win,
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

    GameGrid grid;


    int flag_enable = 0;

    while(!WindowShouldClose()){

        switch (state) {
            case s_setup:
                HandleTextButtonPress(&button);

                if (button.button_pressed) {
                    // Grid settings
                    int tile_size = tilesheet.width/2;
                    int grid_cols = 9;
                    int grid_rows = 9;
                    int num_mines = 2;
                    int scale = 4;
                    int tile_font = 25;

                    // Handle button press
                    button.button_pressed = 0;
                    state = s_playing;

                    // Change window size
                    int grid_width = grid_cols * tile_size * scale;
                    int grid_height = grid_rows * tile_size * scale;

                    screen_width = grid_width + 200; // grid_width + padding
                    screen_height = grid_height + 200; // grid_height + padding

                    SetWindowSize(screen_width, screen_height);

                    // Generate grid
                    grid = CreateGrid(
                        screen_width, 
                        screen_height, 
                        tile_size, 
                        &textures, 
                        grid_cols, 
                        grid_rows, 
                        num_mines, 
                        scale, 
                        tile_font
                        );
                }
                break;

            case s_playing:
                // Handle flag keybind
                if (IsKeyDown(KEY_LEFT_SHIFT)) { 
                    flag_enable = 1; 
                } else {
                    flag_enable = 0;
                }

                // Handle tiles
                HandleGridTileButtons(&grid);
                HandleGridTileButtonClicked(&grid, &textures, flag_enable);

                // Check game over condition
                if (grid.game_over == 1) {
                    state = s_game_over;
                }
                if (grid.game_win == 1) {
                    state = s_win;
                } 
                
                break;

            case s_game_over:
                break;

            case s_win:
                break;

            default:
                break;
        }
        
        
    /*
        Add toggle button that can be toggled by clicking on it with the mouse.
    */

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (state) 
        {
            case s_setup:
                DrawTextButton(&button);
                break;

            case s_playing:
                DrawGameGrid(&grid);
                break;

            case s_win:
                DrawGameGrid(&grid);
                DrawCircle(screen_width/2, screen_height/2, 100, GREEN);
                break;

            case s_game_over:
                DrawGameGrid(&grid);
                DrawCircle(screen_width/2, screen_height/2, 100, RED);
                break;

            default:
                DrawCircle(screen_width/2, screen_height/2, 100, BLUE);
                break;
        }
        EndDrawing(); 
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}