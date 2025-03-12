#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <raylib.h>

typedef struct NumberBox{
    Rectangle rec;                  // Rectangle representing the box
    char number[4];                 // Number input string
    int number_letter_count;        // Current length of number string
    int number_max_len;             // Max length of number string
    char label[50];                 // Label string
    int label_letter_count;         // Current length of label string
    int label_max_len;              // Max length of label string
    int font_size;                  // Font size
    int value;                      // Integer value represented by the number string
    int mouse_on_text;              // Boolean value if the mouse is over the text
    int clicked;                    // Boolean value if the box has been clicked
    int focused;                    // Boolean value if the box is currently focused
    int frame_counter;              // Frame counter for blinking bar
    int x_offset;                   // X-offset for drawing
    int y_offset;                   // Y-offset for drawing
} NumberBox;

NumberBox CreateNumberBox(int screen_width, int screen_height, int box_width, int box_height, int x_offset, int y_offset, int font_size, int default_value, char* label);

void CheckMouseHover(NumberBox* num_box);

void UpdateNumberBox(NumberBox* num_box);

void DrawNumberBox(NumberBox* num_box, int screen_width, int screen_height);

#endif