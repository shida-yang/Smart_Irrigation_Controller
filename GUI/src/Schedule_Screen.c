#include "Schedule_Screen.h"

static screen_element_t schedule_screen_element_list[NUMBER_OF_SCHEDULE_SCREEN_ELEMENTS];
static button_t buttons[5];
// Hours and Minutes
static text_element_t hour_minute_texts[2];
static char hour_minute_str[2][3];
static uint8_t hour_minute_number[2];
static int now_setting = -1;
// Days of Week
static text_element_t days_of_week[7];
static bool days_of_week_active[7];
static char* days_of_week_str[7] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static void generateNavBarButtons(){
    schedule_screen_element_list[SCHEDULE_SAVE_BUTTON].type = BUTTON;
    schedule_screen_element_list[SCHEDULE_SAVE_BUTTON].element_ptr.button_ptr = &buttons[SCHEDULE_SAVE_BUTTON];
    button_t* save_button_ptr = schedule_screen_element_list[SCHEDULE_SAVE_BUTTON].element_ptr.button_ptr;
    save_button_ptr->color = SAVE_BUTTON_COLOR;
    strcpy(save_button_ptr->text, "Save");
    adjustButtonSizeBasedOnText(save_button_ptr, NAV_BAR_BUTTON_PADDING);
    save_button_ptr->y_coordinate = SCREEN_MARGIN;
    save_button_ptr->x_coordinate = MAX_SCREEN_X - SCREEN_MARGIN - save_button_ptr->width;

    schedule_screen_element_list[SCHEDULE_DISCARD_BUTTON].type = BUTTON;
    schedule_screen_element_list[SCHEDULE_DISCARD_BUTTON].element_ptr.button_ptr = &buttons[SCHEDULE_DISCARD_BUTTON];
    button_t* discard_button_ptr = schedule_screen_element_list[SCHEDULE_DISCARD_BUTTON].element_ptr.button_ptr;
    discard_button_ptr->color = DISCARD_BUTTON_COLOR;
    strcpy(discard_button_ptr->text, "Discard");
    adjustButtonSizeBasedOnText(discard_button_ptr, NAV_BAR_BUTTON_PADDING);
    discard_button_ptr->y_coordinate = SCREEN_MARGIN;
    discard_button_ptr->x_coordinate = save_button_ptr->x_coordinate - SCREEN_MARGIN - discard_button_ptr->width;

    schedule_screen_element_list[SCHEDULE_BACK_TO_MAIN_BUTTON].type = BUTTON;
    schedule_screen_element_list[SCHEDULE_BACK_TO_MAIN_BUTTON].element_ptr.button_ptr = &buttons[SCHEDULE_BACK_TO_MAIN_BUTTON];
    button_t* back_to_main_button_ptr = schedule_screen_element_list[SCHEDULE_BACK_TO_MAIN_BUTTON].element_ptr.button_ptr;
    back_to_main_button_ptr->color = BACK_TO_MAIN_BUTTON_COLOR;
    strcpy(back_to_main_button_ptr->text, "< Main");
    adjustButtonSizeBasedOnText(back_to_main_button_ptr, NAV_BAR_BUTTON_PADDING);
    back_to_main_button_ptr->y_coordinate = SCREEN_MARGIN;
    back_to_main_button_ptr->x_coordinate = MIN_SCREEN_X + SCREEN_MARGIN;
}

static void generateUpDownButtons(){
    schedule_screen_element_list[SCHEDULE_DOWN_BUTTON].type = BUTTON;
    schedule_screen_element_list[SCHEDULE_DOWN_BUTTON].element_ptr.button_ptr = &buttons[SCHEDULE_DOWN_BUTTON];
    button_t* down_button_ptr = schedule_screen_element_list[SCHEDULE_DOWN_BUTTON].element_ptr.button_ptr;
    down_button_ptr->color = UP_DOWN_BUTTON_COLOR;
    strcpy(down_button_ptr->text, "DOWN");
    adjustButtonSizeBasedOnText(down_button_ptr, UP_DOWN_BUTTON_PADDING);
    down_button_ptr->y_coordinate = DOWN_BUTTON_Y;
    down_button_ptr->x_coordinate = DOWN_BUTTON_X;

    schedule_screen_element_list[SCHEDULE_UP_BUTTON].type = BUTTON;
    schedule_screen_element_list[SCHEDULE_UP_BUTTON].element_ptr.button_ptr = &buttons[SCHEDULE_UP_BUTTON];
    button_t* up_button_ptr = schedule_screen_element_list[SCHEDULE_UP_BUTTON].element_ptr.button_ptr;
    up_button_ptr->color = UP_DOWN_BUTTON_COLOR;
    strcpy(up_button_ptr->text, " UP ");
    adjustButtonSizeBasedOnText(up_button_ptr, UP_DOWN_BUTTON_PADDING);
    up_button_ptr->y_coordinate = UP_BUTTON_Y;
    up_button_ptr->x_coordinate = UP_BUTTON_X;
}

static void generateDaysOfWeek(){

    for(int i = 0; i < 7; i++){
        days_of_week_active[i] = 0;
        schedule_screen_element_list[i + SUN_TEXT].type = TEXT;
        schedule_screen_element_list[i + SUN_TEXT].element_ptr.text_element_ptr = &days_of_week[i];
        text_element_t* day_ptr = schedule_screen_element_list[i + SUN_TEXT].element_ptr.text_element_ptr;
        day_ptr->color = NORMAL_TEXT;
        day_ptr->size = 1;
        day_ptr->text_ptr = days_of_week_str[i];
        day_ptr->y_coordinate = (NAV_BAR_HEIGHT + SCREEN_MARGIN);
        day_ptr->x_coordinate = SCREEN_MARGIN*4 + i * (SCREEN_MARGIN*5 + CHAR_WIDTH * 3);
    }
}

static void generateHourMinute(){
    for(int i=0; i<2; i++){
        schedule_screen_element_list[i + HOUR_NUM].type = TEXT;
        schedule_screen_element_list[i + HOUR_NUM].element_ptr.text_element_ptr = &hour_minute_texts[i];
        text_element_t* ptr = schedule_screen_element_list[i + HOUR_NUM].element_ptr.text_element_ptr;
        ptr->color = NORMAL_TEXT;
        ptr->size = 2;
        ptr->text_ptr = hour_minute_str[i];
        ptr->y_coordinate = (NAV_BAR_HEIGHT + SCREEN_MARGIN + CHAR_HEIGHT*3);
        ptr->x_coordinate = SCREEN_MARGIN + i * (CHAR_WIDTH * 2 * 3);

        sprintf(hour_minute_str[i], "%02d", hour_minute_number[i]);
    }
}

void drawScheduleScreen(){
    LCD_Clear(LCD_WHITE);
    generateNavBarButtons();
    drawNavBarLine(LCD_BLACK);
    generateUpDownButtons();
    generateDaysOfWeek();
    generateHourMinute();

    LCD_Text_size(SCREEN_MARGIN, NAV_BAR_HEIGHT + SCREEN_MARGIN + CHAR_HEIGHT, "Start time:", LCD_BLACK, 2);
    LCD_Text_size(SCREEN_MARGIN + CHAR_WIDTH * 2 * 2, NAV_BAR_HEIGHT + SCREEN_MARGIN + CHAR_HEIGHT*3, ":", LCD_BLACK, 2);

    drawAllElements(schedule_screen_element_list, NUMBER_OF_SCHEDULE_SCREEN_ELEMENTS);

    updateScheduleHour(5);
    updateScheduleMinute(12);
}

void updateScheduleHour(int8_t hour){

    if(hour >= 24){
        hour_minute_number[0] = 0;
    }
    else if(hour <=-1){
        hour_minute_number[0] = 23;
    }
    else{
        hour_minute_number[0] = hour;
    }

    eraseSingleElement(&schedule_screen_element_list[HOUR_NUM], LCD_WHITE);
    sprintf(hour_minute_str[0], "%02d", hour_minute_number[0]);
    drawSingleElement(&schedule_screen_element_list[HOUR_NUM]);
}

void updateScheduleMinute(int8_t minute){
    if(minute >= 60){
        hour_minute_number[1] = 0;
    }
    else if(minute <=-1){
        hour_minute_number[1] = 59;
    }
    else{
        hour_minute_number[1] = minute;
    }

    eraseSingleElement(&schedule_screen_element_list[MIN_NUM], LCD_WHITE);
    sprintf(hour_minute_str[1], "%02d", hour_minute_number[1]);
    drawSingleElement(&schedule_screen_element_list[MIN_NUM]);
}

static SCHEDULE_SCREEN_ELEMENT_INDEX_t detectPressedElement_schedule(uint16_t x, uint16_t y){
    for(int i = 0; i < NUMBER_OF_SCHEDULE_SCREEN_ELEMENTS; i++){
        // Button
        if(schedule_screen_element_list[i].type == BUTTON){
            button_t* button_ptr = schedule_screen_element_list[i].element_ptr.button_ptr;
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
            text_element_t* text_element_ptr = schedule_screen_element_list[i].element_ptr.text_element_ptr;
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

void scheduleScreenPressed(uint16_t x, uint16_t y){
    SCHEDULE_SCREEN_ELEMENT_INDEX_t element_index = detectPressedElement_schedule(x, y);
    switch(element_index){
    case SCHEDULE_BACK_TO_MAIN_BUTTON:
        break;
    case SCHEDULE_DISCARD_BUTTON:
        break;
    case SCHEDULE_SAVE_BUTTON:
        break;

    case SCHEDULE_UP_BUTTON:
//        if(now_setting>=0){
//            hour_minute_number[now_setting - HOUR_NUM]++;
//            if(now_setting == HOUR_NUM){
//                if(hour_minute_number[now_setting - HOUR_NUM] >= 24){
//                    hour_minute_number[now_setting - HOUR_NUM] = 0;
//                }
//            }
//            else{
//                if(hour_minute_number[now_setting - HOUR_NUM] >= 60){
//                    hour_minute_number[now_setting - HOUR_NUM] = 0;
//                }
//            }
//            eraseSingleElement(&schedule_screen_element_list[now_setting], LCD_WHITE);
//            sprintf(hour_minute_str[now_setting - HOUR_NUM], "%02d", hour_minute_number[now_setting - HOUR_NUM]);
//            drawSingleElement(&schedule_screen_element_list[now_setting]);
//        }
        if(now_setting == HOUR_NUM){
            int8_t hour = hour_minute_number[0];
            hour++;
            updateScheduleHour(hour);
        }
        else{
            int8_t minute = hour_minute_number[1];
            minute++;
            updateScheduleMinute(minute);
        }
        break;
    case SCHEDULE_DOWN_BUTTON:
//        if(now_setting>=0){
//            hour_minute_number[now_setting - HOUR_NUM]--;
//            if(now_setting == HOUR_NUM){
//                if(hour_minute_number[now_setting - HOUR_NUM] >= 24){
//                    hour_minute_number[now_setting - HOUR_NUM] = 23;
//                }
//            }
//            else{
//                if(hour_minute_number[now_setting - HOUR_NUM] >= 60){
//                    hour_minute_number[now_setting - HOUR_NUM] = 59;
//                }
//            }
//            eraseSingleElement(&schedule_screen_element_list[now_setting], LCD_WHITE);
//            sprintf(hour_minute_str[now_setting - HOUR_NUM], "%02d", hour_minute_number[now_setting - HOUR_NUM]);
//            drawSingleElement(&schedule_screen_element_list[now_setting]);
//        }
        if(now_setting == HOUR_NUM){
            int8_t hour = hour_minute_number[0];
            hour--;
            updateScheduleHour(hour);
        }
        else{
            int8_t minute = hour_minute_number[1];
            minute--;
            updateScheduleMinute(minute);
        }
        break;

    case SUN_TEXT:
    case MON_TEXT:
    case TUE_TEXT:
    case WED_TEXT:
    case THU_TEXT:
    case FRI_TEXT:
    case SAT_TEXT:
        days_of_week_active[element_index - SUN_TEXT] = !days_of_week_active[element_index - SUN_TEXT];
        if(days_of_week_active[element_index - SUN_TEXT] == 1){
            schedule_screen_element_list[element_index].element_ptr.text_element_ptr->color = SELECTED_TEXT;
        }
        else{
            schedule_screen_element_list[element_index].element_ptr.text_element_ptr->color = NORMAL_TEXT;
        }
        drawSingleElement(&schedule_screen_element_list[element_index]);
        break;


    case HOUR_NUM:
        if(now_setting != HOUR_NUM){
            schedule_screen_element_list[now_setting].element_ptr.text_element_ptr->color = NORMAL_TEXT;
            drawSingleElement(&schedule_screen_element_list[now_setting]);
            now_setting = HOUR_NUM;
            schedule_screen_element_list[now_setting].element_ptr.text_element_ptr->color = SELECTED_TEXT;
            drawSingleElement(&schedule_screen_element_list[now_setting]);
        }
        break;

    case MIN_NUM:
        if(now_setting != MIN_NUM){
            schedule_screen_element_list[now_setting].element_ptr.text_element_ptr->color = NORMAL_TEXT;
            drawSingleElement(&schedule_screen_element_list[now_setting]);
            now_setting = MIN_NUM;
            schedule_screen_element_list[now_setting].element_ptr.text_element_ptr->color = SELECTED_TEXT;
            drawSingleElement(&schedule_screen_element_list[now_setting]);
        }
        break;
    }
}


