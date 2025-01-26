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
    
    box.font_size = font_size;
    box.value = default_value;
    box.frame_counter = 0;
    
    box.mouse_on_text = 0;

    sprintf(box.number, "%d", default_value);
    box.number_max_len = 3;
    box.number_letter_count = strlen(box.number);
    printf("number length: %d\n", box.number_letter_count);

    // Label text setup
    strcpy(box.label, label);
    box.label_max_len = 49;
    box.label_letter_count = strlen(box.label);
    

    return box;
}

void DrawNumberBox(NumberBox* num_box){
    // Draw box
    DrawRectangleRec(num_box->rec, LIGHTGRAY);

    // Draw mouse hover effect
    if (num_box->mouse_on_text) DrawRectangleLines((int)num_box->rec.x, (int)num_box->rec.y, (int)num_box->rec.width, (int)num_box->rec.height, BLACK);
    else DrawRectangleLines((int)num_box->rec.x, (int)num_box->rec.y, (int)num_box->rec.width, (int)num_box->rec.height, DARKGRAY);
    
    // Draw text
    DrawText(num_box->number, (int)num_box->rec.x + 5, (int)num_box->rec.y + 8, num_box->font_size, DARKGRAY);
    DrawText(num_box->label, (int)num_box->rec.x, (int)num_box->rec.y - num_box->font_size - 5, num_box->font_size, BLACK);
    
    // Draw blinking underscore char
    if (num_box->mouse_on_text)
    {
        if (((num_box->frame_counter/20)%2) == 0) DrawText("_", (int)num_box->rec.x + 8 + MeasureText(num_box->number, num_box->font_size), (int)num_box->rec.y + 5, 40, DARKGRAY);
    }
}

void UpdateNumberBox(NumberBox* num_box){
    // Check cursor collition
    if (CheckCollisionPointRec(GetMousePosition(), num_box->rec)) num_box->mouse_on_text = 1;
    else num_box->mouse_on_text = 0;

    // Mouse hover effect
    if (num_box->mouse_on_text)
    {
        // Set the window's cursor to the I-Beam
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

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
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}