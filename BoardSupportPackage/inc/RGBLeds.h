#include <driverlib.h>
#include "BSP.h"

//slave addr
#define BLUE    0x60
#define GREEN   0x61
#define RED     0x62

//reg addr
#define PSC0    0x02
#define PWM0    0x03
#define PSC1    0x04
#define PWM1    0x05
#define LS0     0x06
#define LS1     0x07
#define LS2     0x08
#define LS3     0x09
#define AI      0x10

//led states
#define OFF     0
#define ON      1
#define DIM0    2
#define DIM1    3

void initLEDI2C();
void sendI2C(uint8_t slaveAddr, uint8_t regAddr, uint8_t* data, uint16_t len);

void initRGBLeds();
void setColor(uint32_t unit, uint32_t PWM_DATA);
void setMode(uint32_t unit, uint16_t LED_DATA);
void setMode1(uint32_t unit, uint32_t LED_DATA);
void setBrightness(uint32_t unit, uint32_t BRG_DATA);
