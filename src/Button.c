#include "../inc/Button.h"
#include "raylib.h"
#include <string.h>

Button CreateButton(int screenWidth, int screenHeight, char text[32], int font_size) 
{
    // Declare new Button
    Button button;

    // Initialize Button values
    strcpy(button.text, text);
    button.font_size = font_size;
    button.text_width = MeasureText(button.text, button.font_size);
    button.button_color = GRAY;
    button.button_pressed = 0;

    int padding = 20;

    button.rec = (Rectangle){
        (float)screenWidth/2 - (float)button.text_width/2, 
        (float)screenHeight/2 - (float)font_size/2, 
        button.text_width + padding, 
        font_size + padding
        };

    return button;
}

void HandleButtonPress(Button* button)
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

void DrawButton(Button* button)
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