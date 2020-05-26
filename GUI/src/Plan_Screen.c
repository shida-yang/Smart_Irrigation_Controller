#include "Plan_Screen.h"

static screen_element_t plan_screen_element_list[NUMBER_OF_PLAN_SCREEN_ELEMENTS];
static button_t plan_buttons[8];
static button_t nav_bar_buttons[3];
static button_t zone_activate_buttons[8][7];
static bool zone_active_state[8][7];
static text_element_t minute_numbers[8][7];
static char minute_number_str[8][7][4];
static uint8_t minute_number_number[8][7];
static button_t up_down_buttons[2];
static PLAN_SCREEN_ELEMENT_INDEX_t curr_plan = PLAN1_BUTTON;
static PLAN_SCREEN_ELEMENT_INDEX_t now_setting;

static void generateNavBarButtons(){
    plan_screen_element_list[PLAN_SAVE_BUTTON].type = BUTTON;
    plan_screen_element_list[PLAN_SAVE_BUTTON].element_ptr.button_ptr = &nav_bar_buttons[PLAN_SAVE_BUTTON];
    button_t* save_button_ptr = plan_screen_element_list[PLAN_SAVE_BUTTON].element_ptr.button_ptr;
    save_button_ptr->color = SAVE_BUTTON_COLOR;
    strcpy(save_button_ptr->text, "Save");
    adjustButtonSizeBasedOnText(save_button_ptr, NAV_BAR_BUTTON_PADDING);
    save_button_ptr->y_coordinate = SCREEN_MARGIN;
    save_button_ptr->x_coordinate = MAX_SCREEN_X - SCREEN_MARGIN - save_button_ptr->width;

    plan_screen_element_list[PLAN_DISCARD_BUTTON].type = BUTTON;
    plan_screen_element_list[PLAN_DISCARD_BUTTON].element_ptr.button_ptr = &nav_bar_buttons[PLAN_DISCARD_BUTTON];
    button_t* discard_button_ptr = plan_screen_element_list[PLAN_DISCARD_BUTTON].element_ptr.button_ptr;
    discard_button_ptr->color = DISCARD_BUTTON_COLOR;
    strcpy(discard_button_ptr->text, "Discard");
    adjustButtonSizeBasedOnText(discard_button_ptr, NAV_BAR_BUTTON_PADDING);
    discard_button_ptr->y_coordinate = SCREEN_MARGIN;
    discard_button_ptr->x_coordinate = save_button_ptr->x_coordinate - SCREEN_MARGIN - discard_button_ptr->width;

    plan_screen_element_list[PLAN_BACK_TO_MAIN_BUTTON].type = BUTTON;
    plan_screen_element_list[PLAN_BACK_TO_MAIN_BUTTON].element_ptr.button_ptr = &nav_bar_buttons[PLAN_BACK_TO_MAIN_BUTTON];
    button_t* back_to_main_button_ptr = plan_screen_element_list[PLAN_BACK_TO_MAIN_BUTTON].element_ptr.button_ptr;
    back_to_main_button_ptr->color = PLAN_BACK_TO_MAIN_BUTTON_COLOR;
    strcpy(back_to_main_button_ptr->text, "< Main");
    adjustButtonSizeBasedOnText(back_to_main_button_ptr, NAV_BAR_BUTTON_PADDING);
    back_to_main_button_ptr->y_coordinate = SCREEN_MARGIN;
    back_to_main_button_ptr->x_coordinate = MIN_SCREEN_X + SCREEN_MARGIN;
}

static void generatePlanButtons(){
    uint16_t current_x_coordinate = 4;
    for(int i = PLAN1_BUTTON; i <= PLAN8_BUTTON; i++){
        plan_screen_element_list[i].type = BUTTON;
        plan_screen_element_list[i].element_ptr.button_ptr = &plan_buttons[i - PLAN1_BUTTON];
        button_t* curr_plan_button_ptr = plan_screen_element_list[i].element_ptr.button_ptr;

        if(i == curr_plan){
            curr_plan_button_ptr->color = PLAN_NUMBER_BUTTON_ACTIVE_COLOR;
        }
        else{
            curr_plan_button_ptr->color = PLAN_NUMBER_BUTTON_DEACTIVE_COLOR;
        }

//        strcpy(curr_date_button_ptr->text, &weekdays_array[i]);
        sprintf(curr_plan_button_ptr->text, "P%1d", i - PLAN1_BUTTON + 1);

        curr_plan_button_ptr->width = PLAN_NUMBER_BUTTON_WIDTH;
        curr_plan_button_ptr->height = PLAN_NUMBER_BUTTON_HEIGHT;

        curr_plan_button_ptr->y_coordinate = PLAN_NUMBER_BUTTON_Y;
        curr_plan_button_ptr->x_coordinate = current_x_coordinate;

        current_x_coordinate += (PLAN_NUMBER_BUTTON_WIDTH + 4);
    }
}

static void generateZoneLines(){
    uint16_t curr_y = ZONE_INIT_Y;
    for(int i = ZOOM1_MINUTE; i <= ZOOM7_MINUTE; i++){
        char buf[50];
        sprintf(buf, "Zone %1d     Minutes", i - ZOOM1_MINUTE + 1);
        LCD_Text_size(ZONE_TEXT_X, curr_y, buf, LCD_BLACK, ZONE_TEXT_SIZE);

        plan_screen_element_list[i].type = TEXT;
        plan_screen_element_list[i].element_ptr.text_element_ptr = &minute_numbers[curr_plan - PLAN1_BUTTON][i - ZOOM1_MINUTE];

        plan_screen_element_list[i + MINUTE_TO_ACTIVE_BUTTON_INDEX_OFFSET].type = BUTTON;
        plan_screen_element_list[i + MINUTE_TO_ACTIVE_BUTTON_INDEX_OFFSET].element_ptr.button_ptr = &zone_activate_buttons[curr_plan - PLAN1_BUTTON][i - ZOOM1_MINUTE];

        for(int j = PLAN1_BUTTON; j <= PLAN8_BUTTON; j++){
            text_element_t* curr_te_ptr = &minute_numbers[j - PLAN1_BUTTON][i - ZOOM1_MINUTE];
            curr_te_ptr->size = ZONE_TEXT_SIZE;
            curr_te_ptr->color = LCD_BLACK;
            curr_te_ptr->text_ptr = &minute_number_str[j - PLAN1_BUTTON][i - ZOOM1_MINUTE];
            curr_te_ptr->x_coordinate = ZONE_MINUTE_NUMBER_X;
            curr_te_ptr->y_coordinate = curr_y;

            button_t* curr_button_ptr = &zone_activate_buttons[j - PLAN1_BUTTON][i - ZOOM1_MINUTE];

            if(zone_active_state[j - PLAN1_BUTTON][i - ZOOM1_MINUTE] == 1){
                curr_button_ptr->color = ZOOM_ACTIVE_BUTTON_COLOR;
                strcpy(curr_button_ptr->text, ZOOM_ACTIVE_BUTTON_STR);
            }
            else{
                curr_button_ptr->color = ZOOM_DEACTIVE_BUTTON_COLOR;
                strcpy(curr_button_ptr->text, ZOOM_DEACTIVE_BUTTON_STR);
            }
            adjustButtonSizeBasedOnText(curr_button_ptr, NAV_BAR_BUTTON_PADDING);
            curr_button_ptr->x_coordinate = ZONE_ACTIVE_BUTTON_X;
            curr_button_ptr->y_coordinate = curr_y - 3;
        }

        curr_y += (CHAR_HEIGHT * ZONE_TEXT_SIZE + 9);
    }
}

static void generateUpDownButtons(){
    plan_screen_element_list[PLAN_DOWN_BUTTON].type = BUTTON;
    plan_screen_element_list[PLAN_DOWN_BUTTON].element_ptr.button_ptr = &up_down_buttons[PLAN_DOWN_BUTTON - PLAN_DOWN_BUTTON];
    button_t* down_button_ptr = plan_screen_element_list[PLAN_DOWN_BUTTON].element_ptr.button_ptr;
    down_button_ptr->color = UP_DOWN_BUTTON_COLOR;
    strcpy(down_button_ptr->text, "DOWN");
    adjustButtonSizeBasedOnText(down_button_ptr, UP_DOWN_BUTTON_PADDING);
    down_button_ptr->y_coordinate = DOWN_BUTTON_Y;
    down_button_ptr->x_coordinate = DOWN_BUTTON_X;

    plan_screen_element_list[PLAN_UP_BUTTON].type = BUTTON;
    plan_screen_element_list[PLAN_UP_BUTTON].element_ptr.button_ptr = &up_down_buttons[PLAN_UP_BUTTON - PLAN_DOWN_BUTTON];
    button_t* up_button_ptr = plan_screen_element_list[PLAN_UP_BUTTON].element_ptr.button_ptr;
    up_button_ptr->color = UP_DOWN_BUTTON_COLOR;
    strcpy(up_button_ptr->text, " UP ");
    adjustButtonSizeBasedOnText(up_button_ptr, UP_DOWN_BUTTON_PADDING);
    up_button_ptr->y_coordinate = UP_BUTTON_Y;
    up_button_ptr->x_coordinate = UP_BUTTON_X;
}

void drawPlanScreen(){
    now_setting = -1;
    LCD_Clear(LCD_WHITE);
    generateNavBarButtons();
    drawNavBarLine(LCD_BLACK);
    generatePlanButtons();
    generateZoneLines();
    generateUpDownButtons();
    drawAllElements(plan_screen_element_list, NUMBER_OF_PLAN_SCREEN_ELEMENTS);
    // TODO - update values based on current values (GET FROM OUTSIDE WORLD)
    for(int i = ZOOM1_MINUTE; i <= ZOOM7_MINUTE; i++){
        for(int j = PLAN1_BUTTON; j <= PLAN8_BUTTON; j++){
            minute_number_number[j - PLAN1_BUTTON][i - ZOOM1_MINUTE] = (i - ZOOM1_MINUTE) + (j - PLAN1_BUTTON) * 7;
            zone_active_state[j - PLAN1_BUTTON][i - ZOOM1_MINUTE] = (j - PLAN1_BUTTON) & 1;
        }
    }

    // update screen based on values
    for(int i = ZOOM1_MINUTE; i <= ZOOM7_MINUTE; i++){
        updateMinuteNumber(i, minute_number_number[curr_plan - PLAN1_BUTTON][i - ZOOM1_MINUTE]);
    }

    for(int i = ZOOM1_MINUTE_ACTIVE_BUTTON; i <= ZOOM7_MINUTE_ACTIVE_BUTTON; i++){
        updateZoneActivateButtons(i, zone_active_state[curr_plan - PLAN1_BUTTON][i - ZOOM1_MINUTE_ACTIVE_BUTTON]);
    }
}

static void updateMinuteNumber(PLAN_SCREEN_ELEMENT_INDEX_t element_index, uint8_t value){
    if((int8_t)value > 99){
        value = 0;
    }
    if((int8_t)value < 0){
        value = 99;
    }
    eraseSingleElement(&plan_screen_element_list[element_index], PLAN_SCREEN_BACKGROUND_COLOR);
    minute_number_number[curr_plan - PLAN1_BUTTON][element_index - ZOOM1_MINUTE] = value;
    sprintf(minute_number_str[curr_plan - PLAN1_BUTTON][element_index - ZOOM1_MINUTE], "%2d", minute_number_number[curr_plan - PLAN1_BUTTON][element_index - ZOOM1_MINUTE]);
    drawSingleElement(&plan_screen_element_list[element_index]);
}

static void updateZoneActivateButtons(PLAN_SCREEN_ELEMENT_INDEX_t element_index, bool is_active){
    button_t* button_ptr = plan_screen_element_list[element_index].element_ptr.button_ptr;
    if(is_active){
        button_ptr->color = ZOOM_ACTIVE_BUTTON_COLOR;
        strcpy(button_ptr->text, ZOOM_ACTIVE_BUTTON_STR);
    }
    else{
        button_ptr->color = ZOOM_DEACTIVE_BUTTON_COLOR;
        strcpy(button_ptr->text, ZOOM_DEACTIVE_BUTTON_STR);
    }
    drawSingleElement(&plan_screen_element_list[element_index]);
}

static PLAN_SCREEN_ELEMENT_INDEX_t detectPressedElement_plan(uint16_t x, uint16_t y){
    for(int i = 0; i < NUMBER_OF_PLAN_SCREEN_ELEMENTS; i++){
        // Button
        if(plan_screen_element_list[i].type == BUTTON){
            button_t* button_ptr = plan_screen_element_list[i].element_ptr.button_ptr;
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
            text_element_t* text_element_ptr = plan_screen_element_list[i].element_ptr.text_element_ptr;
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

void planScreenPressed(uint16_t x, uint16_t y){
    PLAN_SCREEN_ELEMENT_INDEX_t element_index = detectPressedElement_plan(x, y);
    switch(element_index){
    case PLAN_BACK_TO_MAIN_BUTTON:
        break;
    case PLAN_DISCARD_BUTTON:
        break;
    case PLAN_SAVE_BUTTON:
        break;
    case PLAN1_BUTTON:
    case PLAN2_BUTTON:
    case PLAN3_BUTTON:
    case PLAN4_BUTTON:
    case PLAN5_BUTTON:
    case PLAN6_BUTTON:
    case PLAN7_BUTTON:
    case PLAN8_BUTTON:
        button_t* plan_button_ptr = plan_screen_element_list[curr_plan].element_ptr.button_ptr;
        plan_button_ptr->color = PLAN_NUMBER_BUTTON_DEACTIVE_COLOR;
        drawSingleElement(&plan_screen_element_list[curr_plan]);

        curr_plan = element_index;
        plan_button_ptr = plan_screen_element_list[curr_plan].element_ptr.button_ptr;
        plan_button_ptr->color = PLAN_NUMBER_BUTTON_ACTIVE_COLOR;
        drawSingleElement(&plan_screen_element_list[curr_plan]);

        for(int i = ZOOM1_MINUTE; i <= ZOOM7_MINUTE; i++){
            eraseSingleElement(&plan_screen_element_list[i], PLAN_SCREEN_BACKGROUND_COLOR);
            plan_screen_element_list[i].element_ptr.text_element_ptr = &minute_numbers[curr_plan - PLAN1_BUTTON][i - ZOOM1_MINUTE];
            updateMinuteNumber(i, minute_number_number[curr_plan - PLAN1_BUTTON][i - ZOOM1_MINUTE]);
        }

        for(int i = ZOOM1_MINUTE_ACTIVE_BUTTON; i <= ZOOM7_MINUTE_ACTIVE_BUTTON; i++){
            plan_screen_element_list[i].element_ptr.button_ptr = &zone_activate_buttons[curr_plan - PLAN1_BUTTON][i - ZOOM1_MINUTE_ACTIVE_BUTTON];
            updateZoneActivateButtons(i, zone_active_state[curr_plan - PLAN1_BUTTON][i - ZOOM1_MINUTE_ACTIVE_BUTTON]);
        }
        break;
    case ZOOM1_MINUTE:
    case ZOOM2_MINUTE:
    case ZOOM3_MINUTE:
    case ZOOM4_MINUTE:
    case ZOOM5_MINUTE:
    case ZOOM6_MINUTE:
    case ZOOM7_MINUTE:
        if(zone_active_state[curr_plan - PLAN1_BUTTON][element_index - ZOOM1_MINUTE] == 1){
            if((int8_t)now_setting >= 0){
                // Deactivate old element
                plan_screen_element_list[now_setting].element_ptr.text_element_ptr->color = NORMAL_TEXT;
                updateMinuteNumber(now_setting, minute_number_number[curr_plan - PLAN1_BUTTON][now_setting - ZOOM1_MINUTE]);
            }
            // Activate new element
            now_setting = element_index;
            plan_screen_element_list[now_setting].element_ptr.text_element_ptr->color = SELECTED_TEXT;
            updateMinuteNumber(now_setting, minute_number_number[curr_plan - PLAN1_BUTTON][now_setting - ZOOM1_MINUTE]);
        }
        break;
    case ZOOM1_MINUTE_ACTIVE_BUTTON:
    case ZOOM2_MINUTE_ACTIVE_BUTTON:
    case ZOOM3_MINUTE_ACTIVE_BUTTON:
    case ZOOM4_MINUTE_ACTIVE_BUTTON:
    case ZOOM5_MINUTE_ACTIVE_BUTTON:
    case ZOOM6_MINUTE_ACTIVE_BUTTON:
    case ZOOM7_MINUTE_ACTIVE_BUTTON:
        zone_active_state[curr_plan - PLAN1_BUTTON][element_index - ZOOM1_MINUTE_ACTIVE_BUTTON] = !zone_active_state[curr_plan - PLAN1_BUTTON][element_index - ZOOM1_MINUTE_ACTIVE_BUTTON];
        updateZoneActivateButtons(element_index, zone_active_state[curr_plan - PLAN1_BUTTON][element_index - ZOOM1_MINUTE_ACTIVE_BUTTON]);
        break;
    case PLAN_UP_BUTTON:
        if((int8_t)now_setting >= 0 && zone_active_state[curr_plan - PLAN1_BUTTON][now_setting - ZOOM1_MINUTE] == 1){
            minute_number_number[curr_plan - PLAN1_BUTTON][now_setting - ZOOM1_MINUTE]++;
            updateMinuteNumber(now_setting, minute_number_number[curr_plan - PLAN1_BUTTON][now_setting - ZOOM1_MINUTE]);
        }
        break;
    case PLAN_DOWN_BUTTON:
        if((int8_t)now_setting >= 0 && zone_active_state[curr_plan - PLAN1_BUTTON][now_setting - ZOOM1_MINUTE] == 1){
            minute_number_number[curr_plan - PLAN1_BUTTON][now_setting - ZOOM1_MINUTE]--;
            updateMinuteNumber(now_setting, minute_number_number[curr_plan - PLAN1_BUTTON][now_setting - ZOOM1_MINUTE]);
        }
        break;
    default:
        break;
    };
}
