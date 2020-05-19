#ifndef SETTING_SCREEN_H
#define SETTING_SCREEN_H

#include "Screen_Template.h"

typedef enum{
    BACK_TO_MAIN_BUTTON,
    DISCARD_BUTTON,
    SAVE_BUTTON,
    TIME_HOUR_TEXT,
    TIME_MINUTE_TEXT,
    TIME_SECOND_TEXT,
    BRIGHTNESS_INDEX_TEXT,
    SCREEN_TIMEOUT_INDEX_TEXT,
    TIME_AUTO_BUTTON,
    BRIGHTNESS_AUTO_BUTTON,
    UP_BUTTON,
    DOWN_BUTTON,
    NUMBER_OF_SETTING_SCREEN_ELEMENTS
} SETTING_SCREEN_ELEMENT_INDEX_t;

#define SETTING_SCREEN_BACKGROUND_COLOR        LCD_WHITE

#define BACK_TO_MAIN_BUTTON_COLOR   LCD_BLUE
#define DISCARD_BUTTON_COLOR        LCD_RED
#define SAVE_BUTTON_COLOR           LCD_GREEN

#define SELECTED_TEXT       LCD_GREEN
#define NORMAL_TEXT         LCD_BLACK

#define SETTING_TIME_TEXT_SIZE  2
#define SETTING_TIME_TEXT_X     SCREEN_MARGIN
#define SETTING_TIME_TEXT_Y     (NAV_BAR_HEIGHT + SCREEN_MARGIN)
#define HOUR_TEXT_X             (SETTING_TIME_TEXT_X + 5 * CHAR_WIDTH * SETTING_TIME_TEXT_SIZE)
#define MINUTE_TEXT_X           (HOUR_TEXT_X + 3 * CHAR_WIDTH * SETTING_TIME_TEXT_SIZE)
#define SECOND_TEXT_X           (MINUTE_TEXT_X + 3 * CHAR_WIDTH * SETTING_TIME_TEXT_SIZE)

#define SETTING_BRIGHTNESS_TEXT_SIZE    2
#define SETTING_BRIGHTNESS_TEXT_X       SCREEN_MARGIN
#define SETTING_BRIGHTNESS_TEXT_Y       (SETTING_TIME_TEXT_Y + CHAR_HEIGHT * SETTING_TIME_TEXT_SIZE)
#define BEIGHTNESS_TEXT_X               (SETTING_BRIGHTNESS_TEXT_X + 12 * CHAR_WIDTH * SETTING_BRIGHTNESS_TEXT_SIZE)

#define SETTING_TIMEOUT_TEXT_SIZE       1
#define SETTING_TIMEOUT_TEXT_X          SCREEN_MARGIN
#define SETTING_TIMEOUT_TEXT_Y_1        (SETTING_BRIGHTNESS_TEXT_Y + CHAR_HEIGHT * SETTING_BRIGHTNESS_TEXT_SIZE + SCREEN_MARGIN)
#define SETTING_TIMEOUT_TEXT_Y_2        (SETTING_TIMEOUT_TEXT_Y_1 + CHAR_HEIGHT * SETTING_TIMEOUT_TEXT_SIZE)
#define TIMEOUT_INDEX_TEXT_SIZE         2
#define TIMEOUT_INDEX_TEXT_X            (SETTING_TIMEOUT_TEXT_X + 10 * CHAR_WIDTH * TIMEOUT_INDEX_TEXT_SIZE)

#define TIME_AUTO_BUTTON_X              (BEIGHTNESS_TEXT_X + CHAR_WIDTH * SETTING_BRIGHTNESS_TEXT_SIZE + SCREEN_MARGIN * 3)
#define TIME_AUTO_BUTTON_Y              (SETTING_TIME_TEXT_Y + 2)
#define BRIGHTNESS_AUTO_BUTTON_X        (BEIGHTNESS_TEXT_X + CHAR_WIDTH * SETTING_BRIGHTNESS_TEXT_SIZE + SCREEN_MARGIN * 3)
#define BRIGHTNESS_AUTO_BUTTON_Y        (SETTING_BRIGHTNESS_TEXT_Y + 2)
#define AUTO_BUTTON_ACTIVATE_COLOR      LCD_GREEN
#define AUTO_BUTTON_DEACTIVATE_COLOR    LCD_LIGHT_GRAY

#define UP_DOWN_BUTTON_PADDING  5
#define UP_DOWN_BUTTON_COLOR    LCD_YELLOW
#define DOWN_BUTTON_X           (MAX_SCREEN_X - SCREEN_MARGIN * 4 - down_button_ptr->width)
#define DOWN_BUTTON_Y           (MAX_SCREEN_Y - SCREEN_MARGIN * 4 - down_button_ptr->height)
#define UP_BUTTON_X             DOWN_BUTTON_X
#define UP_BUTTON_Y             (DOWN_BUTTON_Y - SCREEN_MARGIN * 4 - up_button_ptr->height)

static void generateNavBarButtons();
static void genetateTime();
static void generateBrightness();
static void generateScreenTimeout();
static void generateAutoButtons();
static void generateUpDownButtons();
void drawSettingScreen();
void updateTimeHour(uint8_t hour);
void updateTimeMinute(uint8_t minute);
void updateTimeSecond(uint8_t second);
void updateBrightness(uint8_t brightness);
void updateTimeout(uint8_t timeout);
void updateTextElement(SETTING_SCREEN_ELEMENT_INDEX_t element_index, uint8_t value);
void updateAutoButton(SETTING_SCREEN_ELEMENT_INDEX_t element_index, bool auto_on);

static SETTING_SCREEN_ELEMENT_INDEX_t detectPressedElement_setting(uint16_t x, uint16_t y);
void settingScreenPressed(uint16_t x, uint16_t y);

#endif
