#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

typedef struct TextButton {
    Rectangle rec;
    char text[32];
    int font_size;
    int text_width;
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

TextButton CreateTextButton(int screen_width, int screen_height, char text[32], int font_size);
ImageButton CreateImageButton(int screen_width, int screen_height, int button_width, int button_height, Texture2D image);

void HandleTextButtonPress(TextButton*);
void HandleImageButtonPress(ImageButton*, int scale);

void DrawTextButton(TextButton*);
void DrawImageButton(ImageButton*, int scale);

#endif