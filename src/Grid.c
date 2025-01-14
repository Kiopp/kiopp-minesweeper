#include "../inc/Grid.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../inc/kiopplib.h"

Tile** CreateGrid(int screen_width, int screen_height, int tile_width, TileMapTexture* textures, int dim_x, int dim_y, int num_mines)
{
    // Allocate TileGrid memory
    Tile** grid = (Tile**)malloc(sizeof(Tile*)*dim_x);
    if (grid == NULL) { printf("Memory allocation for grid failed!\n"); exit(1); }

    for (size_t i = 0; i < dim_x; i++) {
        grid[i] = (Tile*)malloc(sizeof(Tile)*dim_y);
        if (grid[i] == NULL) { printf("Memory allocation for grid[i] failed!\n"); exit(1); }
    }

    // Allocate tuple memory
    Tuple_int* mine_indices = (Tuple_int*)malloc(sizeof(Tuple_int)*num_mines);
    if (mine_indices == NULL) { printf("Memory allocation for mine_indices failed!\n"); exit(1); }

    

    // Randomize mine locations
    srand(time(0));

    // Keep track of used points to avoid duplicates
    int *used_points = (int *)malloc(dim_x * dim_y * sizeof(int)); 
    for (int i = 0; i < dim_x * dim_y; i++) {
        used_points[i] = 0; // Initialize all points as unused
    }

    for (size_t i = 0; i < num_mines; i++) {
        int index;
        int x, y;
        do {
            // Generate random coordinates within the grid
            x = rand() % dim_x;
            y = rand() % dim_y;
            index = y * dim_x + x; // Calculate a unique index for each (x, y) pair
        } while (used_points[index]); // Keep generating until an unused point is found

        // Mark the point as used
        used_points[index] = 1; 

        // Assign coordinates
        mine_indices[i].x = x;
        mine_indices[i].y = y;
    }

    // Create tiles
    for (size_t x = 0; x < dim_x; x++) {
        for (size_t y = 0; y < dim_y; y++) {
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
                tile_width, 
                tile_width, 
                textures->tileMap[1], 
                type
                );
            printf("Created grid[%zu][%zu]\n", x, y);
        }
    }
    free(mine_indices);
    free(used_points);

    return grid;
}

void DrawGameGrid(Tile** grid, int grid_width, int grid_height, int startX, int startY, int grid_rows, int grid_cols, int font_size, int scale){

    int tile_size = grid[0]->button.rec.width;
    // Draw the grid of rectangles
    for (int row = 0; row < grid_rows; row++)
    {
        for (int col = 0; col < grid_cols; col++)
        {
            int x = startX + col * (tile_size * scale);
            int y = startY + row * (tile_size * scale);
            Tuple_int position = (Tuple_int){x, y};
            DrawTile(&grid[col][row], position, font_size, scale);
        }
    }
}