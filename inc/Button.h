#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

typedef struct TextButton {
    Rectangle rec;
    char text[32];
    int font_size;
    int text_width;
    int text_padding;
    Color button_color;
    int button_pressed;
} TextButton;

typedef struct ImageButton {
    Texture2D image;
    Rectangle rec;
    Color button_color;
    int button_pressed;
    int toggle_flag;
} ImageButton;

TextButton CreateTextButton(int screen_width, int screen_height, char text[32], int font_size, int padding);
ImageButton CreateImageButton(int screen_width, int screen_height, int button_width, int button_height, Texture2D image);

void HandleTextButtonPress(TextButton*);
void HandleImageButtonPress(ImageButton*, int scale, Camera2D camera);

void DrawTextButton(TextButton* txt_btn, int screen_width, int screen_height);
void DrawImageButton(ImageButton*, int scale, float zoom);

#endif