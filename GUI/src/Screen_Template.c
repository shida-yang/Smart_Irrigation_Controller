#include "Screen_Template.h"

void drawAllElements(screen_element_t* screenElementList, uint8_t numberOfElements){
    for(int i = 0; i < numberOfElements; i++){
        screen_element_t* currElement=&screenElementList[i];
        drawSingleElement(currElement);
    }
}

void drawSingleElement(screen_element_t* screenElementPtr){
    if(screenElementPtr->type == BUTTON){
        drawButton(screenElementPtr->element_ptr.button_ptr);
    }
    else if(screenElementPtr->type == TEXT){
        text_element_t* te = screenElementPtr->element_ptr.text_element_ptr;
        LCD_Text_size(te->x_coordinate, te->y_coordinate, te->text_ptr, te->color, te->size);
    }
    else if(screenElementPtr->type == IMAGE){

    }
}

void eraseSingleElement(screen_element_t* screenElementPtr, uint16_t backgroundColor){
    if(screenElementPtr->type == BUTTON){
        button_t* button = screenElementPtr->element_ptr.button_ptr;
        LCD_DrawRectangle(button->x_coordinate, button->x_coordinate + button->width,
                          button->y_coordinate, button->y_coordinate + button->height,
                          backgroundColor
        );
    }
    else if(screenElementPtr->type == TEXT){
        text_element_t* te = screenElementPtr->element_ptr.text_element_ptr;
        LCD_Text_size(te->x_coordinate, te->y_coordinate, te->text_ptr, backgroundColor, te->size);
    }
    else if(screenElementPtr->type == IMAGE){

    }
}

void updateSingleElement(screen_element_t* screenElementPtr, element_ptr_t newElementPtr, uint16_t backgroundColor){
    eraseSingleElement(screenElementPtr, backgroundColor);
    if(screenElementPtr->type == BUTTON){
        *(screenElementPtr->element_ptr.button_ptr) = *(newElementPtr.button_ptr);
    }
    else if(screenElementPtr->type == TEXT){
        *(screenElementPtr->element_ptr.text_element_ptr) = *(newElementPtr.text_element_ptr);
    }
    else if(screenElementPtr->type == IMAGE){

    }
    drawSingleElement(screenElementPtr);
}
