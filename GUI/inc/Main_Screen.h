#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include "Screen_Template.h"

typedef enum{
    SETTING_BUTTON,
    WATERING_BUTTON,
    SCHEDULE_BUTTON,
    DATE_TEXT,
    TIME_TEXT,
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

#define DATE_TEXT_SIZE          1
#define DATE_TEXT_COLOR         LCD_BLACK
#define DATE_TEXT_LEN           12
#define DATE_FORMAT             "%02d/%02d/%02d %s"

#define TIME_TEXT_SIZE          3
#define TIME_TEXT_COLOR         LCD_BLACK
#define TIME_TEXT_LEN           8
#define TIME_FORMAT             "%02d:%02d:%02d"

void generateNavBarButtons();
void generateDateTimeText();
void drawMainScreen();
void updateDate(uint8_t month, uint8_t date, uint8_t year, WEEKDAYS_t weekday);
void updateTime(uint8_t hour, uint8_t minute, uint8_t second);
void updateWeather();


#endif
