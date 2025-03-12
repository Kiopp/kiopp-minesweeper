#include "Button.h"
#include "raylib.h"
#include <string.h>

TextButton CreateTextButton(int screenWidth, int screenHeight, char text[32], int font_size, int padding) 
{
    // Declare new Button
    TextButton button;

    // Initialize Button values
    strcpy(button.text, text);
    button.font_size = font_size;
    button.text_width = MeasureText(button.text, button.font_size);
    button.button_color = GRAY;
    button.button_pressed = 0;
    button.text_padding = padding;

    button.rec = (Rectangle){
        0, 
        0, 
        button.text_width + padding, 
        font_size + padding
        };

    return button;
}

ImageButton CreateImageButton(int screen_width, int screen_height, int button_width, int button_height, Texture2D image) 
{
    // Declare new Button
    ImageButton button;

    // Initialize Button values
    button.image = image;
    button.button_color = WHITE; // No image hue change
    button.button_pressed = 0;
    button.toggle_flag = 0;

    button.rec = (Rectangle){
        0, // Default position
        0, // Default position
        button_width, 
        button_height
        };

    return button;
}

void HandleTextButtonPress(TextButton* button)
{
    if (CheckCollisionPointRec(GetMousePosition(), button->rec)) {
        button->button_color = LIGHTGRAY;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            button->button_color = DARKGRAY;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            button->button_pressed = 1;
        }
    } else {
        button->button_color = GRAY;
    }
}

void HandleImageButtonPress(ImageButton* button, int scale, Camera2D camera)
{
    Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera); // Transform mouse position
    Rectangle dest = (Rectangle){ 
        button->rec.x * camera.zoom, 
        button->rec.y * camera.zoom, 
        button->rec.width * scale * camera.zoom, 
        button->rec.height * scale * camera.zoom
    };
    if (CheckCollisionPointRec(mouse_pos, dest)) {
        button->button_color = LIGHTGRAY;
        // Left mouse button click
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            button->button_color = DARKGRAY;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            button->button_pressed = 1;
        }

        // Right mouse button click
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            button->button_color = DARKGRAY;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            button->toggle_flag = 1;
        }
    } else {
        button->button_color = WHITE;
    }
}

void DrawTextButton(TextButton* button, int screen_width, int screen_height)
{
    // Update button position
    button->rec.x = (float)screen_width/2 - (float)button->text_width/2 - (float)button->text_padding/2;
    button->rec.y = (float)screen_height/2 - (float)button->font_size/2 - (float)button->text_padding/2;

    // Draw button
    DrawRectangleRec(button->rec, button->button_color);

    // Draw text
    int text_x = button->rec.x + (button->rec.width - button->text_width) / 2;
    int text_y = button->rec.y + (button->rec.height - button->font_size) / 2;
    DrawText(
        button->text, 
        text_x, 
        text_y, 
        button->font_size, 
        BLACK
        );
}

void DrawImageButton(ImageButton* button, int scale, float zoom)
{
    Rectangle dest = (Rectangle){ 
        button->rec.x * zoom, 
        button->rec.y * zoom, 
        button->rec.width * scale * zoom, 
        button->rec.height * scale * zoom 
    };
    DrawTexturePro(
        button->image,
        (Rectangle){0,0, button->rec.width, button->rec.height},
        dest,
        (Vector2){0,0},
        0.0f,
        button->button_color
        );
}