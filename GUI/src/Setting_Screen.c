#include "Setting_Screen.h"

static screen_element_t setting_screen_element_list[NUMBER_OF_SETTING_SCREEN_ELEMENTS];
static button_t nav_bar_buttons[3];
static button_t other_buttons[4];
static text_element_t clickable_texts[5];
static char clickable_texts_str[5][4];
static uint8_t clickable_text_number[5];
static SETTING_SCREEN_ELEMENT_INDEX_t now_setting=TIME_SECOND_TEXT;

static void generateNavBarButtons(){
    setting_screen_element_list[SAVE_BUTTON].type = BUTTON;
    setting_screen_element_list[SAVE_BUTTON].element_ptr.button_ptr = &nav_bar_buttons[SAVE_BUTTON];
    button_t* save_button_ptr = setting_screen_element_list[SAVE_BUTTON].element_ptr.button_ptr;
    save_button_ptr->color = SAVE_BUTTON_COLOR;
    strcpy(save_button_ptr->text, "Save");
    adjustButtonSizeBasedOnText(save_button_ptr, NAV_BAR_BUTTON_PADDING);
    save_button_ptr->y_coordinate = SCREEN_MARGIN;
    save_button_ptr->x_coordinate = MAX_SCREEN_X - SCREEN_MARGIN - save_button_ptr->width;

    setting_screen_element_list[DISCARD_BUTTON].type = BUTTON;
    setting_screen_element_list[DISCARD_BUTTON].element_ptr.button_ptr = &nav_bar_buttons[DISCARD_BUTTON];
    button_t* discard_button_ptr = setting_screen_element_list[DISCARD_BUTTON].element_ptr.button_ptr;
    discard_button_ptr->color = DISCARD_BUTTON_COLOR;
    strcpy(discard_button_ptr->text, "Discard");
    adjustButtonSizeBasedOnText(discard_button_ptr, NAV_BAR_BUTTON_PADDING);
    discard_button_ptr->y_coordinate = SCREEN_MARGIN;
    discard_button_ptr->x_coordinate = save_button_ptr->x_coordinate - SCREEN_MARGIN - discard_button_ptr->width;

    setting_screen_element_list[BACK_TO_MAIN_BUTTON].type = BUTTON;
    setting_screen_element_list[BACK_TO_MAIN_BUTTON].element_ptr.button_ptr = &nav_bar_buttons[BACK_TO_MAIN_BUTTON];
    button_t* back_to_main_button_ptr = setting_screen_element_list[BACK_TO_MAIN_BUTTON].element_ptr.button_ptr;
    back_to_main_button_ptr->color = BACK_TO_MAIN_BUTTON_COLOR;
    strcpy(back_to_main_button_ptr->text, "< Main");
    adjustButtonSizeBasedOnText(back_to_main_button_ptr, NAV_BAR_BUTTON_PADDING);
    back_to_main_button_ptr->y_coordinate = SCREEN_MARGIN;
    back_to_main_button_ptr->x_coordinate = MIN_SCREEN_X + SCREEN_MARGIN;
}

void genetateTime(){
    LCD_Text_size(SETTING_TIME_TEXT_X, SETTING_TIME_TEXT_Y, "Time  ", LCD_BLACK, SETTING_TIME_TEXT_SIZE);

    setting_screen_element_list[TIME_HOUR_TEXT].type = TEXT;
    setting_screen_element_list[TIME_HOUR_TEXT].element_ptr.text_element_ptr = &clickable_texts[TIME_HOUR_TEXT-TIME_HOUR_TEXT];
    text_element_t* time_hour_text_ptr = setting_screen_element_list[TIME_HOUR_TEXT].element_ptr.text_element_ptr;
    time_hour_text_ptr->color = NORMAL_TEXT;
    time_hour_text_ptr->size = SETTING_TIME_TEXT_SIZE;
    time_hour_text_ptr->text_ptr = clickable_texts_str[TIME_HOUR_TEXT-TIME_HOUR_TEXT];
    time_hour_text_ptr->y_coordinate = SETTING_TIME_TEXT_Y;
    time_hour_text_ptr->x_coordinate = HOUR_TEXT_X;

    LCD_Text_size(HOUR_TEXT_X + CHAR_WIDTH * SETTING_TIME_TEXT_SIZE * 2, SETTING_TIME_TEXT_Y, ":", LCD_BLACK, SETTING_TIME_TEXT_SIZE);

    setting_screen_element_list[TIME_MINUTE_TEXT].type = TEXT;
    setting_screen_element_list[TIME_MINUTE_TEXT].element_ptr.text_element_ptr = &clickable_texts[TIME_MINUTE_TEXT-TIME_HOUR_TEXT];
    text_element_t* time_minute_text_ptr = setting_screen_element_list[TIME_MINUTE_TEXT].element_ptr.text_element_ptr;
    time_minute_text_ptr->color = NORMAL_TEXT;
    time_minute_text_ptr->size = SETTING_TIME_TEXT_SIZE;
    time_minute_text_ptr->text_ptr = clickable_texts_str[TIME_MINUTE_TEXT-TIME_HOUR_TEXT];
    time_minute_text_ptr->y_coordinate = SETTING_TIME_TEXT_Y;
    time_minute_text_ptr->x_coordinate = MINUTE_TEXT_X;

    LCD_Text_size(MINUTE_TEXT_X + CHAR_WIDTH * SETTING_TIME_TEXT_SIZE * 2, SETTING_TIME_TEXT_Y, ":", LCD_BLACK, SETTING_TIME_TEXT_SIZE);

    setting_screen_element_list[TIME_SECOND_TEXT].type = TEXT;
    setting_screen_element_list[TIME_SECOND_TEXT].element_ptr.text_element_ptr = &clickable_texts[TIME_SECOND_TEXT-TIME_HOUR_TEXT];
    text_element_t* time_second_text_ptr = setting_screen_element_list[TIME_SECOND_TEXT].element_ptr.text_element_ptr;
    time_second_text_ptr->color = NORMAL_TEXT;
    time_second_text_ptr->size = SETTING_TIME_TEXT_SIZE;
    time_second_text_ptr->text_ptr = clickable_texts_str[TIME_SECOND_TEXT-TIME_HOUR_TEXT];
    time_second_text_ptr->y_coordinate = SETTING_TIME_TEXT_Y;
    time_second_text_ptr->x_coordinate = SECOND_TEXT_X;
}

void generateBrightness(){
    LCD_Text_size(SETTING_BRIGHTNESS_TEXT_X, SETTING_BRIGHTNESS_TEXT_Y, "Brightness  ", LCD_BLACK, SETTING_BRIGHTNESS_TEXT_SIZE);

    setting_screen_element_list[BRIGHTNESS_INDEX_TEXT].type = TEXT;
    setting_screen_element_list[BRIGHTNESS_INDEX_TEXT].element_ptr.text_element_ptr = &clickable_texts[BRIGHTNESS_INDEX_TEXT-TIME_HOUR_TEXT];
    text_element_t* brightness_index_text_ptr = setting_screen_element_list[BRIGHTNESS_INDEX_TEXT].element_ptr.text_element_ptr;
    brightness_index_text_ptr->color = NORMAL_TEXT;
    brightness_index_text_ptr->size = SETTING_BRIGHTNESS_TEXT_SIZE;
    brightness_index_text_ptr->text_ptr = clickable_texts_str[BRIGHTNESS_INDEX_TEXT-TIME_HOUR_TEXT];
    brightness_index_text_ptr->y_coordinate = SETTING_BRIGHTNESS_TEXT_Y;
    brightness_index_text_ptr->x_coordinate = BEIGHTNESS_TEXT_X;
}

void generateScreenTimeout(){
    LCD_Text_size(SETTING_TIMEOUT_TEXT_X, SETTING_TIMEOUT_TEXT_Y_1, "LCD Screen", LCD_BLACK, SETTING_TIMEOUT_TEXT_SIZE);
    LCD_Text_size(SETTING_TIMEOUT_TEXT_X, SETTING_TIMEOUT_TEXT_Y_2, "Timeout", LCD_BLACK, SETTING_TIMEOUT_TEXT_SIZE);

    setting_screen_element_list[SCREEN_TIMEOUT_INDEX_TEXT].type = TEXT;
    setting_screen_element_list[SCREEN_TIMEOUT_INDEX_TEXT].element_ptr.text_element_ptr = &clickable_texts[SCREEN_TIMEOUT_INDEX_TEXT-TIME_HOUR_TEXT];
    text_element_t* screen_timeout_index_text_ptr = setting_screen_element_list[SCREEN_TIMEOUT_INDEX_TEXT].element_ptr.text_element_ptr;
    screen_timeout_index_text_ptr->color = NORMAL_TEXT;
    screen_timeout_index_text_ptr->size = TIMEOUT_INDEX_TEXT_SIZE;
    screen_timeout_index_text_ptr->text_ptr = clickable_texts_str[SCREEN_TIMEOUT_INDEX_TEXT-TIME_HOUR_TEXT];
    screen_timeout_index_text_ptr->y_coordinate = SETTING_TIMEOUT_TEXT_Y_1;
    screen_timeout_index_text_ptr->x_coordinate = TIMEOUT_INDEX_TEXT_X;
}

void generateAutoButtons(){

}

void generateUpDownButtons(){

}

void drawSettingScreen(){
    LCD_Clear(LCD_WHITE);
    generateNavBarButtons();
    genetateTime();
    drawNavBarLine(LCD_BLACK);
    generateBrightness();
    generateScreenTimeout();
    generateAutoButtons();
    generateUpDownButtons();
    drawAllElements(setting_screen_element_list, NUMBER_OF_SETTING_SCREEN_ELEMENTS);
}

void updateTimeHour(uint8_t hour){
    eraseSingleElement(&setting_screen_element_list[TIME_HOUR_TEXT], SETTING_SCREEN_BACKGROUND_COLOR);
    clickable_text_number[TIME_HOUR_TEXT - TIME_HOUR_TEXT] = hour;
    sprintf(clickable_texts_str[TIME_HOUR_TEXT - TIME_HOUR_TEXT], "%02d", clickable_text_number[TIME_HOUR_TEXT - TIME_HOUR_TEXT]);
    drawSingleElement(&setting_screen_element_list[TIME_HOUR_TEXT]);
}

void updateTimeMinute(uint8_t minute){
    eraseSingleElement(&setting_screen_element_list[TIME_MINUTE_TEXT], SETTING_SCREEN_BACKGROUND_COLOR);
    clickable_text_number[TIME_MINUTE_TEXT - TIME_HOUR_TEXT] = minute;
    sprintf(clickable_texts_str[TIME_MINUTE_TEXT - TIME_HOUR_TEXT], "%02d", clickable_text_number[TIME_MINUTE_TEXT - TIME_HOUR_TEXT]);
    drawSingleElement(&setting_screen_element_list[TIME_MINUTE_TEXT]);
}

void updateTimeSecond(uint8_t second){
    eraseSingleElement(&setting_screen_element_list[TIME_SECOND_TEXT], SETTING_SCREEN_BACKGROUND_COLOR);
    clickable_text_number[TIME_SECOND_TEXT - TIME_HOUR_TEXT] = second;
    sprintf(clickable_texts_str[TIME_SECOND_TEXT - TIME_HOUR_TEXT], "%02d", clickable_text_number[TIME_SECOND_TEXT - TIME_HOUR_TEXT]);
    drawSingleElement(&setting_screen_element_list[TIME_SECOND_TEXT]);
}

void updateBrightness(uint8_t brightness){
    eraseSingleElement(&setting_screen_element_list[BRIGHTNESS_INDEX_TEXT], SETTING_SCREEN_BACKGROUND_COLOR);
    clickable_text_number[BRIGHTNESS_INDEX_TEXT - TIME_HOUR_TEXT] = brightness;
    sprintf(clickable_texts_str[BRIGHTNESS_INDEX_TEXT - TIME_HOUR_TEXT], "%01d", clickable_text_number[BRIGHTNESS_INDEX_TEXT - TIME_HOUR_TEXT]);
    drawSingleElement(&setting_screen_element_list[BRIGHTNESS_INDEX_TEXT]);
}

void updateTimeout(uint8_t timeout){
    eraseSingleElement(&setting_screen_element_list[SCREEN_TIMEOUT_INDEX_TEXT], SETTING_SCREEN_BACKGROUND_COLOR);
    clickable_text_number[SCREEN_TIMEOUT_INDEX_TEXT - TIME_HOUR_TEXT] = timeout;
    if(timeout == 0){
        strcpy(clickable_texts_str[SCREEN_TIMEOUT_INDEX_TEXT - TIME_HOUR_TEXT], "OFF");
    }
    else{
        sprintf(clickable_texts_str[SCREEN_TIMEOUT_INDEX_TEXT - TIME_HOUR_TEXT], "%3d", clickable_text_number[BRIGHTNESS_INDEX_TEXT - TIME_HOUR_TEXT]);
    }
    drawSingleElement(&setting_screen_element_list[SCREEN_TIMEOUT_INDEX_TEXT]);
}

static SETTING_SCREEN_ELEMENT_INDEX_t detectPressedElement(uint16_t x, uint16_t y);
void settingScreenPressed();
