#include "Setting_Screen.h"

static screen_element_t setting_screen_element_list[NUMBER_OF_SETTING_SCREEN_ELEMENTS];
static button_t nav_bar_buttons[3];
static button_t other_buttons[4];
static text_element_t clickable_texts[5];
static char clickable_texts_str[5][4];
static uint8_t clickable_text_number[5];
static bool auto_button_state[2];
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
    setting_screen_element_list[TIME_AUTO_BUTTON].type = BUTTON;
    setting_screen_element_list[TIME_AUTO_BUTTON].element_ptr.button_ptr = &other_buttons[TIME_AUTO_BUTTON - TIME_AUTO_BUTTON];
    button_t* time_auto_button_ptr = setting_screen_element_list[TIME_AUTO_BUTTON].element_ptr.button_ptr;
    time_auto_button_ptr->color = AUTO_BUTTON_DEACTIVATE_COLOR;
    strcpy(time_auto_button_ptr->text, "Auto");
    adjustButtonSizeBasedOnText(time_auto_button_ptr, NAV_BAR_BUTTON_PADDING);
    time_auto_button_ptr->y_coordinate = TIME_AUTO_BUTTON_Y;
    time_auto_button_ptr->x_coordinate = TIME_AUTO_BUTTON_X;

    setting_screen_element_list[BRIGHTNESS_AUTO_BUTTON].type = BUTTON;
    setting_screen_element_list[BRIGHTNESS_AUTO_BUTTON].element_ptr.button_ptr = &other_buttons[BRIGHTNESS_AUTO_BUTTON - TIME_AUTO_BUTTON];
    button_t* brightness_auto_button_ptr = setting_screen_element_list[BRIGHTNESS_AUTO_BUTTON].element_ptr.button_ptr;
    brightness_auto_button_ptr->color = AUTO_BUTTON_DEACTIVATE_COLOR;
    strcpy(brightness_auto_button_ptr->text, "Auto");
    adjustButtonSizeBasedOnText(brightness_auto_button_ptr, NAV_BAR_BUTTON_PADDING);
    brightness_auto_button_ptr->y_coordinate = BRIGHTNESS_AUTO_BUTTON_Y;
    brightness_auto_button_ptr->x_coordinate = BRIGHTNESS_AUTO_BUTTON_X;
}

void generateUpDownButtons(){
    setting_screen_element_list[DOWN_BUTTON].type = BUTTON;
    setting_screen_element_list[DOWN_BUTTON].element_ptr.button_ptr = &other_buttons[DOWN_BUTTON - TIME_AUTO_BUTTON];
    button_t* down_button_ptr = setting_screen_element_list[DOWN_BUTTON].element_ptr.button_ptr;
    down_button_ptr->color = UP_DOWN_BUTTON_COLOR;
    strcpy(down_button_ptr->text, "DOWN");
    adjustButtonSizeBasedOnText(down_button_ptr, UP_DOWN_BUTTON_PADDING);
    down_button_ptr->y_coordinate = DOWN_BUTTON_Y;
    down_button_ptr->x_coordinate = DOWN_BUTTON_X;

    setting_screen_element_list[UP_BUTTON].type = BUTTON;
    setting_screen_element_list[UP_BUTTON].element_ptr.button_ptr = &other_buttons[UP_BUTTON - TIME_AUTO_BUTTON];
    button_t* up_button_ptr = setting_screen_element_list[UP_BUTTON].element_ptr.button_ptr;
    up_button_ptr->color = UP_DOWN_BUTTON_COLOR;
    strcpy(up_button_ptr->text, " UP ");
    adjustButtonSizeBasedOnText(up_button_ptr, UP_DOWN_BUTTON_PADDING);
    up_button_ptr->y_coordinate = UP_BUTTON_Y;
    up_button_ptr->x_coordinate = UP_BUTTON_X;
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
    // TODO - update temp_xx based on current values (GET FROM OUTSIDE WORLD)
    clickable_text_number[TIME_HOUR_TEXT - TIME_HOUR_TEXT] = 22;
    clickable_text_number[TIME_MINUTE_TEXT - TIME_HOUR_TEXT] = 33;
    clickable_text_number[TIME_SECOND_TEXT - TIME_HOUR_TEXT] = 50;
    clickable_text_number[BRIGHTNESS_INDEX_TEXT - TIME_HOUR_TEXT] = 5;
    clickable_text_number[SCREEN_TIMEOUT_INDEX_TEXT - TIME_HOUR_TEXT] = 0;
    auto_button_state[TIME_AUTO_BUTTON - TIME_AUTO_BUTTON] = 0;
    auto_button_state[BRIGHTNESS_AUTO_BUTTON - TIME_AUTO_BUTTON] = 1;
    // Update screen based on current values
    for(int i = TIME_HOUR_TEXT; i <= SCREEN_TIMEOUT_INDEX_TEXT; i++){
        updateTextElement(i, clickable_text_number[i - TIME_HOUR_TEXT]);
    }
    updateAutoButton(TIME_AUTO_BUTTON, auto_button_state[TIME_AUTO_BUTTON - TIME_AUTO_BUTTON]);
    updateAutoButton(BRIGHTNESS_AUTO_BUTTON, auto_button_state[BRIGHTNESS_AUTO_BUTTON - TIME_AUTO_BUTTON]);
}

void updateTimeHour(uint8_t hour){
    if((int8_t)hour > 23){
        hour = 0;
    }
    if((int8_t)hour < 0){
        hour = 23;
    }
    eraseSingleElement(&setting_screen_element_list[TIME_HOUR_TEXT], SETTING_SCREEN_BACKGROUND_COLOR);
    clickable_text_number[TIME_HOUR_TEXT - TIME_HOUR_TEXT] = hour;

    sprintf(clickable_texts_str[TIME_HOUR_TEXT - TIME_HOUR_TEXT], "%02d", clickable_text_number[TIME_HOUR_TEXT - TIME_HOUR_TEXT]);
    drawSingleElement(&setting_screen_element_list[TIME_HOUR_TEXT]);
}

void updateTimeMinute(uint8_t minute){
    if((int8_t)minute > 59){
        minute = 0;
    }
    if((int8_t)minute < 0){
        minute = 59;
    }
    eraseSingleElement(&setting_screen_element_list[TIME_MINUTE_TEXT], SETTING_SCREEN_BACKGROUND_COLOR);
    clickable_text_number[TIME_MINUTE_TEXT - TIME_HOUR_TEXT] = minute;
    sprintf(clickable_texts_str[TIME_MINUTE_TEXT - TIME_HOUR_TEXT], "%02d", clickable_text_number[TIME_MINUTE_TEXT - TIME_HOUR_TEXT]);
    drawSingleElement(&setting_screen_element_list[TIME_MINUTE_TEXT]);
}

void updateTimeSecond(uint8_t second){
    if((int8_t)second > 59){
        second = 0;
    }
    if((int8_t)second < 0){
        second = 59;
    }
    eraseSingleElement(&setting_screen_element_list[TIME_SECOND_TEXT], SETTING_SCREEN_BACKGROUND_COLOR);
    clickable_text_number[TIME_SECOND_TEXT - TIME_HOUR_TEXT] = second;
    sprintf(clickable_texts_str[TIME_SECOND_TEXT - TIME_HOUR_TEXT], "%02d", clickable_text_number[TIME_SECOND_TEXT - TIME_HOUR_TEXT]);
    drawSingleElement(&setting_screen_element_list[TIME_SECOND_TEXT]);
}

void updateBrightness(uint8_t brightness){
    if((int8_t)brightness > 8){
        brightness = 1;
    }
    if((int8_t)brightness < 1){
        brightness = 8;
    }
    eraseSingleElement(&setting_screen_element_list[BRIGHTNESS_INDEX_TEXT], SETTING_SCREEN_BACKGROUND_COLOR);
    clickable_text_number[BRIGHTNESS_INDEX_TEXT - TIME_HOUR_TEXT] = brightness;
    sprintf(clickable_texts_str[BRIGHTNESS_INDEX_TEXT - TIME_HOUR_TEXT], "%01d", clickable_text_number[BRIGHTNESS_INDEX_TEXT - TIME_HOUR_TEXT]);
    drawSingleElement(&setting_screen_element_list[BRIGHTNESS_INDEX_TEXT]);
}

void updateTimeout(uint8_t timeout){
    if((int8_t)timeout > 10){
        timeout = 0;
    }
    if((int8_t)timeout < 0){
        timeout = 10;
    }
    eraseSingleElement(&setting_screen_element_list[SCREEN_TIMEOUT_INDEX_TEXT], SETTING_SCREEN_BACKGROUND_COLOR);
    clickable_text_number[SCREEN_TIMEOUT_INDEX_TEXT - TIME_HOUR_TEXT] = timeout;
    if(timeout == 0){
        strcpy(clickable_texts_str[SCREEN_TIMEOUT_INDEX_TEXT - TIME_HOUR_TEXT], "OFF");
    }
    else{
        sprintf(clickable_texts_str[SCREEN_TIMEOUT_INDEX_TEXT - TIME_HOUR_TEXT], "%3d", clickable_text_number[SCREEN_TIMEOUT_INDEX_TEXT - TIME_HOUR_TEXT]);
    }
    drawSingleElement(&setting_screen_element_list[SCREEN_TIMEOUT_INDEX_TEXT]);
}

void updateTextElement(SETTING_SCREEN_ELEMENT_INDEX_t element_index, uint8_t value){
    switch(element_index){
    case TIME_HOUR_TEXT:
        updateTimeHour(value);
        break;
    case TIME_MINUTE_TEXT:
        updateTimeMinute(value);
        break;
    case TIME_SECOND_TEXT:
        updateTimeSecond(value);
        break;
    case BRIGHTNESS_INDEX_TEXT:
        updateBrightness(value);
        break;
    case SCREEN_TIMEOUT_INDEX_TEXT:
        updateTimeout(value);
        break;
    }
}

void updateAutoButton(SETTING_SCREEN_ELEMENT_INDEX_t element_index, bool auto_on){
    button_t* button_ptr = setting_screen_element_list[element_index].element_ptr.button_ptr;
    if(auto_on){
        button_ptr->color = AUTO_BUTTON_ACTIVATE_COLOR;
    }
    else{
        button_ptr->color = AUTO_BUTTON_DEACTIVATE_COLOR;
    }
    drawSingleElement(&setting_screen_element_list[element_index]);
}

static SETTING_SCREEN_ELEMENT_INDEX_t detectPressedElement_setting(uint16_t x, uint16_t y){
    for(int i = 0; i < NUMBER_OF_SETTING_SCREEN_ELEMENTS; i++){
        // Button
        if(setting_screen_element_list[i].type == BUTTON){
            button_t* button_ptr = setting_screen_element_list[i].element_ptr.button_ptr;
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
            text_element_t* text_element_ptr = setting_screen_element_list[i].element_ptr.text_element_ptr;
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

void settingScreenPressed(uint16_t x, uint16_t y){
    SETTING_SCREEN_ELEMENT_INDEX_t element_index = detectPressedElement_setting(x, y);
    switch(element_index){
    case BACK_TO_MAIN_BUTTON:
        break;
    case DISCARD_BUTTON:
        break;
    case SAVE_BUTTON:
        break;
    case TIME_HOUR_TEXT:
    case TIME_MINUTE_TEXT:
    case TIME_SECOND_TEXT:
        if(auto_button_state[TIME_AUTO_BUTTON - TIME_AUTO_BUTTON] == 0){
            // Deactivate old element
            setting_screen_element_list[now_setting].element_ptr.text_element_ptr->color = NORMAL_TEXT;
            updateTextElement(now_setting, clickable_text_number[now_setting - TIME_HOUR_TEXT]);
            // Activate new element
            now_setting = element_index;
            setting_screen_element_list[now_setting].element_ptr.text_element_ptr->color = SELECTED_TEXT;
            updateTextElement(now_setting, clickable_text_number[now_setting - TIME_HOUR_TEXT]);
        }
        break;
    case BRIGHTNESS_INDEX_TEXT:
        if(auto_button_state[BRIGHTNESS_AUTO_BUTTON - TIME_AUTO_BUTTON] == 0){
            // Deactivate old element
            setting_screen_element_list[now_setting].element_ptr.text_element_ptr->color = NORMAL_TEXT;
            updateTextElement(now_setting, clickable_text_number[now_setting - TIME_HOUR_TEXT]);
            // Activate new element
            now_setting = element_index;
            setting_screen_element_list[now_setting].element_ptr.text_element_ptr->color = SELECTED_TEXT;
            updateTextElement(now_setting, clickable_text_number[now_setting - TIME_HOUR_TEXT]);
        }
        break;
    case SCREEN_TIMEOUT_INDEX_TEXT:
        // Deactivate old element
        setting_screen_element_list[now_setting].element_ptr.text_element_ptr->color = NORMAL_TEXT;
        updateTextElement(now_setting, clickable_text_number[now_setting - TIME_HOUR_TEXT]);
        // Activate new element
        now_setting = element_index;
        setting_screen_element_list[now_setting].element_ptr.text_element_ptr->color = SELECTED_TEXT;
        updateTextElement(now_setting, clickable_text_number[now_setting - TIME_HOUR_TEXT]);
        break;
    case TIME_AUTO_BUTTON:
    case BRIGHTNESS_AUTO_BUTTON:
        auto_button_state[element_index - TIME_AUTO_BUTTON] = !auto_button_state[element_index - TIME_AUTO_BUTTON];
        updateAutoButton(element_index, auto_button_state[element_index - TIME_AUTO_BUTTON]);
        break;
    case UP_BUTTON:
        clickable_text_number[now_setting - TIME_HOUR_TEXT]++;
        updateTextElement(now_setting, clickable_text_number[now_setting - TIME_HOUR_TEXT]);
        break;
    case DOWN_BUTTON:
        clickable_text_number[now_setting - TIME_HOUR_TEXT]--;
        updateTextElement(now_setting, clickable_text_number[now_setting - TIME_HOUR_TEXT]);
        break;
    default:
        break;
    }
}
