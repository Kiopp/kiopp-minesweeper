#include "raylib.h"
#include <stdio.h>
#include "Tile.h"

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

Tile CreateTile(int screen_width, int screen_height, int width, int height, Texture2D image, enum tile_type type, Vector2 position)
{
    // Declare new tile
    Tile tile;

    // Assign values to tile
    tile.button = CreateImageButton(screen_width, screen_height, width, height, image);
    tile.button.rec.x = position.x;
    tile.button.rec.y = position.y;
    tile.type = type;
    
    tile.state = unexplored; // Default
    tile.mine_num = 0; // Set 0 mines default

    return tile;
}

Texture2D* GetTileImage(Tile* tile, TileMapTexture* textures){
    if (tile->state == unexplored) {
        return &textures->tileMap[1]; // Unexplored
    }
    else if (tile->state == flag) {
        return &textures->tileMap[0]; // Flag
    }
    else if (tile->state == explored) {
        if (tile->type == mine) return &textures->tileMap[3]; // Mine

        return &textures->tileMap[2]; // Empty
    } 
    return &textures->tileMap[1]; // Unexplored
}

void DrawTile(Tile* tile, int font_size, int scale)
{
    DrawImageButton(&tile->button, scale);

    if (tile->mine_num > 0 && tile->state == explored) {
        Color text_color = BLACK;
        switch (tile->mine_num) {
            case 1:
                text_color = BLUE;
                break;
            case 2:
                text_color = GREEN;
                break;
            case 3:
                text_color = RED;
                break;
            case 4:
                text_color = DARKBLUE;
                break;
            case 5:
                text_color = MAROON;
                break;
            case 6:
                text_color = SKYBLUE;
                break;
            case 7:
                text_color = PURPLE;
                break;
            case 8:
                text_color = GRAY;
                break;
            default:
                break;
        }

        char number[2] = "\0";
        sprintf(number, "%d", tile->mine_num);

        Rectangle dest = (Rectangle){ tile->button.rec.x, tile->button.rec.y, tile->button.rec.width * scale, tile->button.rec.height * scale };

        // Calculate character position for centering
        int textWidth = MeasureText(number, font_size);
        int textX = dest.x + (dest.width - textWidth) / 2;
        int textY = dest.y + (dest.height - font_size) / 2;
        // Draw the character
        DrawText(number, textX, textY, font_size, text_color); 
    }
}