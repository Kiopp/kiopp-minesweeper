#ifndef TILE_H
#define TILE_H

#include "kiopplib.h"
#include "raylib.h"
#include "Button.h"

enum tile_type{
    empty,
    mine
};

enum tile_state{
    unexplored,
    explored,
    flag
};

typedef struct Tile {
    ImageButton button;
    enum tile_type type;
    enum tile_state state;
    int mine_num;
} Tile;

typedef struct TileMapTexture{
    Texture2D tileMap[4];
    int num_tiles;
} TileMapTexture;

TileMapTexture SplitTileMap(Texture2D tilemap);

Texture2D* GetTileImage(Tile* tile, TileMapTexture* textures);

Tile CreateTile(int screen_width, int screen_height, int width, int height, Texture2D image, enum tile_type type);

void DrawTile(Tile* tile, Tuple_int position, int font_size, int scale);

#endif