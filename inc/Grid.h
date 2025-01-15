#ifndef GRID_H
#define GRID_H

#include "Tile.h"
#include "Button.h"
#include "kiopplib.h"

typedef struct GameGrid {
    Tile** tiles;
    int cols;
    int rows;
    int tile_size;
    int scale;
    int tile_font;
} GameGrid;

GameGrid CreateGrid(int screen_width, int screen_height, int tile_size, TileMapTexture* textures, int grid_cols, int grid_rows, int num_mines, int scale, int tile_font);

void DrawGameGrid(GameGrid grid, int grid_width, int grid_height, int startX, int startY);

#endif