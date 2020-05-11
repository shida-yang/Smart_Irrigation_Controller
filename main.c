#include "msp.h"
#include "BSP.h"
#include "weatherServer.h"
#include "GUI.h"

/**
 * main.c
 */

volatile uint8_t brightness=30;

button_t bt3={
              135,
              70,
              50,
              20,
              LCD_YELLOW,
              "0"
};

button_t bt1={
             240,
             150,
             50,
             20,
             LCD_GREEN,
             "+"
};

button_t bt2={
             80,
             150,
             50,
             20,
             LCD_RED,
             "-"
};

uint8_t count=0;

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

        // inc
        if(buttonTouched(&bt1, p.x, p.y)){
            count++;
        }

        // dec
        if(buttonTouched(&bt2, p.x, p.y)){
            count--;
        }

        sprintf(bt3.text, "%d", count);

        drawButton(&bt3);
    }
    P4->IFG &= (~BIT0);
    P4->IE |= BIT0;
}


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
//    LCD_Text_size(0, 0, "15:33:26", LCD_BLUE, 3);



    drawButton(&bt1);



    drawButton(&bt2);

    drawButton(&bt3);

    NVIC_EnableIRQ(PORT4_IRQn);

    while(1);
}
