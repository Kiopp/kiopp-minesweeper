#include "Grid.h"
#include "Tile.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

GameGrid* CreateGrid(int screen_width, int screen_height, int tile_size, TileMapTexture* textures, int grid_cols, int grid_rows, int num_mines, int scale, int tile_font, int isDarkMode) {
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
                                    (long long)(start_x + x * (tile_size * scale)), 
                                    (long long)(start_y + y * (tile_size * scale))
                                    },
                isDarkMode
                );
        }
    }

    // Update num_mines for all tiles close to mines
    for (size_t i = 0; i < num_mines; i++) {
        // Loop through the 3x3 area surrounding each mine
        for (int j = mine_indices[i].x - 1; j <= mine_indices[i].x + 1; j++) {
            for (int k = mine_indices[i].y - 1; k <= mine_indices[i].y + 1; k++) {
                // Skip the cell itself
                if (j == mine_indices[i].x && k == mine_indices[i].y) {
                    continue;
                }
                // Bounds checking to avoid accessing elements outside the array and skipping mines
                if (j >= 0 && j < grid_cols && k >= 0 && k < grid_rows && grid[j][k].type == empty) {
                    grid[j][k].mine_num++;
                }
            }
        }   
    }

    free(mine_indices);
    free(used_points);

    // Construct GameGrid
    GameGrid* gameGrid = (GameGrid*)malloc(sizeof(GameGrid));
    if (gameGrid == NULL) { printf("Memory allocation for gameGrid failed!\n"); exit(1); }
    gameGrid->tiles = grid;
    gameGrid->cols = grid_cols;
    gameGrid->rows = grid_rows;
    gameGrid->scale = scale;
    gameGrid->tile_size = tile_size;
    gameGrid->tile_font = tile_font;
    gameGrid->start_x = start_x;
    gameGrid->start_y = start_y;
    gameGrid->width = grid_width;
    gameGrid->height = grid_height;
    gameGrid->game_over = 0;
    gameGrid->game_win = 0;

    return gameGrid;
}

void DestroyGrid(GameGrid* grid) {
    if (grid) {
        if (grid->tiles) {
            for (int i = 0; i < grid->cols; i++) {
                free(grid->tiles[i]);
            }
            free(grid->tiles);
        }
        free(grid);
    }
}

void ToggleFlag(Tile* tile, TileMapTexture* textures){
    if (tile->state == flag) {
        tile->button.image = textures->tileMap[1]; // Unexplored
        tile->state = unexplored;
    } else {
        tile->button.image = textures->tileMap[0]; // Flag
        tile->state = flag;
    }
}

void HandleGridTileButtons(GameGrid* grid, Camera2D camera){
    for (size_t x = 0; x < grid->cols; x++) {
        for (size_t y = 0; y < grid->rows; y++) {
            HandleImageButtonPress(&grid->tiles[x][y].button, grid->scale, camera);
        }
    }
}

void HandleGridTileButtonClicked(GameGrid* grid, TileMapTexture* textures){
    for (size_t x = 0; x < grid->cols; x++) {
        for (size_t y = 0; y < grid->rows; y++) {
            // Right click flag toggle
            if (grid->tiles[x][y].button.toggle_flag == 1) {
                grid->tiles[x][y].button.toggle_flag = 0;

                // Check if click can place a flag
                if (grid->tiles[x][y].state == unexplored || grid->tiles[x][y].state == flag) {
                    ToggleFlag(&grid->tiles[x][y], textures);
                }
            }
            
            if (grid->tiles[x][y].button.button_pressed == 1) {
                grid->tiles[x][y].button.button_pressed = 0;

                // Check if click should place a flag
                if (IsKeyDown(KEY_LEFT_SHIFT) && (grid->tiles[x][y].state == unexplored || grid->tiles[x][y].state == flag)) {
                    ToggleFlag(&grid->tiles[x][y], textures);
                } else {
                    ExploreTile(grid, textures, x, y);

                    if (CheckWinCondition(grid)) {
                        grid->game_win = 1;
                    }
                }
            }
        }
    }
}

void DrawGameGrid(GameGrid* grid, int screen_width, int screen_height, float zoom, Camera2D camera){
    // Determine the camera's viewport in world coordinates.
    Vector2 topLeft = GetScreenToWorld2D((Vector2){ 0, 0 }, camera);
    Vector2 bottomRight = GetScreenToWorld2D((Vector2){ screen_width, screen_height }, camera);

    // Adjust for zoom to get the correct viewport size
    Rectangle viewport = {
        topLeft.x,
        topLeft.y,
        bottomRight.x - topLeft.x,
        bottomRight.y - topLeft.y
    };

    for (int row = 0; row < grid->rows; row++){
        for (int col = 0; col < grid->cols; col++){
            Tile* tile = &grid->tiles[col][row];

            // Check if the tile's bounding box intersects with the viewport.
            Rectangle tileRect = {
                tile->button.rec.x * camera.zoom,
                tile->button.rec.y * camera.zoom,
                tile->button.rec.width * grid->scale * camera.zoom,
                tile->button.rec.height * grid->scale * camera.zoom
            };

            if (CheckCollisionRecs(tileRect, viewport)) {
                // Only draw tiles that intersect.
                DrawTile(tile, grid->tile_font, grid->scale, zoom);
            }
        }
    }
}

// I know the row and col is flipped here and is weird. I made a mistake, but it works so I left it...
void ExploreTile(GameGrid* grid, TileMapTexture* textures, int row, int col){
    // If the tile is already revealed or flagged, do nothing
    if (grid->tiles[row][col].state == explored || grid->tiles[row][col].state == flag) {
        return;
    }

    // Check if explored tile is mine
    if (grid->tiles[row][col].type == mine) {
        grid->tiles[row][col].button.image = textures->tileMap[3]; // Mine tile
        grid->tiles[row][col].state = explored;
        grid->game_over = 1;
        return;
    }

    // Reveal the tile
    grid->tiles[row][col].state = explored;
    grid->tiles[row][col].button.image = textures->tileMap[2];

    // If the tile has no neighboring mines, recursively reveal its neighbors
    if (grid->tiles[row][col].mine_num == 0) {
        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = col - 1; j <= col + 1; j++) {
                // Check boundaries and avoid revealing the clicked tile again
                if (i >= 0 && i < grid->rows && j >= 0 && j < grid->cols && !(i == row && j == col)) {
                    ExploreTile(grid, textures, i, j);
                }
            }
        }
    }
}

int CheckWinCondition(GameGrid* grid){
    int unrevealed_tiles = 0;
    for (int i = 0; i < grid->cols; i++) {
        for (int j = 0; j < grid->rows; j++) {
            // Check if tile is flagged or unexplored
            if ((grid->tiles[i][j].state == flag || grid->tiles[i][j].state == unexplored) && grid->tiles[i][j].type == empty) {
                unrevealed_tiles++;
            }
        }
    }

    if (unrevealed_tiles == 0) {
        return 1;
    } else {
        return 0;
    }
}