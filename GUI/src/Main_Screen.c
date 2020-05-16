#include "Main_Screen.h"

extern dailyWeatherData_t weekWeatherData[7];

screen_element_t main_screen_element_list[NUMBER_OF_MAIN_SCREEN_ELEMENTS];
button_t setting_button, watering_button, schedule_button;
button_t weekday_buttons[7];
char weekdays_array[][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
text_element_t date_text_element, time_text_element;
char date_str[DATE_TEXT_LEN + 1] = "--/--/-- ---";
char time_str[TIME_TEXT_LEN + 1] = "--:--:--";
WEEKDAYS_t currently_active_weekday = SUN;

void generateNavBarButtons(){
    main_screen_element_list[SETTING_BUTTON].type = BUTTON;
    main_screen_element_list[SETTING_BUTTON].element_ptr.button_ptr = &setting_button;
    button_t* setting_button_ptr = main_screen_element_list[SETTING_BUTTON].element_ptr.button_ptr;
    setting_button_ptr->color = SETTING_BUTTON_COLOR;
    strcpy(setting_button_ptr->text, "Setting");
    adjustButtonSizeBasedOnText(setting_button_ptr, NAV_BAR_BUTTON_PADDING);
    setting_button_ptr->y_coordinate = SCREEN_MARGIN;
    setting_button_ptr->x_coordinate = MAX_SCREEN_X - SCREEN_MARGIN - setting_button_ptr->width;

    main_screen_element_list[WATERING_BUTTON].type = BUTTON;
    main_screen_element_list[WATERING_BUTTON].element_ptr.button_ptr = &watering_button;
    button_t* watering_button_ptr = main_screen_element_list[WATERING_BUTTON].element_ptr.button_ptr;
    watering_button_ptr->color = WATERING_BUTTON_COLOR;
    strcpy(watering_button_ptr->text, "Watering");
    adjustButtonSizeBasedOnText(watering_button_ptr, NAV_BAR_BUTTON_PADDING);
    watering_button_ptr->y_coordinate = SCREEN_MARGIN;
    watering_button_ptr->x_coordinate = setting_button_ptr->x_coordinate - SCREEN_MARGIN - watering_button_ptr->width;

    main_screen_element_list[SCHEDULE_BUTTON].type = BUTTON;
    main_screen_element_list[SCHEDULE_BUTTON].element_ptr.button_ptr = &schedule_button;
    button_t* schedule_button_ptr = main_screen_element_list[SCHEDULE_BUTTON].element_ptr.button_ptr;
    schedule_button_ptr->color = SCHEDULE_BUTTON_COLOR;
    strcpy(schedule_button_ptr->text, "Schedule");
    adjustButtonSizeBasedOnText(schedule_button_ptr, NAV_BAR_BUTTON_PADDING);
    schedule_button_ptr->y_coordinate = SCREEN_MARGIN;
    schedule_button_ptr->x_coordinate = watering_button_ptr->x_coordinate - SCREEN_MARGIN - schedule_button_ptr->width;
}

void generateDateTimeText(){
    main_screen_element_list[DATE_TEXT].type = TEXT;
    main_screen_element_list[DATE_TEXT].element_ptr.text_element_ptr = &date_text_element;
    text_element_t* date_text_ptr = main_screen_element_list[DATE_TEXT].element_ptr.text_element_ptr;
    date_text_ptr->color = DATE_TEXT_COLOR;
    date_text_ptr->size = DATE_TEXT_SIZE;
    date_text_ptr->text_ptr = date_str;
    date_text_ptr->y_coordinate = NAV_BAR_HEIGHT + SCREEN_MARGIN;
    date_text_ptr->x_coordinate = (MAX_SCREEN_X >> 1) - ((DATE_TEXT_LEN * CHAR_WIDTH * DATE_TEXT_SIZE) >> 1);

    main_screen_element_list[TIME_TEXT].type = TEXT;
    main_screen_element_list[TIME_TEXT].element_ptr.text_element_ptr = &time_text_element;
    text_element_t* time_text_ptr = main_screen_element_list[TIME_TEXT].element_ptr.text_element_ptr;
    time_text_ptr->color = TIME_TEXT_COLOR;
    time_text_ptr->size = TIME_TEXT_SIZE;
    time_text_ptr->text_ptr = time_str;
    time_text_ptr->y_coordinate = date_text_ptr->y_coordinate + CHAR_HEIGHT * DATE_TEXT_SIZE;
    time_text_ptr->x_coordinate = (MAX_SCREEN_X >> 1) - ((TIME_TEXT_LEN * CHAR_WIDTH * TIME_TEXT_SIZE) >> 1);
}

void generateWeekdayButtons(){
    uint16_t current_x_coordinate = 4;
    for(WEEKDAYS_t i = SUN; i <= SAT; i++){
        main_screen_element_list[i+5].type = BUTTON;
        main_screen_element_list[i+5].element_ptr.button_ptr = &weekday_buttons[i];
        button_t* curr_weekday_button_ptr = main_screen_element_list[i+5].element_ptr.button_ptr;

        if(i == currently_active_weekday){
            curr_weekday_button_ptr->color = WEEKDAY_BUTTON_ACTIVE_COLOR;
        }
        else{
            curr_weekday_button_ptr->color = WEEKDAY_BUTTON_DEACTIVE_COLOR;
        }

        strcpy(curr_weekday_button_ptr->text, &weekdays_array[i]);

        curr_weekday_button_ptr->width = WEEKDAY_BUTTON_WIDTH;
        curr_weekday_button_ptr->height = WEEKDAY_BUTTON_HEIGHT;

        curr_weekday_button_ptr->y_coordinate = main_screen_element_list[TIME_TEXT].element_ptr.text_element_ptr->y_coordinate + CHAR_HEIGHT * TIME_TEXT_SIZE;
        curr_weekday_button_ptr->x_coordinate = current_x_coordinate;

        current_x_coordinate += (WEEKDAY_BUTTON_WIDTH + 4);
    }
}

void drawMainScreen(){
    generateNavBarButtons();
    drawNavBarLine(LCD_BLACK);
    generateDateTimeText();
    generateWeekdayButtons();
    drawAllElements(main_screen_element_list, NUMBER_OF_MAIN_SCREEN_ELEMENTS);
}

void updateDate(uint8_t month, uint8_t date, uint8_t year, WEEKDAYS_t weekday){
    eraseSingleElement(&main_screen_element_list[DATE_TEXT], MAIN_SCREEN_BACKGROUND_COLOR);
    sprintf(date_str, DATE_FORMAT, month, date, year, &weekdays_array[weekday]);
    drawSingleElement(&main_screen_element_list[DATE_TEXT]);
}

void updateTime(uint8_t hour, uint8_t minute, uint8_t second){
    eraseSingleElement(&main_screen_element_list[TIME_TEXT], MAIN_SCREEN_BACKGROUND_COLOR);
    sprintf(time_str, TIME_FORMAT, hour, minute, second);
    drawSingleElement(&main_screen_element_list[TIME_TEXT]);
}

void updateWeather(WEEKDAYS_t weekday){

}
