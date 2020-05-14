#ifndef SCREEN_TEMPLATE_H
#define SCREEN_TEMPLATE_H

#include "LCD_Button.h"
#include "LCD.h"

typedef enum{
    BUTTON,
    TEXT,
    IMAGE
} screen_element_type_t;

typedef struct text_element{
    char* text_ptr;
    uint8_t size;
    uint16_t color;
    uint16_t x_coordinate;
    uint16_t y_coordinate;
} text_element_t;

typedef union element_ptr{
    button_t* button_ptr;
    text_element_t* text_element_ptr;
} element_ptr_t;

typedef struct screen_element{
    screen_element_type_t type;
    element_ptr_t element_ptr;
} screen_element_t;

void drawAllElements(screen_element_t* screenElementList, uint8_t numberOfElements);
void drawSingleElement(screen_element_t* screenElementPtr);
void eraseSingleElement(screen_element_t* screenElementPtr, uint16_t backgroundColor);
void updateSingleElement(screen_element_t* screenElementPtr, element_ptr_t newElementPtr, uint16_t backgroundColor);

#endif
