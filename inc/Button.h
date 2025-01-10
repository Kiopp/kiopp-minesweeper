#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

typedef struct Button {
    Rectangle rec;
    char text[32];
    int font_size;
    int text_width;
    Color button_color;
    int button_pressed;
} Button;

Button CreateButton(int screenWidth, int screenHeight, char text[32], int font_size);

void HandleButtonPress(Button*);

void DrawButton(Button*);

#endif