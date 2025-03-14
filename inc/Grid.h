#ifndef GRID_H
#define GRID_H

#include "Tile.h"
#include "raylib.h"

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
    int game_over;
    int game_win;
} GameGrid;

GameGrid* CreateGrid(int screen_width, int screen_height, int tile_size, TileMapTexture* textures, int grid_cols, int grid_rows, int num_mines, int scale, int tile_font, int isDarkmode);

void DestroyGrid(GameGrid* grid);

void HandleGridTileButtons(GameGrid* grid, Camera2D camera);

void HandleGridTileButtonClicked(GameGrid* grid, TileMapTexture* textures);

void DrawGameGrid(GameGrid* grid, int screen_width, int screen_height, float zoom, Camera2D camera);

void ExploreTile(GameGrid* grid, TileMapTexture* textures, int row, int col);

int CheckWinCondition(GameGrid* grid);

#endif
