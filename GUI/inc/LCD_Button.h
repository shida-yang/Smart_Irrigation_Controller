#ifndef LCD_BUTTON_H
#define LCD_BUTTON_H

#include "LCD.h"
#include "string.h"

#define BUTTON_TOUCHED      1
#define BUTTON_NOT_TOUCHED  0

#define CHAR_WIDTH          8
#define CHAR_HEIGHT         16
#define CHAR_WIDTH_D2       (CHAR_WIDTH>>1)
#define CHAR_HEIGHT_D2      (CHAR_HEIGHT>>1)

#define BUTTON_TEXT_MAX_LEN 32

typedef struct button{
    uint16_t x_coordinate;
    uint16_t y_coordinate;
    uint16_t width;
    uint16_t height;
    uint16_t color;
    char text[16];
} button_t;

void drawButton(button_t* button);
bool buttonTouched(button_t* button, uint16_t touch_x, uint16_t touch_y);

#endif
