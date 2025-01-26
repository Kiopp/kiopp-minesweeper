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

enum grid_setting_ix{
    e_cols,
    e_rows,
    e_mines,
    e_scale,
    e_font
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

void UpdateAllNumberBox(NumberBox* num_mines_input, NumberBox* width_input, NumberBox* height_input, NumberBox* scale_input){
    int mouse_on_any_text = 0;

    // Update mouse_on_text and focused
    CheckMouseHover(num_mines_input);
    if (num_mines_input->mouse_on_text) mouse_on_any_text = 1;
    if (num_mines_input->clicked) {
        num_mines_input->clicked = 0;
        num_mines_input->focused = !num_mines_input->focused;

        if (num_mines_input->focused) {
            // Unfocus all other numboxes
            width_input->focused = 0;
            height_input->focused = 0;
            scale_input->focused = 0;
        }
    }
    
    
    CheckMouseHover(width_input);
    if (width_input->mouse_on_text) mouse_on_any_text = 1;
    if (width_input->clicked) {
        width_input->clicked = 0;
        width_input->focused = !width_input->focused;

        if (width_input->focused) {
            // Unfocus all other numboxes
            num_mines_input->focused = 0;
            height_input->focused = 0;
            scale_input->focused = 0;
        }
    }

    CheckMouseHover(height_input);
    if (height_input->mouse_on_text) mouse_on_any_text = 1;
    if (height_input->clicked) {
        height_input->clicked = 0;
        height_input->focused = !height_input->focused;

        if (height_input->focused) {
            // Unfocus all other numboxes
            num_mines_input->focused = 0;
            width_input->focused = 0;
            scale_input->focused = 0;
        }
    }

    CheckMouseHover(scale_input);
    if (scale_input->mouse_on_text) mouse_on_any_text = 1;
    if (scale_input->clicked) {
        scale_input->clicked = 0;
        scale_input->focused = !scale_input->focused;

        if (scale_input->focused) {
            // Unfocus all other numboxes
            num_mines_input->focused = 0;
            width_input->focused = 0;
            height_input->focused = 0;
        }
    }

    // Update NumberBoxes
    UpdateNumberBox(num_mines_input);
    UpdateNumberBox(width_input);
    UpdateNumberBox(height_input);
    UpdateNumberBox(scale_input);

    // Update mouse cursor
    if (mouse_on_any_text) {
        // Set the window's cursor to the I-Beam
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    
}

void ResetGame(int screen_width, int screen_height, int* grid_settings, TextButton* start_button, NumberBox* num_mines_input, NumberBox* width_input, NumberBox* height_input, NumberBox* scale_input){
    
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
    int row1_offset = 120;
    int row2_offset = 220;

    int col1_offset = -200;
    int col2_offset = 0;
    int col3_offset = 200;

    char mine_label[50] = "Mines";
    *num_mines_input = CreateNumberBox(
        screen_width, 
        screen_height, 
        100, 
        40, 
        col2_offset,
        row1_offset,
        font_size, 
        grid_settings[e_mines],
        mine_label
        );

    char width_label[50] = "Width";
    *width_input = CreateNumberBox(
        screen_width, 
        screen_height, 
        100, 
        40, 
        col1_offset,
        row1_offset,
        font_size, 
        grid_settings[e_cols],
        width_label
        );

    char height_label[50] = "Height";
    *height_input = CreateNumberBox(
        screen_width, 
        screen_height, 
        100, 
        40, 
        col3_offset,
        row1_offset,
        font_size, 
        grid_settings[e_rows],
        height_label
        );

    // ----------------------------------------------------------
    
    char scale_label[50] = "Scale";
    *scale_input = CreateNumberBox(
        screen_width, 
        screen_height, 
        100, 
        40, 
        col2_offset,
        row2_offset,
        font_size, 
        grid_settings[e_scale],
        scale_label
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

    // Misc
    int flag_enable = 0;

    // Buttons
    char btn_restart_text[32] = "Retry";
    TextButton button;
    TextButton restart_button;

    // Tile
    Texture2D tilesheet = LoadTexture("../assets/kiopp_minesweeper_tilesheet.png");
    TileMapTexture textures = SplitTileMap(tilesheet);

    // TextBox
    NumberBox num_mines_input;
    NumberBox width_input;
    NumberBox height_input;
    NumberBox scale_input;

    // Grid
    GameGrid grid;
    int tile_size = tilesheet.width/2;
    int grid_cols = 1;
    int grid_rows = 1;
    int num_mines = 1;
    int scale = 1;
    int tile_font = 1;
    int grid_settings[5] = {9 ,9, 10,4,40};

    ResetGame(
        screen_width, 
        screen_height, 
        grid_settings,
        &button, 
        &num_mines_input, 
        &width_input, 
        &height_input, 
        &scale_input
        );

    while(!WindowShouldClose()){

        // Update state mashine
        switch (state) {
            case s_setup:
                HandleTextButtonPress(&button);
                UpdateAllNumberBox(&num_mines_input, &width_input, &height_input, &scale_input);

                if (button.button_pressed) {
                    // Grid settings
                    grid_cols = width_input.value;
                    grid_rows = height_input.value;
                    num_mines = num_mines_input.value;
                    scale = scale_input.value;
                    tile_font = 10 * scale;

                    // Save settings
                    grid_settings[e_cols] = grid_cols;
                    grid_settings[e_rows] = grid_rows;
                    grid_settings[e_mines] = num_mines;
                    grid_settings[e_scale] = scale;
                    grid_settings[e_font] = tile_font;

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
                    ResetGame(
                        screen_width, 
                        screen_height, 
                        grid_settings,
                        &button, 
                        &num_mines_input, 
                        &width_input, 
                        &height_input, 
                        &scale_input
                        );
                }
                break;

            case s_win:
                HandleTextButtonPress(&restart_button);

                if (restart_button.button_pressed) {
                    restart_button.button_pressed = 0;
                    state = s_setup;

                    CheckWindowSize(&screen_width, &screen_height);
                    ResetGame(
                        screen_width, 
                        screen_height, 
                        grid_settings,
                        &button, 
                        &num_mines_input, 
                        &width_input, 
                        &height_input, 
                        &scale_input
                        );
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
                DrawNumberBox(&scale_input);
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