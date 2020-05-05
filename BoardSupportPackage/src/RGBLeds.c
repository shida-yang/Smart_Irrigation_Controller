#include "RGBLeds.h"

//volatile eUSCI_I2C_MasterConfig i2cConfig1 =
//{
//        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
//        12000000,
//        EUSCI_B_I2C_SET_DATA_RATE_400KBPS,      // Desired I2C Clock of 400khz
//        0,                                      // No byte counter threshold
//        EUSCI_B_I2C_NO_AUTO_STOP                // no Autostop
//};

void initLEDI2C(){

    P3->DIR|=BIT7;
    int i;
    for(i=0; i<10; i++){
        BITBAND_PERI(P3->OUT, 7)=~BITBAND_PERI(P3->OUT, 7);
        DelayMs(1);
        BITBAND_PERI(P3->OUT, 7)=~BITBAND_PERI(P3->OUT, 7);
        DelayMs(1);
    }

    //software reset
    UCB2CTLW0=UCSWRST;
    //master, I2C, clock sync, smclk source, transmitter
    UCB2CTLW0|= (1<<11) |   //master BIT11
                (3<<9)  |   //I2C
                (1<<8)  |   //clock sync
                (3<<6)  |   //SMCLK
                (1<<4);     //transmitter
    //set Fclk as 400kHz
    //presumes: SMCLK is the source, FSMCLK is 12MHz
    UCB2BRW=30;
    //P3.6=UCB2_SDA
    //P3.7=UCB2_SLC
    uint16_t temp=(1<<6) | (1<<7);
    P3SEL0 |= temp;
    temp=(~(1<<6)) & (~(1<<7));
    P3SEL1 &= temp;
    //exit software reset
    UCB2CTLW0 &= ~UCSWRST;
    //__delay_cycles(30000);

//    I2C_initMaster(EUSCI_B2_BASE, &i2cConfig1);
//        //P3.6=UCB2_SDA
//        //P3.7=UCB2_SLC
//        uint16_t temp=(1<<6) | (1<<7);
//        P3SEL0 |= temp;
//        temp=(~(1<<6)) & (~(1<<7));
//        P3SEL1 &= temp;
//    I2C_setMode(EUSCI_B2_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
//    I2C_enableModule(EUSCI_B2_BASE);
}

void sendI2C(uint8_t slaveAddr, uint8_t regAddr, uint8_t* data, uint16_t len){
    while(UCB2CTLW0&(1<<2)==1);
    //set slave addr
    UCB2I2CSA=slaveAddr;
    //generate start condition
    UCB2CTLW0|=(1<<1);
    __delay_cycles(1000);
    //check if the start condition is sent
    while((UCB2CTLW0&(1<<1)));
    while(!(UCB2IFG&(1<<1)));
    //send reg addr
    UCB2TXBUF=regAddr;
    //check if the buffer is available
    while(!(UCB2IFG&(1<<1)));
    int i=0;
    for(i=0; i<len; i++){
        //send data
        UCB2TXBUF=data[i];
        //check if the buffer is available
        while(!(UCB2IFG&(1<<1)));
    }
    //generate stop condition
    UCB2CTLW0|=(1<<2);

//    while(I2C_isBusBusy(EUSCI_B2_BASE)==EUSCI_B_I2C_BUS_BUSY);
//    I2C_setSlaveAddress(EUSCI_B2_BASE, slaveAddr);
//    I2C_masterSendMultiByteStart(EUSCI_B2_BASE, regAddr);
//    int i=0;
//    for(i=0; i<len; i++){
//        I2C_masterSendMultiByteNext(EUSCI_B2_BASE, data[i]);
//    }
//    I2C_masterSendMultiByteStop(EUSCI_B2_BASE);
//    while(I2C_masterIsStopSent(EUSCI_B2_BASE)==EUSCI_B_I2C_SENDING_STOP);
}

void initRGBLeds(){
    initLEDI2C();
    setMode(RED, 0);
    setMode(BLUE, 0);
    setMode(GREEN, 0);
}

void setColor(uint32_t unit, uint32_t PWM_DATA){
    uint8_t temp;
    uint8_t data_send[4];
    int i;
    for(i=0; i<4; i++){
        temp=((PWM_DATA>>(i*8))&0xFF);
        data_send[i]=temp;
    }
    sendI2C(unit, AI|PSC0, data_send, 4);
}


void setMode(uint32_t unit, uint16_t LED_DATA){
    uint32_t data=0;
    int i;
    for(i=0; i<16; i++){
        //ON
        if(LED_DATA&1){
            data|=(ON<<(i*2));
        }
        LED_DATA=LED_DATA>>1;
    }

    uint8_t data_send[4];
    uint8_t temp;

    for(i=0; i<4; i++){
        temp=((data>>(i*8))&0xFF);
        data_send[i]=temp;
    }

    sendI2C(unit, AI|LS0, data_send, 4);

}

void setMode1(uint32_t unit, uint32_t LED_DATA){
    int i;
    uint8_t data_send[4];
    uint8_t temp;
    for(i=0; i<4; i++){
        temp=((LED_DATA>>(i*8))&0xFF);
        data_send[i]=temp;
    }
    sendI2C(unit, AI|LS0, data_send, 4);
}
