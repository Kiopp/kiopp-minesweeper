#ifndef GRID_H
#define GRID_H

#include "Tile.h"
#include "Button.h"
#include "kiopplib.h"

typedef struct GameGrid {
    Tile** grid;
} GameGrid;

Tile** CreateGrid(int screen_width, int screen_height, int tile_width, TileMapTexture* tilemap, int dim_x, int dim_y, int num_mines);

void DrawGameGrid(Tile** grid, int grid_width, int grid_height, int startX, int startY, int grid_rows, int grid_cols, int font_size, int scale);

#endif