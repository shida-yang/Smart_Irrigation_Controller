#include "msp.h"
#include "BSP.h"
#include "weatherServer.h"
#include "Time_Server.h"
#include "GUI.h"
#include "G8RTOS.h"

semaphore_t SPI_MUTEX;

void PORT4_IRQHandler(){
    if(P4->IV==2){
        P4->IE &= (~BIT0);
        P4->IFG &= (~BIT0);
        BGT_WaitForTap();
    }
}

void BGT_WaitForTap(){
    DelayMs(50);
    if((P4->IN & BIT0)==0){
        //read coordinate
        Point p;
        p=TP_ReadXY();

//        mainScreenPressed(p.x, p.y);
//        settingScreenPressed(p.x, p.y);
//        planScreenPressed(p.x, p.y);
//        wateringScreenPressed(p.x, p.y);
        scheduleScreenPressed(p.x, p.y);
    }
    P4->IFG &= (~BIT0);
    P4->IE |= BIT0;
}

void main(void)
{
    BSP_InitBoard();

    NVIC_EnableIRQ(PORT4_IRQn);

    G8RTOS_InitSemaphore(&SPI_MUTEX, 1);

//    fillInWeekWeatherData();
//
//    drawMainScreen();
//
//    updateRainSumTartget(5.87951, 91.56482);
//
//    struct tm ts;
//
//    while(1){
//        getCurrentTime(&ts);
//        P4->IE &= (~BIT0);
//        updateDate(ts.tm_mon+1, ts.tm_mday, ts.tm_year%100, ts.tm_wday);
//        updateTime(ts.tm_hour, ts.tm_min, ts.tm_sec);
//        P4->IE |= BIT0;
//        DelayMs(4000);
//    }



//    drawSettingScreen();

//    drawPlanScreen();

//    drawWateringScreen();

    drawScheduleScreen();

    while(1);
}
