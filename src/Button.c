#include "Button.h"
#include "raylib.h"
#include <string.h>

TextButton CreateTextButton(int screenWidth, int screenHeight, char text[32], int font_size, int padding) {
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

ImageButton CreateImageButton(int screen_width, int screen_height, int button_width, int button_height, Texture2D image, int dark_mode) {
    // Declare new Button
    ImageButton button;

    // Initialize Button values
    button.image = image;
    button.button_color = WHITE; // No image hue change
    button.button_pressed = 0;
    button.toggle_flag = 0;
    if (dark_mode) {
        button.default_color = LIGHTGRAY;
        button.hover_color = (Color){ 150, 150, 150, 255 };
        button.active_color = (Color){ 60, 60, 60, 255 };
    } else {
        button.default_color = WHITE;
        button.hover_color = LIGHTGRAY;
        button.active_color = DARKGRAY;
    }
    

    button.rec = (Rectangle){
        0, // Default position
        0, // Default position
        button_width, 
        button_height
        };

    return button;
}

ToggleButton CreateToggleButton(int screen_width, int screen_height, int button_width, int button_height, int x_offset, int y_offset, int font_size, char* label, int previous_active_state){
    // Declare new Button
    ToggleButton btn;

    // Init Rectangle
    btn.rec = (Rectangle){
        (float)(screen_width-MeasureText(label, font_size))/2 + x_offset, 
        (float)(screen_height-button_height)/2 + y_offset, 
        button_width, 
        button_height
        };

    // Init Button values
    btn.button_color = GRAY;
    btn.button_pressed = 0;
    btn.active = previous_active_state;
    btn.x_offset = x_offset;
    btn.y_offset = y_offset;

    // Label text setup
    strcpy(btn.label, label);
    btn.label_max_len = 49;
    btn.label_letter_count = strlen(btn.label);
    btn.font_size = font_size;

    return btn;
}

void HandleTextButtonPress(TextButton* button){
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

void HandleImageButtonPress(ImageButton* button, int scale, Camera2D camera){
    Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera); // Transform mouse position
    Rectangle dest = (Rectangle){ 
        button->rec.x * camera.zoom, 
        button->rec.y * camera.zoom, 
        button->rec.width * scale * camera.zoom, 
        button->rec.height * scale * camera.zoom
    };

    if (CheckCollisionPointRec(mouse_pos, dest)) {
        button->button_color = button->hover_color;
        // Left mouse button click
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            button->button_color = button->active_color;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            button->button_pressed = 1;
        }

        // Right mouse button click
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            button->button_color = button->active_color;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            button->toggle_flag = 1;
        }
    } else {
        button->button_color = button->default_color;
    }
}

void HandleToggleButtonPress(ToggleButton* btn){
    if (CheckCollisionPointRec(GetMousePosition(), btn->rec)) {
        btn->button_color = LIGHTGRAY;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            btn->button_color = DARKGRAY;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            btn->button_pressed = 1;
        }
    } else {
        btn->button_color = GRAY;
    }
}

void DrawTextButton(TextButton* button, int screen_width, int screen_height){
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

void DrawImageButton(ImageButton* button, int scale, float zoom){
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

void DrawToggleButton(ToggleButton* btn, int screen_width, int screen_height){
    // Update button position
    btn->rec.x = (float)(screen_width - btn->rec.width)/2 + (float)btn->x_offset;
    btn->rec.y = (float)(screen_height - btn->rec.height)/2 + (float)btn->y_offset;

    // Draw button
    DrawRectangleRec(btn->rec, btn->button_color);

    Color btn_active_col = BLACK;
    if (btn->active) {
        btn_active_col = GREEN;
    } else {
        btn_active_col = RED;
    }
    DrawCircle(btn->rec.x + btn->rec.width/2, btn->rec.y + btn->rec.width/2, btn->rec.width/2 - 2, DARKGRAY);
    DrawCircle(btn->rec.x + btn->rec.width/2, btn->rec.y + btn->rec.width/2, btn->rec.width/2 - 3.5, btn_active_col);

    // Draw label
    DrawText(
        btn->label, 
        (int)btn->rec.x + 10 - MeasureText(btn->label, btn->font_size)/2, 
        (int)btn->rec.y - btn->font_size - 5,
        btn->font_size, 
        BLACK
    );
}