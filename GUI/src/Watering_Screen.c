#include "Watering_Screen.h"

static screen_element_t watering_screen_element_list[NUMBER_OF_WATERING_SCREEN_ELEMENTS];
static text_element_t zonex_lines[7];
static char zonex_line_str[7][22];
static uint8_t target_minutes[7];
static uint8_t finished_minutes[7];
static button_t watering_back_to_main_button, curr_zone_indicator_button, switch_zone_button;
static WATERING_SCREEN_ELEMENT_INDEX_t now_setting, curr_zone;

static void generateNavBarButtons(){
    watering_screen_element_list[WATERING_BACK_TO_MAIN_BUTTON].type = BUTTON;
    watering_screen_element_list[WATERING_BACK_TO_MAIN_BUTTON].element_ptr.button_ptr = &watering_back_to_main_button;
    button_t* back_to_main_button_ptr = watering_screen_element_list[WATERING_BACK_TO_MAIN_BUTTON].element_ptr.button_ptr;
    back_to_main_button_ptr->color = WATERING_BACK_TO_MAIN_BUTTON_COLOR;
    strcpy(back_to_main_button_ptr->text, "< Main");
    adjustButtonSizeBasedOnText(back_to_main_button_ptr, NAV_BAR_BUTTON_PADDING);
    back_to_main_button_ptr->y_coordinate = SCREEN_MARGIN;
    back_to_main_button_ptr->x_coordinate = MIN_SCREEN_X + SCREEN_MARGIN;
}

static void generateZonexLines(){
    uint16_t curr_y = ZONEX_LINES_INIT_Y;
    for(int i = WATERING_ZONE1_LINE; i <= WATERING_ZONE7_LINE; i++){
        watering_screen_element_list[i].type = TEXT;
        watering_screen_element_list[i].element_ptr.text_element_ptr = &zonex_lines[i - WATERING_ZONE1_LINE];

        text_element_t* curr_te_ptr = &zonex_lines[i - WATERING_ZONE1_LINE];
        curr_te_ptr->size = ZONEX_TEXT_SIZE;
        curr_te_ptr->color = LCD_BLACK;
        curr_te_ptr->text_ptr = &zonex_line_str[i - WATERING_ZONE1_LINE];
        sprintf(&zonex_line_str[i - WATERING_ZONE1_LINE], " ");
        curr_te_ptr->x_coordinate = ZONEX_X;
        curr_te_ptr->y_coordinate = curr_y;

        curr_y += (CHAR_HEIGHT * ZONEX_TEXT_SIZE + ZONEX_LINES_SPACE);
    }
}

static void generateCurrZoneIndicator(){
    watering_screen_element_list[CURR_ZONE_INDICATOR].type = BUTTON;
    watering_screen_element_list[CURR_ZONE_INDICATOR].element_ptr.button_ptr = &curr_zone_indicator_button;
    button_t* curr_zone_indicator_ptr = watering_screen_element_list[CURR_ZONE_INDICATOR].element_ptr.button_ptr;
    curr_zone_indicator_ptr->color = CURR_ZONE_BUTTON_COLOR;
    strcpy(curr_zone_indicator_ptr->text, "Current Zone ->");
    adjustButtonSizeBasedOnText(curr_zone_indicator_ptr, NAV_BAR_BUTTON_PADDING);
    curr_zone_indicator_ptr->y_coordinate = ZONEX_LINES_INIT_Y - 3;
    curr_zone_indicator_ptr->x_coordinate = CURR_ZONE_X;
}

static void generateSwitchZoneButton(){
    watering_screen_element_list[SWITCH_ZONE_BUTTON].type = BUTTON;
    watering_screen_element_list[SWITCH_ZONE_BUTTON].element_ptr.button_ptr = &switch_zone_button;
    button_t* switch_zone_button_ptr = watering_screen_element_list[SWITCH_ZONE_BUTTON].element_ptr.button_ptr;
    switch_zone_button_ptr->color = SWITCH_ZONE_BUTTON_COLOR;
    strcpy(switch_zone_button_ptr->text, "Switch Zone");
    adjustButtonSizeBasedOnText(switch_zone_button_ptr, NAV_BAR_BUTTON_PADDING);
    switch_zone_button_ptr->y_coordinate = SWITCH_ZONE_BUTTON_Y;
    switch_zone_button_ptr->x_coordinate = SWITCH_ZONE_BUTTON_X;
}

void drawWateringScreen(){
    now_setting = -1;

    LCD_Clear(WATERING_SCREEN_BACKGROUND_COLOR);
    generateNavBarButtons();
    generateCurrZoneIndicator();
    generateZonexLines();
    generateSwitchZoneButton();
    drawNavBarLine(LCD_BLACK);
    drawAllElements(watering_screen_element_list, NUMBER_OF_WATERING_SCREEN_ELEMENTS);

    // TODO - update values based on current values (GET FROM OUTSIDE WORLD)
    // TODO - Add logic for active/inactive zones
    for(int i = WATERING_ZONE1_LINE; i <= WATERING_ZONE7_LINE; i++){
        target_minutes[i - WATERING_ZONE1_LINE] = (i+1)*9;
        finished_minutes[i - WATERING_ZONE1_LINE] = i*9;
    }
    curr_zone = WATERING_ZONE3_LINE;

    for(int i = WATERING_ZONE1_LINE; i <= WATERING_ZONE7_LINE; i++){
        sprintf(&zonex_line_str[i - WATERING_ZONE1_LINE], ZONEX_LINES_TEMPLATE, i - WATERING_ZONE1_LINE + 1, finished_minutes[i - WATERING_ZONE1_LINE], target_minutes[i - WATERING_ZONE1_LINE]);
        drawSingleElement(&watering_screen_element_list[i]);
    }

    updateCurrZoneIndicator(curr_zone);
}

void updateZonexLineFinishedMinutes(WATERING_SCREEN_ELEMENT_INDEX_t element_index, uint8_t minutes){
    eraseSingleElement(&watering_screen_element_list[element_index], WATERING_SCREEN_BACKGROUND_COLOR);

    finished_minutes[element_index - WATERING_ZONE1_LINE] = minutes;
    sprintf(&zonex_line_str[element_index - WATERING_ZONE1_LINE], ZONEX_LINES_TEMPLATE, element_index - WATERING_ZONE1_LINE + 1, finished_minutes[element_index - WATERING_ZONE1_LINE], target_minutes[element_index - WATERING_ZONE1_LINE]);

    drawSingleElement(&watering_screen_element_list[element_index]);
}

void updateZonexLineTargetMinutes(WATERING_SCREEN_ELEMENT_INDEX_t element_index, uint8_t minutes){
    eraseSingleElement(&watering_screen_element_list[element_index], WATERING_SCREEN_BACKGROUND_COLOR);

    target_minutes[element_index - WATERING_ZONE1_LINE] = minutes;
    sprintf(&zonex_line_str[element_index - WATERING_ZONE1_LINE], ZONEX_LINES_TEMPLATE, element_index - WATERING_ZONE1_LINE + 1, finished_minutes[element_index - WATERING_ZONE1_LINE], target_minutes[element_index - WATERING_ZONE1_LINE]);

    drawSingleElement(&watering_screen_element_list[element_index]);
}

void updateCurrZoneIndicator(WATERING_SCREEN_ELEMENT_INDEX_t element_index){
    eraseSingleElement(&watering_screen_element_list[CURR_ZONE_INDICATOR], WATERING_SCREEN_BACKGROUND_COLOR);

    curr_zone_indicator_button.y_coordinate = watering_screen_element_list[element_index].element_ptr.text_element_ptr->y_coordinate - 3;

    drawSingleElement(&watering_screen_element_list[CURR_ZONE_INDICATOR]);
}

static WATERING_SCREEN_ELEMENT_INDEX_t detectPressedElement_watering(uint16_t x, uint16_t y){
    for(int i = 0; i < NUMBER_OF_WATERING_SCREEN_ELEMENTS; i++){
        // Button
        if(watering_screen_element_list[i].type == BUTTON){
            button_t* button_ptr = watering_screen_element_list[i].element_ptr.button_ptr;
            uint16_t min_x = button_ptr->x_coordinate;
            uint16_t max_x = min_x + button_ptr->width;
            uint16_t min_y = button_ptr->y_coordinate;
            uint16_t max_y = min_y + button_ptr->height;
            if(x >= min_x && x <= max_x && y >= min_y && y <= max_y){
                return i;
            }
        }
        // Text
        else{
            text_element_t* text_element_ptr = watering_screen_element_list[i].element_ptr.text_element_ptr;
            uint16_t min_x = text_element_ptr->x_coordinate;
            uint16_t max_x = min_x + strlen(text_element_ptr->text_ptr) * text_element_ptr->size * CHAR_WIDTH;
            uint16_t min_y = text_element_ptr->y_coordinate;
            uint16_t max_y = min_y + text_element_ptr->size * CHAR_HEIGHT;
            if(x >= min_x && x <= max_x && y >= min_y && y <= max_y){
                return i;
            }
        }
    }
    return -1;
}

void wateringScreenPressed(uint16_t x, uint16_t y){
    WATERING_SCREEN_ELEMENT_INDEX_t element_index = detectPressedElement_watering(x, y);

    switch(element_index){
    case WATERING_BACK_TO_MAIN_BUTTON:
        break;

    case WATERING_ZONE1_LINE:
    case WATERING_ZONE2_LINE:
    case WATERING_ZONE3_LINE:
    case WATERING_ZONE4_LINE:
    case WATERING_ZONE5_LINE:
    case WATERING_ZONE6_LINE:
    case WATERING_ZONE7_LINE:
        // TODO - Add logic for active/inactive zones

        if((int8_t)now_setting >= 0){
            // Deactivate old element
            watering_screen_element_list[now_setting].element_ptr.text_element_ptr->color = NORMAL_TEXT;
            updateZonexLineFinishedMinutes(now_setting, finished_minutes[now_setting - WATERING_ZONE1_LINE]);
        }
        // Activate new element
        now_setting = element_index;
        watering_screen_element_list[now_setting].element_ptr.text_element_ptr->color = SELECTED_TEXT;
        updateZonexLineFinishedMinutes(now_setting, finished_minutes[now_setting - WATERING_ZONE1_LINE]);
        break;

    case SWITCH_ZONE_BUTTON:
        // TODO - Add logic for active/inactive zones
        if((int8_t)now_setting >= 0 && curr_zone != now_setting){
            curr_zone = now_setting;
            updateCurrZoneIndicator(curr_zone);
        }
        break;

    default:
        break;
    }
}

