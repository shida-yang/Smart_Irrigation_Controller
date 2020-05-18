#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include "Screen_Template.h"
#include "weatherServer.h"
#include "Time_Server.h"
#include "WeatherIcon.h"

typedef enum{
    SETTING_BUTTON,
    WATERING_BUTTON,
    SCHEDULE_BUTTON,
    PLAN_BUTTON,
    DATE_TEXT,
    TIME_TEXT,
    RAIN_SUM_TEXT,
    RAIN_TARGET_TEXT,
    DATE0_BUTTON,
    DATE1_BUTTON,
    DATE2_BUTTON,
    DATE3_BUTTON,
    DATE4_BUTTON,
    DATE5_BUTTON,
    DATE6_BUTTON,
    NUMBER_OF_MAIN_SCREEN_ELEMENTS
} MAIN_SCREEN_ELEMENT_INDEX_t;

typedef enum{
    SUN,
    MON,
    TUE,
    WED,
    THU,
    FRI,
    SAT
} WEEKDAYS_t;

#define MAIN_SCREEN_BACKGROUND_COLOR        LCD_WHITE

#define SETTING_BUTTON_COLOR    LCD_GREEN
#define WATERING_BUTTON_COLOR   LCD_BLUE
#define SCHEDULE_BUTTON_COLOR   LCD_YELLOW
#define PLAN_BUTTON_COLOR       LCD_PURPLE

#define DATE_TEXT_SIZE          1
#define DATE_TEXT_COLOR         LCD_BLACK
#define DATE_TEXT_LEN           12
#define DATE_FORMAT             "%02d/%02d/%02d %s"

#define TIME_TEXT_SIZE          3
#define TIME_TEXT_COLOR         LCD_BLACK
#define TIME_TEXT_LEN           8
#define TIME_FORMAT             "%02d:%02d:%02d"

#define RAIN_SUM_TEXT_SIZE          1
#define RAIN_SUM_TEXT_COLOR         LCD_BLACK
#define RAIN_SUM_TEXT_LEN           26
#define RAIN_SUM_FORMAT             "Weekly Rain Sum: %6.2f mm"

#define RAIN_TARGET_TEXT_SIZE          1
#define RAIN_TARGET_TEXT_COLOR         LCD_BLACK
#define RAIN_TARGET_TEXT_LEN           26
#define RAIN_TARGET_FORMAT             "Rain Target:     %6.2f mm"

#define DATE_BUTTON_WIDTH            41
#define DATE_BUTTON_HEIGHT           (CHAR_HEIGHT + (NAV_BAR_BUTTON_PADDING + 1) * 2)
#define DATE_BUTTON_ACTIVE_COLOR     LCD_WHITE
#define DATE_BUTTON_DEACTIVE_COLOR   LCD_LIGHT_GRAY
#define DATE_BUTTON_LOWER_EDGE       (main_screen_element_list[DATE0_BUTTON].element_ptr.button_ptr->y_coordinate + main_screen_element_list[DATE0_BUTTON].element_ptr.button_ptr->height)

#define WEATHER_ICON_WIDTH              64
#define WEATHER_ICON_Y_COORDINATE       (((DATE_BUTTON_LOWER_EDGE+MAX_SCREEN_Y)>>1)-(WEATHER_ICON_WIDTH>>1))
#define WEATHER_ICON_X_COORDINATE       (WEATHER_ICON_Y_COORDINATE-DATE_BUTTON_LOWER_EDGE)

#define WEATHER_TEXT_X_COORDINATE       (WEATHER_ICON_X_COORDINATE + WEATHER_ICON_WIDTH + 2 + SCREEN_MARGIN * 2)
#define WEATHER_TEXT_Y_COORDINATE       WEATHER_ICON_Y_COORDINATE
#define WEATHER_TEXT_FORMAT             "Weather:          %s"

#define HI_TEMP_TEXT_X_COORDINATE       (WEATHER_ICON_X_COORDINATE + WEATHER_ICON_WIDTH + 2 + SCREEN_MARGIN * 2)
#define HI_TEMP_TEXT_Y_COORDINATE       (WEATHER_TEXT_Y_COORDINATE + CHAR_HEIGHT)
#define HI_TEMP_TEXT_FORMAT             "High Temperature: %4.1f F"

#define LO_TEMP_TEXT_X_COORDINATE       (WEATHER_ICON_X_COORDINATE + WEATHER_ICON_WIDTH + 2 + SCREEN_MARGIN * 2)
#define LO_TEMP_TEXT_Y_COORDINATE       (HI_TEMP_TEXT_Y_COORDINATE + CHAR_HEIGHT)
#define LO_TEMP_TEXT_FORMAT             "Low Temperature:  %4.1f F"

#define RAIN_DEPTH_TEXT_X_COORDINATE    (WEATHER_ICON_X_COORDINATE + WEATHER_ICON_WIDTH + 2 + SCREEN_MARGIN * 2)
#define RAIN_DEPTH_TEXT_Y_COORDINATE    (LO_TEMP_TEXT_Y_COORDINATE + CHAR_HEIGHT)
#define RAIN_DEPTH_TEXT_FORMAT          "Rain Depth:    %6.2f mm"

static void generateNavBarButtons();
void generateDateTimeText();
void generateRainSumTarget();
void generateDateButtons();
void drawMainScreen();
void updateDate(uint8_t month, uint8_t date, uint8_t year, WEEKDAYS_t weekday);
void updateTime(uint8_t hour, uint8_t minute, uint8_t second);
void updateWeather(uint8_t date);
void displayDateWeather();
void updateRainSumTartget(float rainSum, float rainTarget);
void updateWeatherIcon(char* weather);
void drawIcon(const unsigned char* icon);

static MAIN_SCREEN_ELEMENT_INDEX_t detectPressedElement_main(uint16_t x, uint16_t y);
void mainScreenPressed(uint16_t x, uint16_t y);

#endif
