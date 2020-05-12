#include "msp.h"
#include "BSP.h"
#include "weatherServer.h"
#include "GUI.h"


void main(void)
{
    BSP_InitBoard();

    connectToWeatherServer();
//    char* ptr1=getRawHistoricalWeatherJSON(1588611600);
//    jsmntok_t* tokenList=0;
//    uint16_t len=parseRawWeatherJSON(ptr1, &tokenList);
//    free(tokenList);

//    fillInWeekWeatherData();

    LCD_Clear(LCD_WHITE);
    LCD_Text_size(0, 0, "15:33:26", LCD_BLUE, 4);

    while(1);
}
