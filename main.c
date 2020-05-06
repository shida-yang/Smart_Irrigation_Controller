#include "msp.h"
#include "BSP.h"
#include "weatherServer.h"

/**
 * main.c
 */
void main(void)
{
    BSP_InitBoard();
    connectToWeatherServer();
    char* ptr1=getRawHistoricalWeatherJSON(1588611600);
    jsmntok_t* tokenList=0;
    uint16_t len=parseRawWeatherJSON(ptr1, &tokenList);
    free(tokenList);
    while(1);
}
