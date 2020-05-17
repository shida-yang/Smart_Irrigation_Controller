#include "msp.h"
#include "BSP.h"
#include "weatherServer.h"
#include "Time_Server.h"
#include "GUI.h"
//#include "Screen_Template.h"

void main(void)
{
    BSP_InitBoard();



//    drawMainScreen();
//
//    updateRainSumTartget(5.87951, 91.56482);
//
//    struct tm ts;
//
//    while(1){
//        getCurrentTime(&ts);
//        updateDate(ts.tm_mon+1, ts.tm_mday, ts.tm_year%100, ts.tm_wday);
//        updateTime(ts.tm_hour, ts.tm_min, ts.tm_sec);
//        DelayMs(4000);
//    }

    drawSettingScreen();
    updateTimeHour(16);
    updateTimeMinute(8);
    updateTimeSecond(0);
    updateBrightness(3);
    updateTimeout(0);
    while(1);
}
