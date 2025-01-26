#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <raylib.h>

typedef struct NumberBox{
    Rectangle rec;
    char number[4];
    int number_letter_count;
    int number_max_len;
    char label[50];
    int label_letter_count;
    int label_max_len;
    int font_size;
    int value;
    int mouse_on_text;
    int frame_counter;
} NumberBox;

NumberBox CreateNumberBox(int screen_width, int screen_height, int box_width, int box_height, int x_offset, int y_offset, int font_size, int default_value, char* label);

void UpdateNumberBox(NumberBox* num_box);

void DrawNumberBox(NumberBox* num_box);

#endif