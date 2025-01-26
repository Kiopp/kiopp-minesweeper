#include "Button.h"
#include "raylib.h"
#include <string.h>

TextButton CreateTextButton(int screenWidth, int screenHeight, char text[32], int font_size) 
{
    // Declare new Button
    TextButton button;

    // Initialize Button values
    strcpy(button.text, text);
    button.font_size = font_size;
    button.text_width = MeasureText(button.text, button.font_size);
    button.button_color = GRAY;
    button.button_pressed = 0;

    int padding = 20;

    button.rec = (Rectangle){
        (float)screenWidth/2 - (float)button.text_width/2 - (float)padding/2, 
        (float)screenHeight/2 - (float)font_size/2 - (float)padding/2, 
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

void HandleImageButtonPress(ImageButton* button, int scale)
{
    Rectangle dest = (Rectangle){ button->rec.x, button->rec.y, button->rec.width * scale, button->rec.height * scale };
    if (CheckCollisionPointRec(GetMousePosition(), dest)) {
        button->button_color = LIGHTGRAY;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            button->button_color = DARKGRAY;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            button->button_pressed = 1;
        }
    } else {
        button->button_color = WHITE;
    }
}

void DrawTextButton(TextButton* button)
{
    DrawRectangleRec(button->rec, button->button_color);

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

void DrawImageButton(ImageButton* button, int scale)
{
    Rectangle dest = (Rectangle){ button->rec.x, button->rec.y, button->rec.width * scale, button->rec.height * scale };
    DrawTexturePro(
        button->image,
        (Rectangle){0,0, button->rec.width, button->rec.height},
        dest,
        (Vector2){0,0},
        0.0f,
        button->button_color
        );
}