#ifndef SCHEDULE_SCREEN_H
#define SCHEDULE_SCREEN_H

#include "Screen_Template.h"

typedef enum{
    SCHEDULE_BACK_TO_MAIN_BUTTON,
    SCHEDULE_DISCARD_BUTTON,
    SCHEDULE_SAVE_BUTTON,

    SCHEDULE_UP_BUTTON,
    SCHEDULE_DOWN_BUTTON,

    SUN_TEXT,
    MON_TEXT,
    TUE_TEXT,
    WED_TEXT,
    THU_TEXT,
    FRI_TEXT,
    SAT_TEXT,

    HOUR_NUM,
    MIN_NUM,

    NUMBER_OF_SCHEDULE_SCREEN_ELEMENTS
} SCHEDULE_SCREEN_ELEMENT_INDEX_t;

#define SCHEDULE_SCREEN_BACKGROUND_COLOR        LCD_WHITE

#define BACK_TO_MAIN_BUTTON_COLOR   LCD_BLUE
#define DISCARD_BUTTON_COLOR        LCD_RED
#define SAVE_BUTTON_COLOR           LCD_GREEN

#define SELECTED_TEXT       LCD_GREEN
#define NORMAL_TEXT         LCD_BLACK

#define UP_DOWN_BUTTON_PADDING  5
#define UP_DOWN_BUTTON_COLOR    LCD_YELLOW
#define DOWN_BUTTON_X           (MAX_SCREEN_X - SCREEN_MARGIN * 4 - down_button_ptr->width)
#define DOWN_BUTTON_Y           (MAX_SCREEN_Y - SCREEN_MARGIN * 4 - down_button_ptr->height)
#define UP_BUTTON_X             DOWN_BUTTON_X
#define UP_BUTTON_Y             (DOWN_BUTTON_Y - SCREEN_MARGIN * 4 - up_button_ptr->height)

static void generateNavBarButtons();
static void generateUpDownButtons();
static void generateFixedText();
static void generateDaysOfWeek();
static void generateHourMinute();
void drawScheduleScreen();
void updateScheduleHour(int8_t hour);
void updateScheduleMinute(int8_t minute);


static SCHEDULE_SCREEN_ELEMENT_INDEX_t detectPressedElement_schedule(uint16_t x, uint16_t y);
void scheduleScreenPressed(uint16_t x, uint16_t y);

#endif
