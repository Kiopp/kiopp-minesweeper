#include "TextBox.h"
#include "raylib.h"
#include "Button.h"
#include "Tile.h"
#include "Grid.h"
#include "TextBox.h"
#include <stdio.h>

enum gameState {
    s_setup,
    s_playing,
    s_win,
    s_game_over
};

void CheckWindowSize(int* screen_width, int* screen_height){
    // Check boundries
    if (*screen_width < 1280) {
        *screen_width = 1280;
    }
    if (*screen_height < 720) {
        *screen_height = 720;
    }

    // Update window size
    SetWindowSize(*screen_width, *screen_height);
}

void UpdateAllNumberBox(NumberBox* num_mines_input, NumberBox* width_input, NumberBox* height_input){
    UpdateNumberBox(num_mines_input);
    if (!num_mines_input->mouse_on_text) {
        UpdateNumberBox(height_input);
        if (!height_input->mouse_on_text) {
            UpdateNumberBox(width_input);
        }
    }
}

void ResetGame(int screen_width, int screen_height, TextButton* start_button, NumberBox* num_mines_input, NumberBox* width_input, NumberBox* height_input){
    
    // Start button
    char btn_text[32] = "Start";
    *start_button = CreateTextButton( 
        screen_width, 
        screen_height, 
        btn_text, 
        50
        );

    // Number boxes
    int font_size = 30;

    char mine_label[50] = "Number of mines";
    *num_mines_input = CreateNumberBox(
        screen_width, 
        screen_height, 
        100, 
        40, 
        0,
        120,
        font_size, 
        5,
        mine_label
        );

    char width_label[50] = "Width";
    *width_input = CreateNumberBox(
        screen_width, 
        screen_height, 
        100, 
        40, 
        200,
        120,
        font_size, 
        9,
        width_label
        );

    char height_label[50] = "Height";
    *height_input = CreateNumberBox(
        screen_width, 
        screen_height, 
        100, 
        40, 
        -200,
        120,
        font_size, 
        9,
        height_label
        );
}

int main()
{
    // Window setup
    int screen_width = 1280;
    int screen_height = 720;
    InitWindow(screen_width, screen_height, "Kiopp Minesweeper");
    SetTargetFPS(60);

    // Game setup
    enum gameState state = s_setup;

    // Buttons
    char btn_restart_text[32] = "Retry";
    TextButton button;
    TextButton restart_button;

    // Tile
    Texture2D tilesheet = LoadTexture("../assets/kiopp_minesweeper_tilesheet.png");
    TileMapTexture textures = SplitTileMap(tilesheet);

    // grid
    GameGrid grid;

    // TextBox
    NumberBox num_mines_input;
    NumberBox width_input;
    NumberBox height_input;

    int flag_enable = 0;

    ResetGame(screen_width, screen_height, &button, &num_mines_input, &width_input, &height_input);

    while(!WindowShouldClose()){

        // Update state mashine
        switch (state) {
            case s_setup:
                HandleTextButtonPress(&button);
                UpdateAllNumberBox(&num_mines_input, &width_input, &height_input);

                if (button.button_pressed) {
                    // Grid settings
                    int tile_size = tilesheet.width/2;
                    int grid_cols = width_input.value;
                    int grid_rows = height_input.value;
                    int num_mines = num_mines_input.value;
                    int scale = 4;
                    int tile_font = 40;

                    // Handle button press
                    button.button_pressed = 0;
                    if (num_mines > grid_cols * grid_rows) {
                        printf("Invalid number of mines!\n");
                        break;
                    }
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
                    restart_button = CreateTextButton( screen_width, screen_height, btn_restart_text, 50);
                }
                if (grid.game_win == 1) {
                    state = s_win;
                    restart_button = CreateTextButton( screen_width, screen_height, btn_restart_text, 50);
                } 
                
                break;

            case s_game_over:
                HandleTextButtonPress(&restart_button);

                if (restart_button.button_pressed) {
                    restart_button.button_pressed = 0;
                    state = s_setup;

                    CheckWindowSize(&screen_width, &screen_height);
                    ResetGame(screen_width, screen_height, &button, &num_mines_input, &width_input, &height_input);
                }
                break;

            case s_win:
                HandleTextButtonPress(&restart_button);

                if (restart_button.button_pressed) {
                    restart_button.button_pressed = 0;
                    state = s_setup;

                    CheckWindowSize(&screen_width, &screen_height);
                    ResetGame(screen_width, screen_height, &button, &num_mines_input, &width_input, &height_input);
                }
                break;

            default:
                break;
        }
        
        
    /*
        Add toggle button that can be toggled by clicking on it with the mouse.
    */

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Drawing state mashine
        switch (state) 
        {
            case s_setup:
                DrawTextButton(&button);
                DrawNumberBox(&num_mines_input);
                DrawNumberBox(&height_input);
                DrawNumberBox(&width_input);
                break;

            case s_playing:
                DrawGameGrid(&grid);
                break;

            case s_win:
                DrawGameGrid(&grid);
                DrawCircle(screen_width/2, screen_height/2, 100, GREEN);
                DrawTextButton(&restart_button);
                break;

            case s_game_over:
                DrawGameGrid(&grid);
                DrawCircle(screen_width/2, screen_height/2, 100, RED);
                DrawTextButton(&restart_button);
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