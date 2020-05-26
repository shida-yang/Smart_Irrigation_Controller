#ifndef WATERING_SCREEN_H
#define WATERING_SCREEN_H

#include "Screen_Template.h"

typedef enum{
    WATERING_BACK_TO_MAIN_BUTTON,

    WATERING_ZONE1_LINE,
    WATERING_ZONE2_LINE,
    WATERING_ZONE3_LINE,
    WATERING_ZONE4_LINE,
    WATERING_ZONE5_LINE,
    WATERING_ZONE6_LINE,
    WATERING_ZONE7_LINE,

    CURR_ZONE_INDICATOR,
    SWITCH_ZONE_BUTTON,

    NUMBER_OF_WATERING_SCREEN_ELEMENTS
} WATERING_SCREEN_ELEMENT_INDEX_t;

#define WATERING_SCREEN_BACKGROUND_COLOR    LCD_WHITE

#define WATERING_BACK_TO_MAIN_BUTTON_COLOR      LCD_BLUE

#define SELECTED_TEXT       LCD_GREEN
#define NORMAL_TEXT         LCD_BLACK

#define CURR_ZONE_BUTTON_COLOR      LCD_YELLOW
#define CURR_ZONE_X                 SCREEN_MARGIN
#define CURR_ZONE_WIDTH             (watering_screen_element_list[CURR_ZONE_INDICATOR].element_ptr.button_ptr->width)
#define CURR_ZONE_RIGHT_X           (CURR_ZONE_X + CURR_ZONE_WIDTH)

#define ZONEX_TEXT_SIZE         1
#define ZONEX_X                 (CURR_ZONE_RIGHT_X + SCREEN_MARGIN * 3)
#define ZONEX_LINES_INIT_Y      (NAV_BAR_HEIGHT + SCREEN_MARGIN * 2)
#define ZONEX_LINES_SPACE       9
#define ZONEX_LINES_TEMPLATE    "Zone %1d  %2d/%2d Minutes"

#define SWITCH_ZONE_BUTTON_COLOR    LCD_GREEN
#define SWITCH_ZONE_BUTTON_X        (MIN_SCREEN_X + SCREEN_MARGIN * 1)
#define SWITCH_ZONE_BUTTON_Y        (MAX_SCREEN_Y - SCREEN_MARGIN * 1 - switch_zone_button_ptr->height)

static void generateNavBarButtons();
static void generateZonexLines();
static void generateCurrZoneIndicator();
static void generateSwitchZoneButton();
void drawWateringScreen();
void updateZonexLineFinishedMinutes(WATERING_SCREEN_ELEMENT_INDEX_t element_index, uint8_t minutes);
void updateZonexLineTargetMinutes(WATERING_SCREEN_ELEMENT_INDEX_t element_index, uint8_t minutes);
void updateCurrZoneIndicator(WATERING_SCREEN_ELEMENT_INDEX_t element_index);

static WATERING_SCREEN_ELEMENT_INDEX_t detectPressedElement_watering(uint16_t x, uint16_t y);
void wateringScreenPressed(uint16_t x, uint16_t y);

#endif
