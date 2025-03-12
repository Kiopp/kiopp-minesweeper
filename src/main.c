#include "TextBox.h"
#include "raylib.h"
#include "raymath.h"
#include "Button.h"
#include "Tile.h"
#include "Grid.h"
#include "TextBox.h"
#include "tilesheet.h"
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
    if (*screen_height < 1280) {
        *screen_height = 1280;
    }

    // Update window size
    SetWindowSize(*screen_width, *screen_height);
}

void UpdateAllNumberBox(NumberBox* num_mines_input, NumberBox* grid_size_input, NumberBox* scale_input){
    int mouse_on_any_text = 0;

    // Update mouse_on_text and focused
    CheckMouseHover(num_mines_input);
    if (num_mines_input->mouse_on_text) mouse_on_any_text = 1;
    if (num_mines_input->clicked) {
        num_mines_input->clicked = 0;
        num_mines_input->focused = !num_mines_input->focused;

        if (num_mines_input->focused) {
            // Unfocus all other numboxes
            grid_size_input->focused = 0;
            scale_input->focused = 0;
        }
    }
    
    
    CheckMouseHover(grid_size_input);
    if (grid_size_input->mouse_on_text) mouse_on_any_text = 1;
    if (grid_size_input->clicked) {
        grid_size_input->clicked = 0;
        grid_size_input->focused = !grid_size_input->focused;

        if (grid_size_input->focused) {
            // Unfocus all other numboxes
            num_mines_input->focused = 0;
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
            grid_size_input->focused = 0;
        }
    }

    // Update NumberBoxes
    UpdateNumberBox(num_mines_input);
    UpdateNumberBox(grid_size_input);
    UpdateNumberBox(scale_input);

    // Update mouse cursor
    if (mouse_on_any_text) {
        // Set the window's cursor to the I-Beam
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    
}

void ResetGame(int screen_width, int screen_height, int* grid_settings, TextButton* start_button, NumberBox* num_mines_input, NumberBox* grid_size_input, NumberBox* scale_input){
    
    // Start button
    char btn_text[32] = "Start";
    *start_button = CreateTextButton( 
        screen_width, 
        screen_height, 
        btn_text, 
        50,
        20
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

    char grid_label[50] = "Gridsize";
    *grid_size_input = CreateNumberBox(
        screen_width, 
        screen_height, 
        100, 
        40, 
        col1_offset,
        row1_offset,
        font_size, 
        grid_settings[e_cols],
        grid_label
        );

    char scale_label[50] = "Scale";
    *scale_input = CreateNumberBox(
        screen_width, 
        screen_height, 
        100, 
        40, 
        col3_offset,
        row1_offset,
        font_size, 
        grid_settings[e_scale],
        scale_label
        );
}

int main()
{
    // Window setup
    const int max_screen_width = 1280;
    const int max_screen_height = 1280;
    int screen_width = 1280;
    int screen_height = 720;
    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
    InitWindow(screen_width, screen_height, "Kiopp Minesweeper");
    SetTargetFPS(60);

    // Game setup
    enum gameState state = s_setup;

    // Camera
    Camera2D camera = {
        .offset = {0, 0},
        .target = {0, 0},
        .rotation = 0,
        .zoom = 1.0f
    };
    float zoom = 1.0f;

    // Fullscreen
    bool isFullscreen = false;

    // Buttons
    char btn_restart_text[32] = "Retry";
    TextButton button;
    TextButton restart_button;

    // Tile
    Image image = { 
        .data = image_data, 
        .width = image_width, 
        .height = image_height, 
        .mipmaps = 1, 
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 
    };
    Texture2D tilesheet = LoadTextureFromImage(image);
    TileMapTexture textures = SplitTileMap(tilesheet);

    // TextBox
    NumberBox num_mines_input;
    NumberBox grid_size_input;
    NumberBox scale_input;

    // Grid
    GameGrid* grid;
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
        &grid_size_input, 
        &scale_input
        );

    while(!WindowShouldClose()){
        // Fullscreen Toggle (always available)
        if (IsKeyPressed(KEY_F11)) {
            isFullscreen = !isFullscreen;
            if (isFullscreen) {
                int monitor = GetCurrentMonitor();
                SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
            } else {
                SetWindowSize(1280, 720);
            }
            ToggleFullscreen();
        }

        if(IsWindowResized()){
            screen_width = GetScreenWidth();
            screen_height = GetScreenHeight();
        }
        

        // Update state mashine
        switch (state) {
            case s_setup:
                HandleTextButtonPress(&button);
                UpdateAllNumberBox(&num_mines_input, &grid_size_input, &scale_input);

                if (button.button_pressed) {
                    // Grid settings
                    grid_cols = grid_size_input.value;
                    grid_rows = grid_size_input.value;
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
                // Panning
                if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
                    Vector2 mouseDelta = {
                        GetMouseDelta().x,
                        GetMouseDelta().y
                    };
                    camera.offset.x += mouseDelta.x;
                    camera.offset.y += mouseDelta.y;
                }

                // Zoom
                float wheelMove = GetMouseWheelMove();
                if (wheelMove != 0) {
                    Vector2 mouse_pos_before_zoom = GetScreenToWorld2D(GetMousePosition(), camera); // Mouse position before zoom
                    float zoomIncrement = 0.05f;
                    if (wheelMove > 0) {
                        zoom += zoomIncrement;
                    } else {
                        zoom -= zoomIncrement;
                    }
                    if (zoom < 0.5f) zoom = 0.5f;
                    if (zoom > 3.0f) zoom = 3.0f;

                    camera.zoom = zoom;

                    Vector2 mouse_pos_after_zoom = GetScreenToWorld2D(GetMousePosition(), camera); // Mouse position after zoom
                    Vector2 offset_diff = Vector2Subtract(mouse_pos_before_zoom, mouse_pos_after_zoom); // Calculate difference
                    
                    camera.target = Vector2Add(camera.target, offset_diff); // Adjust camera offset
                }

                // Handle tiles
                HandleGridTileButtons(grid, camera);
                HandleGridTileButtonClicked(grid, &textures);

                // Check game over condition
                if (grid->game_over == 1) {
                    state = s_game_over;
                    restart_button = CreateTextButton( screen_width, screen_height, btn_restart_text, 50, 20);
                }
                if (grid->game_win == 1) {
                    state = s_win;
                    restart_button = CreateTextButton( screen_width, screen_height, btn_restart_text, 50, 20);
                } 
                
                break;

            case s_game_over:
                HandleTextButtonPress(&restart_button);

                if (restart_button.button_pressed) {
                    restart_button.button_pressed = 0;
                    state = s_setup;

                    ResetGame(
                        screen_width, 
                        screen_height, 
                        grid_settings,
                        &button, 
                        &num_mines_input, 
                        &grid_size_input, 
                        &scale_input
                        );
                }
                break;

            case s_win:
                HandleTextButtonPress(&restart_button);

                if (restart_button.button_pressed) {
                    restart_button.button_pressed = 0;
                    state = s_setup;

                    ResetGame(
                        screen_width, 
                        screen_height, 
                        grid_settings,
                        &button, 
                        &num_mines_input, 
                        &grid_size_input, 
                        &scale_input
                        );
                }
                break;

            default:
                break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Drawing state mashine
        switch (state) 
        {
            case s_setup:
                DrawTextButton(&button, screen_width, screen_height);
                DrawNumberBox(&num_mines_input, screen_width, screen_height);
                DrawNumberBox(&grid_size_input, screen_width, screen_height);
                DrawNumberBox(&scale_input, screen_width, screen_height);
                break;

            case s_playing:
                // Apply camera offset and zoom
                BeginMode2D(camera);
            
                DrawGameGrid(grid, zoom);
                EndMode2D();
                break;

            case s_win:
                DrawGameGrid(grid, zoom);
                DrawCircle(screen_width/2, screen_height/2, 100, GREEN);
                DrawTextButton(&restart_button, screen_width, screen_height);
                break;

            case s_game_over:
                DrawGameGrid(grid, zoom);
                DrawCircle(screen_width/2, screen_height/2, 100, RED);
                DrawTextButton(&restart_button, screen_width, screen_height);
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