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
    Color default_color;
    Color hover_color;
    Color active_color;
} ImageButton;

typedef struct ToggleButton {
    // Button
    Rectangle rec;
    Color button_color;
    int x_offset;
    int y_offset;

    // Label
    char label[50];                 // Label string
    int label_letter_count;         // Current length of label string
    int label_max_len;              // Max length of label string
    int font_size;                  // Font size
    
    // Flags
    int button_pressed;             // If the button has been pressed
    int active;                     // If the button is toggled on/off
} ToggleButton;

TextButton CreateTextButton(int screen_width, int screen_height, char text[32], int font_size, int padding);
ImageButton CreateImageButton(int screen_width, int screen_height, int button_width, int button_height, Texture2D image, int dark_mode);
ToggleButton CreateToggleButton(int screen_width, int screen_height, int button_width, int button_height, int x_offset, int y_offset, int font_size, char* label, int previous_active_state);

void HandleTextButtonPress(TextButton* btn);
void HandleImageButtonPress(ImageButton* btn, int scale, Camera2D camera);
void HandleToggleButtonPress(ToggleButton* btn);

void DrawTextButton(TextButton* txt_btn, int screen_width, int screen_height);
void DrawImageButton(ImageButton*, int scale, float zoom);
void DrawToggleButton(ToggleButton* btn, int screen_width, int screen_height);

#endif