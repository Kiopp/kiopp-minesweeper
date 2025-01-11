#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include "../inc/Tile.h"

TileMapTexture SplitTileMap(Texture2D tilesheet)
{
    // Declare new TileMapTexture
    TileMapTexture tilemap_texture;

    // Calculate tile dimensions
    int tileWidth = tilesheet.width / 2;  // 2 tiles wide
    int tileHeight = tilesheet.height / 2; // 2 tiles high

    // Get the image data from the texture
    Image sheetImage = LoadImageFromTexture(tilesheet); 

    // Split the tilesheet into individual tiles
    for (int y = 0; y < 2; y++)
    {
        for (int x = 0; x < 2; x++)
        {
            // Calculate the source rectangle for the current tile
            Rectangle sourceRec = { x * tileWidth, y * tileHeight, tileWidth, tileHeight };

            // Create a new texture for the tile and copy the data
            Image tileImage = ImageFromImage(sheetImage, sourceRec);
            tilemap_texture.tileMap[y * 2 + x] = LoadTextureFromImage(tileImage);
            UnloadImage(tileImage); // Unload image data after loading texture
        }
    }

    UnloadImage(sheetImage);

    return tilemap_texture;
}

Tile CreateTile(int screen_width, int screen_height, int width, int height, Texture2D image, enum tile_type type)
{
    // Declare new tile
    Tile tile;

    // Assign values to tile
    tile.button = CreateImageButton(screen_width, screen_height, width, height, image);
    tile.type = type;

    tile.state = unexplored; // Default
    tile.mine_num = 0; // Set 0 mines default

    return tile;
}

void DrawTile(Tile* tile)
{
    DrawImageButton(&tile->button);
}