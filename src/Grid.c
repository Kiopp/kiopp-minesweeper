#include "../inc/Grid.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

GameGrid CreateGrid(int screen_width, int screen_height, int tile_size, TileMapTexture* textures, int grid_cols, int grid_rows, int num_mines, int scale, int tile_font)
{
    // Allocate TileGrid memory
    Tile** grid = (Tile**)malloc(sizeof(Tile*)*grid_cols);
    if (grid == NULL) { printf("Memory allocation for grid failed!\n"); exit(1); }

    for (size_t i = 0; i < grid_cols; i++) {
        grid[i] = (Tile*)malloc(sizeof(Tile)*grid_rows);
        if (grid[i] == NULL) { printf("Memory allocation for grid[i] failed!\n"); exit(1); }
    }

    // Allocate tuple memory
    Vector2* mine_indices = (Vector2*)malloc(sizeof(Vector2)*num_mines);
    if (mine_indices == NULL) { printf("Memory allocation for mine_indices failed!\n"); exit(1); }

    // Calculate total grid width and height
    int grid_width = grid_cols * tile_size * scale;
    int grid_height = grid_rows * tile_size * scale;

    // Calculate starting position for the grid
    int start_x = screen_width/2 - grid_width/2;
    int start_y = screen_height/2 - grid_height/2;

    // Randomize mine locations
    srand(time(0));

    // Keep track of used points to avoid duplicates
    int *used_points = (int *)malloc(grid_cols * grid_rows * sizeof(int)); 
    for (int i = 0; i < grid_cols * grid_rows; i++) {
        used_points[i] = 0; // Initialize all points as unused
    }

    for (size_t i = 0; i < num_mines; i++) {
        int index;
        int x, y;
        do {
            // Generate random coordinates within the grid
            x = rand() % grid_cols;
            y = rand() % grid_rows;
            index = y * grid_cols + x; // Calculate a unique index for each (x, y) pair
        } while (used_points[index]); // Keep generating until an unused point is found

        // Mark the point as used
        used_points[index] = 1; 

        // Assign coordinates
        mine_indices[i].x = x;
        mine_indices[i].y = y;
    }

    // Create tiles
    for (size_t x = 0; x < grid_cols; x++) {
        for (size_t y = 0; y < grid_rows; y++) {
            // Declare and initialize variables
            enum tile_type type = empty;
            enum tile_state state = unexplored;

            // Check if tile should be mine
            for (size_t i = 0; i < num_mines; i++) {
                if (mine_indices[i].x == x && mine_indices[i].y == y) {
                    type = mine;
                }
            }

            // Create tile
            grid[x][y] = CreateTile(
                screen_width, 
                screen_height, 
                tile_size, 
                tile_size, 
                textures->tileMap[1], 
                type,
                (Vector2){
                                    start_x + x * (tile_size * scale), 
                                    start_y + y * (tile_size * scale)
                                    }
                );
        }
    }
    free(mine_indices);
    free(used_points);

    // Construct GameGrid
    GameGrid gameGrid;
    gameGrid.tiles = grid;
    gameGrid.cols = grid_cols;
    gameGrid.rows = grid_cols;
    gameGrid.scale = scale;
    gameGrid.tile_size = tile_size;
    gameGrid.tile_font = tile_font;
    gameGrid.start_x = start_x;
    gameGrid.start_y = start_y;
    gameGrid.width = grid_width;
    gameGrid.height = grid_height;

    return gameGrid;
}

void HandleGridTileButtons(GameGrid* grid){
    for (size_t x = 0; x < grid->cols; x++) {
        for (size_t y = 0; y < grid->rows; y++) {
            HandleImageButtonPress(&grid->tiles[x][y].button, grid->scale);
        }
    }
}

void HandleGridTileButtonClicked(GameGrid* grid, TileMapTexture* textures){
    for (size_t x = 0; x < grid->cols; x++) {
        for (size_t y = 0; y < grid->rows; y++) {
            if (grid->tiles[x][y].button.button_pressed == 1) {
                grid->tiles[x][y].button.button_pressed = 0;
                printf("Clicked on tile[%zu][%zu]\n", x, y);
                grid->tiles[x][y].state = explored;
                if (grid->tiles[x][y].type == mine) {
                    grid->tiles[x][y].button.image = textures->tileMap[3]; // Mine tile
                }
                else {
                    grid->tiles[x][y].button.image = textures->tileMap[2]; // Empty tile
                }
            }
        }
    }
}

void DrawGameGrid(GameGrid* grid){
    for (int row = 0; row < grid->rows; row++){
        for (int col = 0; col < grid->cols; col++){
            DrawTile(&grid->tiles[col][row], grid->tile_font, grid->scale);
        }
    }
}