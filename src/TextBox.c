#include "TextBox.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


NumberBox CreateNumberBox(int screen_width, int screen_height, int box_width, int box_height, int x_offset, int y_offset, int font_size, int default_value, char* label){
    NumberBox box;

    box.rec = (Rectangle){
        (float)(screen_width-MeasureText(label, font_size))/2 + x_offset, 
        (float)(screen_height-box_height)/2 + y_offset, 
        MeasureText(label, font_size), 
        box_height
        };

    // Offset
    box.x_offset = x_offset;
    box.y_offset = y_offset;
    
    // Misc setup
    box.font_size = font_size;
    box.value = default_value;

    // Mouse event variables
    box.frame_counter = 0;
    box.mouse_on_text = 0;
    box.focused = 0;
    box.clicked = 0;
    
    // Number text setup
    sprintf(box.number, "%d", default_value);
    box.number_max_len = 3;
    box.number_letter_count = strlen(box.number);

    // Label text setup
    strcpy(box.label, label);
    box.label_max_len = 49;
    box.label_letter_count = strlen(box.label);
    
    return box;
}

void DrawNumberBox(NumberBox* num_box, int screen_width, int screen_height){
    // Update box position
    num_box->rec.x = (float)(screen_width-MeasureText(num_box->label, num_box->font_size))/2 + num_box->x_offset; 
    num_box->rec.y = (float)(screen_height-num_box->rec.height)/2 + num_box->y_offset;
    
    // Draw box
    DrawRectangleRec(num_box->rec, LIGHTGRAY);

    // Draw mouse hover effect
    if (num_box->mouse_on_text) DrawRectangleLines((int)num_box->rec.x, (int)num_box->rec.y, (int)num_box->rec.width, (int)num_box->rec.height, BLACK);
    else DrawRectangleLines((int)num_box->rec.x, (int)num_box->rec.y, (int)num_box->rec.width, (int)num_box->rec.height, DARKGRAY);
    
    // Draw text
    DrawText(num_box->number, (int)num_box->rec.x + 5, (int)num_box->rec.y + 8, num_box->font_size, DARKGRAY);
    DrawText(num_box->label, (int)num_box->rec.x, (int)num_box->rec.y - num_box->font_size - 5, num_box->font_size, BLACK);
    
    // Draw blinking underscore char
    if (num_box->focused)
    {
        if (((num_box->frame_counter/20)%2) == 0) DrawText("_", (int)num_box->rec.x + 8 + MeasureText(num_box->number, num_box->font_size), (int)num_box->rec.y + 10, num_box->font_size, DARKGRAY);
        num_box->frame_counter++;
    }
}

void CheckMouseHover(NumberBox* num_box){
    // Check cursor collition
    if (CheckCollisionPointRec(GetMousePosition(), num_box->rec)) {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            num_box->clicked = 1;
        }
        num_box->mouse_on_text = 1;
    }
    else num_box->mouse_on_text = 0;
}

void UpdateNumberBox(NumberBox* num_box){
    // Textbox focused effect
    if (num_box->focused)
    {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow numbers in range [48..57] (ASCII for 0-9)
            if ((key >= 48) && (key <= 57) && (num_box->number_letter_count < num_box->number_max_len))
            {
                // Update number string
                num_box->number[num_box->number_letter_count] = (char)key;
                num_box->number_letter_count++;
                num_box->number[num_box->number_letter_count] = '\0';

                // Update value
                num_box->value = atoi(num_box->number);
                //printf("Current value: %d\n", num_box->value);
            }
            key = GetCharPressed();  // Check next character in the queue
        }
        if (IsKeyPressed(KEY_BACKSPACE))
        {   
            // Update number string
            num_box->number_letter_count--;
            if (num_box->number_letter_count < 0) num_box->number_letter_count = 0;
            num_box->number[num_box->number_letter_count] = '\0';

            // Update value
            num_box->value = atoi(num_box->number);
            //printf("Current value: %d\n", num_box->value);
        }
    }
}