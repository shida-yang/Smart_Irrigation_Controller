#include "LCD_Button.h"

void adjustButtonSizeBasedOnText(button_t* button, uint8_t padding_pixel_width){
    uint16_t text_width = strlen(button->text) * CHAR_WIDTH;
    button->width = text_width + 2 * padding_pixel_width + 2;
    button->height = CHAR_HEIGHT + 2 * padding_pixel_width + 2;
}

void drawButton(button_t* button){
    // Calculate text position
    uint16_t text_width = strlen(button->text) * CHAR_WIDTH;
    uint16_t button_center_pos_x = button->x_coordinate + (button->width >> 1);
    uint16_t button_center_pos_y = button->y_coordinate + (button->height >> 1);
    uint16_t text_x = button_center_pos_x - (text_width >> 1);
    uint16_t text_y = button_center_pos_y - (CHAR_HEIGHT >> 1);

    // Draw rectangle (with edge)
    LCD_DrawRectangle_edge(button->x_coordinate, button->x_coordinate + button->width,
                           button->y_coordinate, button->y_coordinate + button->height,
                           button->color, LCD_BLACK
    );

    // Write text
    LCD_Text(text_x, text_y, button->text, LCD_BLACK);
}

bool buttonTouched(button_t* button, uint16_t touch_x, uint16_t touch_y){
    uint16_t button_x_max = button->x_coordinate + button->width;
    uint16_t button_y_max = button->y_coordinate + button->height;
    if( touch_x >= button->x_coordinate &&
        touch_x <= button_x_max         &&
        touch_y >= button->y_coordinate &&
        touch_y <= button_y_max
    ){
        return true;
    }
    else{
        return false;
    }
}
