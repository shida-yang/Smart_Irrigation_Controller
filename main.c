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
    char* ptr1=getRawForecastWeatherJSON();
    while(1);
}
