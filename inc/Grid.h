#ifndef GRID_H
#define GRID_H

#include "Tile.h"
#include "Button.h"

typedef struct GameGrid {
    Tile** tiles;
    int cols;
    int rows;
    int tile_size;
    int scale;
    int tile_font;
    int width;
    int height;
    int start_x;
    int start_y;
} GameGrid;

GameGrid CreateGrid(int screen_width, int screen_height, int tile_size, TileMapTexture* textures, int grid_cols, int grid_rows, int num_mines, int scale, int tile_font);

void HandleGridTileButtons(GameGrid* grid);

void HandleGridTileButtonClicked(GameGrid* grid, TileMapTexture* textures);

void DrawGameGrid(GameGrid* grid);

void UpdateTiles(GameGrid* grid, int row, int col);

#endif
