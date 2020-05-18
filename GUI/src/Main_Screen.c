#include "Main_Screen.h"

extern dailyWeatherData_t weekWeatherData[7];

static screen_element_t main_screen_element_list[NUMBER_OF_MAIN_SCREEN_ELEMENTS];
static button_t setting_button, watering_button, schedule_button, plan_button;
static button_t date_buttons[7];
static char weekdays_array[][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
static text_element_t date_text_element, time_text_element, rain_sum_text_element, rain_target_text_element;
static char date_str[DATE_TEXT_LEN + 1] = "            ";
static char time_str[TIME_TEXT_LEN + 1] = "        ";
static char rain_sum_str[RAIN_SUM_TEXT_LEN + 1] =       "Weekly Rain Sum:       ";
static char rain_target_str[RAIN_TARGET_TEXT_LEN + 1] = "Rain Target:           ";
static uint8_t currently_active_date = 4;

static void generateNavBarButtons(){
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

    main_screen_element_list[PLAN_BUTTON].type = BUTTON;
    main_screen_element_list[PLAN_BUTTON].element_ptr.button_ptr = &plan_button;
    button_t* plan_button_ptr = main_screen_element_list[PLAN_BUTTON].element_ptr.button_ptr;
    plan_button_ptr->color = PLAN_BUTTON_COLOR;
    strcpy(plan_button_ptr->text, "Plan");
    adjustButtonSizeBasedOnText(plan_button_ptr, NAV_BAR_BUTTON_PADDING);
    plan_button_ptr->y_coordinate = SCREEN_MARGIN;
    plan_button_ptr->x_coordinate = schedule_button_ptr->x_coordinate - SCREEN_MARGIN - plan_button_ptr->width;
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

void generateRainSumTarget(){
    main_screen_element_list[RAIN_SUM_TEXT].type = TEXT;
    main_screen_element_list[RAIN_SUM_TEXT].element_ptr.text_element_ptr = &rain_sum_text_element;
    text_element_t* rain_sum_text_ptr = main_screen_element_list[RAIN_SUM_TEXT].element_ptr.text_element_ptr;
    rain_sum_text_ptr->color = RAIN_SUM_TEXT_COLOR;
    rain_sum_text_ptr->size = RAIN_SUM_TEXT_SIZE;
    rain_sum_text_ptr->text_ptr = rain_sum_str;
    rain_sum_text_ptr->y_coordinate = main_screen_element_list[TIME_TEXT].element_ptr.text_element_ptr->y_coordinate + CHAR_HEIGHT * TIME_TEXT_SIZE;
    rain_sum_text_ptr->x_coordinate = SCREEN_MARGIN;

    main_screen_element_list[RAIN_TARGET_TEXT].type = TEXT;
    main_screen_element_list[RAIN_TARGET_TEXT].element_ptr.text_element_ptr = &rain_target_text_element;
    text_element_t* rain_target_text_ptr = main_screen_element_list[RAIN_TARGET_TEXT].element_ptr.text_element_ptr;
    rain_target_text_ptr->color = RAIN_TARGET_TEXT_COLOR;
    rain_target_text_ptr->size = RAIN_TARGET_TEXT_SIZE;
    rain_target_text_ptr->text_ptr = rain_target_str;
    rain_target_text_ptr->y_coordinate = rain_sum_text_ptr->y_coordinate + CHAR_HEIGHT * RAIN_TARGET_TEXT_SIZE;
    rain_target_text_ptr->x_coordinate = SCREEN_MARGIN;
}

void generateDateButtons(){
    fillInWeekWeatherData();
    uint16_t current_x_coordinate = 4;
    for(int i = 0; i <= 6; i++){
        main_screen_element_list[i+DATE0_BUTTON].type = BUTTON;
        main_screen_element_list[i+DATE0_BUTTON].element_ptr.button_ptr = &date_buttons[i];
        button_t* curr_date_button_ptr = main_screen_element_list[i+DATE0_BUTTON].element_ptr.button_ptr;

        if(i == currently_active_date){
            curr_date_button_ptr->color = DATE_BUTTON_ACTIVE_COLOR;
        }
        else{
            curr_date_button_ptr->color = DATE_BUTTON_DEACTIVE_COLOR;
        }

//        strcpy(curr_date_button_ptr->text, &weekdays_array[i]);
        sprintf(curr_date_button_ptr->text, "%02d/%02d", weekWeatherData[i].month, weekWeatherData[i].day);

        curr_date_button_ptr->width = DATE_BUTTON_WIDTH;
        curr_date_button_ptr->height = DATE_BUTTON_HEIGHT;

        curr_date_button_ptr->y_coordinate = main_screen_element_list[RAIN_TARGET_TEXT].element_ptr.text_element_ptr->y_coordinate + CHAR_HEIGHT * RAIN_TARGET_TEXT_SIZE;
        curr_date_button_ptr->x_coordinate = current_x_coordinate;

        current_x_coordinate += (DATE_BUTTON_WIDTH + 4);
    }
}

void drawMainScreen(){
    LCD_Clear(LCD_WHITE);
    generateNavBarButtons();
    drawNavBarLine(LCD_BLACK);
    generateDateTimeText();
    generateRainSumTarget();
    generateDateButtons();
    drawAllElements(main_screen_element_list, NUMBER_OF_MAIN_SCREEN_ELEMENTS);
    displayDateWeather();
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

void updateWeather(uint8_t date){
    main_screen_element_list[currently_active_date+5].element_ptr.button_ptr->color = DATE_BUTTON_DEACTIVE_COLOR;
    currently_active_date = date;
    main_screen_element_list[currently_active_date+5].element_ptr.button_ptr->color = DATE_BUTTON_ACTIVE_COLOR;
    displayDateWeather();
}

void displayDateWeather(){
    uint16_t weather_section_min_y = main_screen_element_list[DATE0_BUTTON].element_ptr.button_ptr->y_coordinate + main_screen_element_list[DATE0_BUTTON].element_ptr.button_ptr->height + SCREEN_MARGIN;
    LCD_DrawRectangle(MIN_SCREEN_X, MAX_SCREEN_X-1, weather_section_min_y, MAX_SCREEN_Y-1, MAIN_SCREEN_BACKGROUND_COLOR);

    dailyWeatherData_t* curr_weather_ptr = &weekWeatherData[currently_active_date];
    updateWeatherIcon(curr_weather_ptr->weather);

    char buf[50];

    sprintf(buf, WEATHER_TEXT_FORMAT, curr_weather_ptr->weather);
    LCD_Text(WEATHER_TEXT_X_COORDINATE, WEATHER_TEXT_Y_COORDINATE, buf, LCD_BLACK);

    sprintf(buf, HI_TEMP_TEXT_FORMAT, curr_weather_ptr->max_temp_f);
    LCD_Text(HI_TEMP_TEXT_X_COORDINATE, HI_TEMP_TEXT_Y_COORDINATE, buf, LCD_BLACK);

    sprintf(buf, LO_TEMP_TEXT_FORMAT, curr_weather_ptr->min_temp_f);
    LCD_Text(LO_TEMP_TEXT_X_COORDINATE, LO_TEMP_TEXT_Y_COORDINATE, buf, LCD_BLACK);

    sprintf(buf, RAIN_DEPTH_TEXT_FORMAT, curr_weather_ptr->rain_depth);
    LCD_Text(RAIN_DEPTH_TEXT_X_COORDINATE, RAIN_DEPTH_TEXT_Y_COORDINATE, buf, LCD_BLACK);
}

void updateRainSumTartget(float rainSum, float rainTarget){
    eraseSingleElement(&main_screen_element_list[RAIN_SUM_TEXT], MAIN_SCREEN_BACKGROUND_COLOR);
    sprintf(rain_sum_str, RAIN_SUM_FORMAT, rainSum);
    drawSingleElement(&main_screen_element_list[RAIN_SUM_TEXT]);

    eraseSingleElement(&main_screen_element_list[RAIN_TARGET_TEXT], MAIN_SCREEN_BACKGROUND_COLOR);
    sprintf(rain_target_str, RAIN_TARGET_FORMAT, rainTarget);
    drawSingleElement(&main_screen_element_list[RAIN_TARGET_TEXT]);
}

void updateWeatherIcon(char* weather){
    if(strcmp(weather, "Clear") == 0){
        drawIcon(Clear);
    }
    else if(strcmp(weather, "Clouds") == 0){
        drawIcon(Clouds);
    }
    else if(strcmp(weather, "Snow") == 0){
        drawIcon(Snow);
    }
    else if(strcmp(weather, "Rain") == 0){
        drawIcon(Rain);
    }
    else if(strcmp(weather, "Drizzle") == 0){
        drawIcon(Drizzle);
    }
    else if(strcmp(weather, "Thunderstorm") == 0){
        drawIcon(Thunderstorm);
    }
    else{
        uint16_t text_len = strlen(weather);
        uint16_t x_coordinate = WEATHER_ICON_X_COORDINATE + (WEATHER_ICON_WIDTH>>1) - text_len * CHAR_WIDTH_D2;
        uint16_t y_coordinate = WEATHER_ICON_Y_COORDINATE + (WEATHER_ICON_WIDTH>>1) - CHAR_HEIGHT_D2;
    }
}

void drawIcon(const unsigned char* icon){
    LCD_DrawRectangle_edge(WEATHER_ICON_X_COORDINATE - 1, WEATHER_ICON_X_COORDINATE + WEATHER_ICON_WIDTH + 1,
                           WEATHER_ICON_Y_COORDINATE - 1, WEATHER_ICON_Y_COORDINATE + WEATHER_ICON_WIDTH + 1,
                           MAIN_SCREEN_BACKGROUND_COLOR, LCD_BLACK
    );

    for(int i=0; i<32; i++){
        for(int j=0; j<4; j++){
            for(int k=0; k<8; k++){
                if( ((icon[i*4+j]>>(7-k))&1) == 1 ){
                    LCD_SetPoint((j*8+k)*2+WEATHER_ICON_X_COORDINATE+0, (i*2+WEATHER_ICON_Y_COORDINATE)+0, LCD_BLACK);
                    LCD_SetPoint((j*8+k)*2+WEATHER_ICON_X_COORDINATE+0, (i*2+WEATHER_ICON_Y_COORDINATE)+1, LCD_BLACK);
                    LCD_SetPoint((j*8+k)*2+WEATHER_ICON_X_COORDINATE+1, (i*2+WEATHER_ICON_Y_COORDINATE)+0, LCD_BLACK);
                    LCD_SetPoint((j*8+k)*2+WEATHER_ICON_X_COORDINATE+1, (i*2+WEATHER_ICON_Y_COORDINATE)+1, LCD_BLACK);
                }
                else{
                    LCD_SetPoint((j*8+k)*2+WEATHER_ICON_X_COORDINATE+0, (i*2+WEATHER_ICON_Y_COORDINATE)+0, LCD_WHITE);
                    LCD_SetPoint((j*8+k)*2+WEATHER_ICON_X_COORDINATE+0, (i*2+WEATHER_ICON_Y_COORDINATE)+1, LCD_WHITE);
                    LCD_SetPoint((j*8+k)*2+WEATHER_ICON_X_COORDINATE+1, (i*2+WEATHER_ICON_Y_COORDINATE)+0, LCD_WHITE);
                    LCD_SetPoint((j*8+k)*2+WEATHER_ICON_X_COORDINATE+1, (i*2+WEATHER_ICON_Y_COORDINATE)+1, LCD_WHITE);
                }
            }
        }
    }
}
