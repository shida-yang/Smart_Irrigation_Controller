#include "msp.h"
#include "BSP.h"
#include "weatherServer.h"
#include "Time_Server.h"
#include "GUI.h"
//#include "Screen_Template.h"

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

        settingScreenPressed(p.x, p.y);
    }
    P4->IFG &= (~BIT0);
    P4->IE |= BIT0;
}

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

    NVIC_EnableIRQ(PORT4_IRQn);

    drawSettingScreen();


    while(1);
}
