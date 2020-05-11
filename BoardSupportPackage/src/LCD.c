/*
 * LCDLib.c
 *
 *  Created on: Mar 2, 2017
 *      Author: Danny
 */

#include "LCD.h"
#include "msp.h"
#include "driverlib.h"
#include "AsciiLib.h"

/************************************  Private Functions  *******************************************/

/*
 * Delay x ms
 */
static void Delay(unsigned long interval)
{
    while(interval > 0)
    {
        __delay_cycles(48000);
        interval--;
    }
}

/*******************************************************************************
 * Function Name  : LCD_initSPI
 * Description    : Configures LCD Control lines
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
static void LCD_initSPI()
{
    /* P10.1 - CLK
     * P10.2 - MOSI
     * P10.3 - MISO
     * P10.4 - LCD CS 
     * P10.5 - TP CS 
     */
    //enter reset mode
    UCB3CTLW0 = UCSWRST;
    //The inactive state is high
    UCB3CTLW0 |= UCCKPL;
    //MSB first
    UCB3CTLW0 |= UCMSB;
    //master mode
    UCB3CTLW0 |= UCMST;
    //Synchronous mode
    UCB3CTLW0 |= UCSYNC;
    //SMCLK
    UCB3CTLW0 |= UCSSEL__SMCLK;
    //configure pins
    P10SEL0|=BIT1;
    P10SEL1&=(~BIT1);
    P10SEL0|=BIT2;
    P10SEL1&=(~BIT2);
    P10SEL0|=BIT3;
    P10SEL1&=(~BIT3);
    BITBAND_PERI(P10->OUT, 4)=1;
    P10->DIR|=BIT4;
    BITBAND_PERI(P10->OUT, 5)=1;
    P10->DIR|=BIT5;
    //exit software reset
    UCB3CTLW0 &= (~UCSWRST);
}

/*******************************************************************************
 * Function Name  : LCD_reset
 * Description    : Resets LCD
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention      : Uses P10.0 for reset
 *******************************************************************************/
static void LCD_reset()
{
    P10DIR |= BIT0;
    P10OUT |= BIT0;  // high
    Delay(100);
    P10OUT &= ~BIT0; // low
    Delay(100);
    P10OUT |= BIT0;  // high
}

/************************************  Private Functions  *******************************************/


/************************************  Public Functions  *******************************************/

/*******************************************************************************
 * Function Name  : LCD_Configure_Brightness_CTRL
 * Description    : Configure CTRL Display Value (WRITE_CTRL_DISPLAY_VALUE)
 * Input          : BCTRL, DD, BL
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void LCD_Configure_Brightness_CTRL(bool BCTRL, bool DD, bool BL){
    uint16_t control_word=0;
    if(BCTRL){
        control_word |= BCTRL_BIT;
    }
    if(DD){
        control_word |= DD_BIT;
    }
    if(BL){
        control_word |= BL_BIT;
    }
    LCD_WriteReg(WRITE_CTRL_DISPLAY_VALUE, control_word);
}

/*******************************************************************************
 * Function Name  : LCD_Change_Brightness
 * Description    : Change the brightness of LCD
 * Input          : brightness (0 to 255)
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void LCD_Change_Brightness(uint8_t brightness){
    LCD_WriteReg(WRITE_DISPLAY_BRIGHTNESS_VALUE, brightness);
}

/*******************************************************************************
 * Function Name  : LCD_DrawRectangle
 * Description    : Draw a rectangle as the specified color
 * Input          : xStart, xEnd, yStart, yEnd, Color
 * Output         : None
 * Return         : None
 * Attention      : Must draw from left to right, top to bottom!
 *******************************************************************************/
void LCD_DrawRectangle(int16_t xStart, int16_t xEnd, int16_t yStart, int16_t yEnd, uint16_t Color)
{
    // Optimization complexity: O(64 + 2N) Bytes Written 

    /* Check special cases for out of bounds */
    if(     xStart<MIN_SCREEN_X ||
            xEnd>=MAX_SCREEN_X  ||
            xStart>xEnd         ||
            yStart<MIN_SCREEN_Y ||
            yEnd>=MAX_SCREEN_Y  ||
            yStart>yEnd         ){
        return;
    }
    /* Set window area for high-speed RAM write */
    LCD_WriteReg(HOR_ADDR_START_POS, yStart);     /* Horizontal GRAM Start Address */
    LCD_WriteReg(HOR_ADDR_END_POS, yEnd);  /* Horizontal GRAM End Address */
    LCD_WriteReg(VERT_ADDR_START_POS, xStart);    /* Vertical GRAM Start Address */
    LCD_WriteReg(VERT_ADDR_END_POS, xEnd); /* Vertical GRAM Start Address */
    /* Set cursor */ 
    LCD_SetCursor(xStart, yStart);
    /* Set index to GRAM */ 
    LCD_WriteIndex(GRAM);
    /* Send out data only to the entire area */ 
    int i, j;
    SPI_CS_LOW;
    LCD_Write_Data_Start();
    for(i=yStart; i<=yEnd; i++){
        for(j=xStart; j<=xEnd; j++){
            LCD_Write_Data_Only(Color);

//            if(i==yStart || i==yEnd || j==xStart || j==xEnd){
//                LCD_Write_Data_Only(Color);
//            }
//            else{
//                LCD_Write_Data_Only(LCD_WHITE);
//            }
        }
    }
    SPI_CS_HIGH;
    /* Set area back to span the entire LCD */
    LCD_WriteReg(HOR_ADDR_START_POS, 0x0000);     /* Horizontal GRAM Start Address */
    LCD_WriteReg(HOR_ADDR_END_POS, (MAX_SCREEN_Y - 1));  /* Horizontal GRAM End Address */
    LCD_WriteReg(VERT_ADDR_START_POS, 0x0000);    /* Vertical GRAM Start Address */
    LCD_WriteReg(VERT_ADDR_END_POS, (MAX_SCREEN_X - 1)); /* Vertical GRAM Start Address */
}

void LCD_DrawRectangle_edge(int16_t xStart, int16_t xEnd, int16_t yStart, int16_t yEnd, uint16_t fillColor, uint16_t edgeColor)
{
    // Optimization complexity: O(64 + 2N) Bytes Written

    /* Check special cases for out of bounds */
    if(     xStart<MIN_SCREEN_X ||
            xEnd>=MAX_SCREEN_X  ||
            xStart>xEnd         ||
            yStart<MIN_SCREEN_Y ||
            yEnd>=MAX_SCREEN_Y  ||
            yStart>yEnd         ){
        return;
    }
    /* Set window area for high-speed RAM write */
    LCD_WriteReg(HOR_ADDR_START_POS, yStart);     /* Horizontal GRAM Start Address */
    LCD_WriteReg(HOR_ADDR_END_POS, yEnd);  /* Horizontal GRAM End Address */
    LCD_WriteReg(VERT_ADDR_START_POS, xStart);    /* Vertical GRAM Start Address */
    LCD_WriteReg(VERT_ADDR_END_POS, xEnd); /* Vertical GRAM Start Address */
    /* Set cursor */
    LCD_SetCursor(xStart, yStart);
    /* Set index to GRAM */
    LCD_WriteIndex(GRAM);
    /* Send out data only to the entire area */
    int i, j;
    SPI_CS_LOW;
    LCD_Write_Data_Start();
    for(i=yStart; i<=yEnd; i++){
        for(j=xStart; j<=xEnd; j++){
            if(i==yStart || i==yEnd || j==xStart || j==xEnd){
                LCD_Write_Data_Only(edgeColor);
            }
            else{
                LCD_Write_Data_Only(fillColor);
            }
        }
    }
    SPI_CS_HIGH;
    /* Set area back to span the entire LCD */
    LCD_WriteReg(HOR_ADDR_START_POS, 0x0000);     /* Horizontal GRAM Start Address */
    LCD_WriteReg(HOR_ADDR_END_POS, (MAX_SCREEN_Y - 1));  /* Horizontal GRAM End Address */
    LCD_WriteReg(VERT_ADDR_START_POS, 0x0000);    /* Vertical GRAM Start Address */
    LCD_WriteReg(VERT_ADDR_END_POS, (MAX_SCREEN_X - 1)); /* Vertical GRAM Start Address */
}

/******************************************************************************
 * Function Name  : PutChar
 * Description    : Lcd screen displays a character
 * Input          : - Xpos: Horizontal coordinate
 *                  - Ypos: Vertical coordinate
 *                  - ASCI: Displayed character
 *                  - charColor: Character color
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor)
{
    uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);  /* get font data */
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
            {
                LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* Character color */
            }
        }
    }
}

inline void PutChar_size( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint8_t size)
{
    uint16_t i, j, k, l;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);  /* get font data */

    uint16_t adjusted_height = 16 * size;
    uint16_t adjusted_width = 8 * size;

    for( i=0; i<adjusted_height; i+=size )
    {
        tmp_char = buffer[i/size];
        for( j=0; j<adjusted_width; j+=size )
        {
            if( (tmp_char >> 7 - j / size) & 0x01 == 0x01 )
            {
//                LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* Character color */
//                LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* Character color */
                for( k=0; k<size; k++ ){
                    for (l=0; l<size; l++ ){
                        LCD_SetPoint( Xpos + j + k, Ypos + i + l, charColor );
                    }
                }
            }
        }
    }
}

/******************************************************************************
 * Function Name  : GUI_Text
 * Description    : Displays the string
 * Input          : - Xpos: Horizontal coordinate
 *                  - Ypos: Vertical coordinate
 *                  - str: Displayed string
 *                  - charColor: Character color
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void LCD_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str, uint16_t Color)
{
    uint8_t TempChar;

    /* Set area back to span the entire LCD */
    LCD_WriteReg(HOR_ADDR_START_POS, 0x0000);     /* Horizontal GRAM Start Address */
    LCD_WriteReg(HOR_ADDR_END_POS, (MAX_SCREEN_Y - 1));  /* Horizontal GRAM End Address */
    LCD_WriteReg(VERT_ADDR_START_POS, 0x0000);    /* Vertical GRAM Start Address */
    LCD_WriteReg(VERT_ADDR_END_POS, (MAX_SCREEN_X - 1)); /* Vertical GRAM Start Address */
    do
    {
        TempChar = *str++;
        PutChar( Xpos, Ypos, TempChar, Color);
        if( Xpos < MAX_SCREEN_X - 8)
        {
            Xpos += 8;
        }
        else if ( Ypos < MAX_SCREEN_X - 16)
        {
            Xpos = 0;
            Ypos += 16;
        }
        else
        {
            Xpos = 0;
            Ypos = 0;
        }
    }
    while ( *str != 0 );
}

void LCD_Text_size(uint16_t Xpos, uint16_t Ypos, uint8_t *str, uint16_t Color, uint8_t size)
{
    uint8_t TempChar;

    uint16_t adjusted_height = 16 * size;
    uint16_t adjusted_width = 8 * size;

    /* Set area back to span the entire LCD */
    LCD_WriteReg(HOR_ADDR_START_POS, 0x0000);     /* Horizontal GRAM Start Address */
    LCD_WriteReg(HOR_ADDR_END_POS, (MAX_SCREEN_Y - 1));  /* Horizontal GRAM End Address */
    LCD_WriteReg(VERT_ADDR_START_POS, 0x0000);    /* Vertical GRAM Start Address */
    LCD_WriteReg(VERT_ADDR_END_POS, (MAX_SCREEN_X - 1)); /* Vertical GRAM Start Address */
    do
    {
        TempChar = *str++;
        PutChar_size( Xpos, Ypos, TempChar, Color, size);
        if( Xpos < MAX_SCREEN_X - adjusted_width)
        {
            Xpos += adjusted_width;
        }
        else if ( Ypos < MAX_SCREEN_X - adjusted_height)
        {
            Xpos = 0;
            Ypos += adjusted_height;
        }
        else
        {
            Xpos = 0;
            Ypos = 0;
        }
    }
    while ( *str != 0 );
}


/*******************************************************************************
 * Function Name  : LCD_Clear
 * Description    : Fill the screen as the specified color
 * Input          : - Color: Screen Color
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void LCD_Clear(uint16_t Color)
{
    /* Set area back to span the entire LCD */
    LCD_WriteReg(HOR_ADDR_START_POS, 0x0000);     /* Horizontal GRAM Start Address */
    LCD_WriteReg(HOR_ADDR_END_POS, (MAX_SCREEN_Y - 1));  /* Horizontal GRAM End Address */
    LCD_WriteReg(VERT_ADDR_START_POS, 0x0000);    /* Vertical GRAM Start Address */
    LCD_WriteReg(VERT_ADDR_END_POS, (MAX_SCREEN_X - 1)); /* Vertical GRAM Start Address */
    /* Set cursor to (0,0) */ 
    LCD_SetCursor(0, 0);
    /* Set write index to GRAM */     
    LCD_WriteIndex(GRAM);
    /* Start data transmittion */ 
    uint16_t i, j;
    SPI_CS_LOW;
    LCD_Write_Data_Start();
    for(i=0; i<MAX_SCREEN_X; i++){
        for(j=0; j<MAX_SCREEN_Y; j++){
            LCD_Write_Data_Only(Color);
        }
    }
    SPI_CS_HIGH;
    // You'll need to call LCD_Write_Data_Start() and then send out only data to fill entire screen with color 
}

/******************************************************************************
 * Function Name  : LCD_SetPoint
 * Description    : Drawn at a specified point coordinates
 * Input          : - Xpos: Row Coordinate
 *                  - Ypos: Line Coordinate
 * Output         : None
 * Return         : None
 * Attention      : 18N Bytes Written
 *******************************************************************************/
void LCD_SetPoint(uint16_t Xpos, uint16_t Ypos, uint16_t color)
{
    /* Should check for out of bounds */ 
    if(Xpos>=MAX_SCREEN_X || Xpos<MIN_SCREEN_X || Ypos>=MAX_SCREEN_Y || Ypos<MIN_SCREEN_Y){
        return;
    }
    /* Set cursor to Xpos and Ypos */ 
    LCD_SetCursor(Xpos, Ypos);
    /* Write color to GRAM reg */ 
    LCD_WriteReg(GRAM, color);
}

uint16_t LCD_GetPoint(uint16_t Xpos, uint16_t Ypos){
    /* Should check for out of bounds */
    if(Xpos>=MAX_SCREEN_X || Xpos<MIN_SCREEN_X || Ypos>=MAX_SCREEN_Y || Ypos<MIN_SCREEN_Y){
        return;
    }
    /* Set cursor to Xpos and Ypos */
    LCD_SetCursor(Xpos, Ypos);
    /* Read color from GRAM reg */
    uint16_t color=LCD_ReadReg(GRAM);
    return color;
}

/*******************************************************************************
 * Function Name  : LCD_Write_Data_Only
 * Description    : Data writing to the LCD controller
 * Input          : - data: data to be written
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void LCD_Write_Data_Only(uint16_t data)
{
    /* Send out MSB */ 
    SPISendRecvByte((data >>   8));
    /* Send out LSB */ 
    SPISendRecvByte((data & 0xFF));
}

/*******************************************************************************
 * Function Name  : LCD_WriteData
 * Description    : LCD write register data
 * Input          : - data: register data
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void LCD_WriteData(uint16_t data)
{
    SPI_CS_LOW;

    SPISendRecvByte(SPI_START | SPI_WR | SPI_DATA);    /* Write : RS = 1, RW = 0       */
    SPISendRecvByte((data >>   8));                    /* Write D8..D15                */
    SPISendRecvByte((data & 0xFF));                    /* Write D0..D7                 */

    SPI_CS_HIGH;
}

/*******************************************************************************
 * Function Name  : LCD_WriteReg
 * Description    : Reads the selected LCD Register.
 * Input          : None
 * Output         : None
 * Return         : LCD Register Value.
 * Attention      : None
 *******************************************************************************/
inline uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
    /* Write 16-bit Index */
    LCD_WriteIndex(LCD_Reg);
    /* Return 16-bit Reg using LCD_ReadData() */
    if(LCD_Reg==GRAM){
        return LCD_ReadData(1);
    }
    else{
        return LCD_ReadData(0);
    }

}

/*******************************************************************************
 * Function Name  : LCD_WriteIndex
 * Description    : LCD write register address
 * Input          : - index: register address
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void LCD_WriteIndex(uint16_t index)
{
    SPI_CS_LOW;

    /* SPI write data */
    SPISendRecvByte(SPI_START | SPI_WR | SPI_INDEX);   /* Write : RS = 0, RW = 0  */
    SPISendRecvByte(0);
    SPISendRecvByte(index);

    SPI_CS_HIGH;
}

/*******************************************************************************
 * Function Name  : SPISendRecvByte
 * Description    : Send one byte then receive one byte of response
 * Input          : uint8_t: byte
 * Output         : None
 * Return         : Recieved value 
 * Attention      : None
 *******************************************************************************/
inline uint8_t SPISendRecvByte (uint8_t byte)
{
    while((UCB3STATW & UCBUSY) !=0);
    /* Send byte of data */
    UCB3TXBUF=byte;
    /* Wait as long as busy */ 
    while((UCB3STATW & UCBUSY) !=0);
    //while(SPI_isBusy(EUSCI_B3_BASE)==EUSCI_SPI_BUSY);
    /* Return received value*/
    uint8_t recv=UCB3RXBUF;
    return recv;
}

/*******************************************************************************
 * Function Name  : LCD_Write_Data_Start
 * Description    : Start of data writing to the LCD controller
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void LCD_Write_Data_Start(void)
{
    SPISendRecvByte(SPI_START | SPI_WR | SPI_DATA);    /* Write : RS = 1, RW = 0 */
}

/*******************************************************************************
 * Function Name  : LCD_ReadData
 * Description    : LCD read data
 * Input          : None
 * Output         : None
 * Return         : return data
 * Attention      : Diagram (d) in datasheet
 *******************************************************************************/
inline uint16_t LCD_ReadData(bool readGRAM)
{
    uint16_t value;
    SPI_CS_LOW;

    SPISendRecvByte(SPI_START | SPI_RD | SPI_DATA);   /* Read: RS = 1, RW = 1   */
    SPISendRecvByte(0);                               /* Dummy read 1           */
    //if reading GRAM, need four more dummy bytes (total of 5)
    if(readGRAM){
        SPISendRecvByte(0);
        SPISendRecvByte(0);
        SPISendRecvByte(0);
        SPISendRecvByte(0);
    }
    value = (SPISendRecvByte(0) << 8);                /* Read D8..D15           */
    value |= SPISendRecvByte(0);                      /* Read D0..D7            */

    SPI_CS_HIGH;
    return value;
}

/*******************************************************************************
 * Function Name  : LCD_WriteReg
 * Description    : Writes to the selected LCD register.
 * Input          : - LCD_Reg: address of the selected register.
 *                  - LCD_RegValue: value to write to the selected register.
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue)
{
    /* Write 16-bit Index */
    LCD_WriteIndex(LCD_Reg);
    /* Write 16-bit Reg Data */
    LCD_WriteData(LCD_RegValue);
}

/*******************************************************************************
 * Function Name  : LCD_SetCursor
 * Description    : Sets the cursor position.
 * Input          : - Xpos: specifies the X position.
 *                  - Ypos: specifies the Y position.
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos )
{
    /* Should just be two LCD_WriteReg to appropriate registers */ 

    /* Set horizonal GRAM coordinate (Ypos) */ 
    LCD_WriteReg(GRAM_HORIZONTAL_ADDRESS_SET, Ypos);
    /* Set vertical GRAM coordinate (Xpos) */
    LCD_WriteReg(GRAM_VERTICAL_ADDRESS_SET, Xpos);
}

/*******************************************************************************
 * Function Name  : LCD_Init
 * Description    : Configures LCD Control lines, sets whole screen black
 * Input          : bool usingTP: determines whether or not to enable TP interrupt 
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void LCD_Init(bool usingTP)
{
    LCD_initSPI();

    if (usingTP)
    {
        /* Configure low true interrupt on P4.0 for TP */ 
        P4->DIR &= ~BIT0;
        P4->IFG &= ~BIT0;// P4.0 IFG cleared
        P4->IE |= BIT0; // Enable interrupt on P4.0
        P4->IES |= BIT0; // high-to-low transition
        P4->REN |= BIT0; // Pull-up resister
        P4->OUT |= BIT0; // Sets res to pull-up
        //NVIC_EnableIRQ(PORT4_IRQn);
    }

    LCD_reset();

    LCD_WriteReg(0xE5, 0x78F0); /* set SRAM internal timing */
    LCD_WriteReg(DRIVER_OUTPUT_CONTROL, 0x0100); /* set Driver Output Control */
    LCD_WriteReg(DRIVING_WAVE_CONTROL, 0x0700); /* set 1 line inversion */
    LCD_WriteReg(ENTRY_MODE, 0x1038); /* set GRAM write direction and BGR=1 */
    LCD_WriteReg(RESIZING_CONTROL, 0x0000); /* Resize register */
    LCD_WriteReg(DISPLAY_CONTROL_2, 0x0207); /* set the back porch and front porch */
    LCD_WriteReg(DISPLAY_CONTROL_3, 0x0000); /* set non-display area refresh cycle ISC[3:0] */
    LCD_WriteReg(DISPLAY_CONTROL_4, 0x0000); /* FMARK function */
    LCD_WriteReg(RGB_DISPLAY_INTERFACE_CONTROL_1, 0x0000); /* RGB interface setting */
    LCD_WriteReg(FRAME_MARKER_POSITION, 0x0000); /* Frame marker Position */
    LCD_WriteReg(RGB_DISPLAY_INTERFACE_CONTROL_2, 0x0000); /* RGB interface polarity */

    /* Power On sequence */
    LCD_WriteReg(POWER_CONTROL_1, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    LCD_WriteReg(POWER_CONTROL_2, 0x0007); /* DC1[2:0], DC0[2:0], VC[2:0] */
    LCD_WriteReg(POWER_CONTROL_3, 0x0000); /* VREG1OUT voltage */
    LCD_WriteReg(POWER_CONTROL_4, 0x0000); /* VDV[4:0] for VCOM amplitude */
    LCD_WriteReg(DISPLAY_CONTROL_1, 0x0001);
    Delay(200);

    /* Dis-charge capacitor power voltage */
    LCD_WriteReg(POWER_CONTROL_1, 0x1090); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    LCD_WriteReg(POWER_CONTROL_2, 0x0227); /* Set DC1[2:0], DC0[2:0], VC[2:0] */
    Delay(50); /* Delay 50ms */
    LCD_WriteReg(POWER_CONTROL_3, 0x001F);
    Delay(50); /* Delay 50ms */
    LCD_WriteReg(POWER_CONTROL_4, 0x1500); /* VDV[4:0] for VCOM amplitude */
    LCD_WriteReg(POWER_CONTROL_7, 0x0027); /* 04 VCM[5:0] for VCOMH */
    LCD_WriteReg(FRAME_RATE_AND_COLOR_CONTROL, 0x000D); /* Set Frame Rate */
    Delay(50); /* Delay 50ms */
    LCD_WriteReg(GRAM_HORIZONTAL_ADDRESS_SET, 0x0000); /* GRAM horizontal Address */
    LCD_WriteReg(GRAM_VERTICAL_ADDRESS_SET, 0x0000); /* GRAM Vertical Address */

    /* Adjust the Gamma Curve */
    LCD_WriteReg(GAMMA_CONTROL_1,    0x0000);
    LCD_WriteReg(GAMMA_CONTROL_2,    0x0707);
    LCD_WriteReg(GAMMA_CONTROL_3,    0x0307);
    LCD_WriteReg(GAMMA_CONTROL_4,    0x0200);
    LCD_WriteReg(GAMMA_CONTROL_5,    0x0008);
    LCD_WriteReg(GAMMA_CONTROL_6,    0x0004);
    LCD_WriteReg(GAMMA_CONTROL_7,    0x0000);
    LCD_WriteReg(GAMMA_CONTROL_8,    0x0707);
    LCD_WriteReg(GAMMA_CONTROL_9,    0x0002);
    LCD_WriteReg(GAMMA_CONTROL_10,   0x1D04);

    /* Set GRAM area */
    LCD_WriteReg(HOR_ADDR_START_POS, 0x0000);     /* Horizontal GRAM Start Address */
    LCD_WriteReg(HOR_ADDR_END_POS, (MAX_SCREEN_Y - 1));  /* Horizontal GRAM End Address */
    LCD_WriteReg(VERT_ADDR_START_POS, 0x0000);    /* Vertical GRAM Start Address */
    LCD_WriteReg(VERT_ADDR_END_POS, (MAX_SCREEN_X - 1)); /* Vertical GRAM Start Address */
    LCD_WriteReg(GATE_SCAN_CONTROL_0X60, 0x2700); /* Gate Scan Line */
    LCD_WriteReg(GATE_SCAN_CONTROL_0X61, 0x0001); /* NDL,VLE, REV */
    LCD_WriteReg(GATE_SCAN_CONTROL_0X6A, 0x0000); /* set scrolling line */

    /* Partial Display Control */
    LCD_WriteReg(PART_IMAGE_1_DISPLAY_POS, 0x0000);
    LCD_WriteReg(PART_IMG_1_START_END_ADDR_0x81, 0x0000);
    LCD_WriteReg(PART_IMG_1_START_END_ADDR_0x82, 0x0000);
    LCD_WriteReg(PART_IMAGE_2_DISPLAY_POS, 0x0000);
    LCD_WriteReg(PART_IMG_2_START_END_ADDR_0x84, 0x0000);
    LCD_WriteReg(PART_IMG_2_START_END_ADDR_0x85, 0x0000);

    /* Panel Control */
    LCD_WriteReg(PANEL_ITERFACE_CONTROL_1, 0x0010);
    LCD_WriteReg(PANEL_ITERFACE_CONTROL_2, 0x0600);
    LCD_WriteReg(DISPLAY_CONTROL_1, 0x0133); /* 262K color and display ON */
    Delay(50); /* delay 50 ms */

    LCD_Clear(LCD_BLACK);
}

/*******************************************************************************
 * Function Name  : TP_ReadXY
 * Description    : Obtain X and Y touch coordinates
 * Input          : None
 * Output         : None
 * Return         : Pointer to "Point" structure
 * Attention      : None
 *******************************************************************************/
Point TP_ReadXY()
{
    Point pt;
    uint32_t high=0;
    uint32_t low=0;

    //enter reset mode
    UCB3CTLW0 |= UCSWRST;
    UCB3BRW=5;
    //exit software reset
    UCB3CTLW0 &= (~UCSWRST);


    //enable TP
    SPI_CS_TP_LOW;

    /* Read X coord. */ 

    SPISendRecvByte(CHX);
    high=SPISendRecvByte(0x00);
    low=SPISendRecvByte(0x00);
    //disable TP
    SPI_CS_TP_HIGH;
    high=(high<<5)|(low>>3);
    if(high>MAX_TP_X){
        high=MAX_TP_X;
    }
    if(high<MIN_TP_X){
        high=MIN_TP_X;
    }
    high=high-MIN_TP_X;
    high=high*MAX_SCREEN_X;
    high=high/x_diff;
    pt.x=high;



    //enable TP
    SPI_CS_TP_LOW;

    /* Read Y coord. */ 
    SPISendRecvByte(CHY);
    high=SPISendRecvByte(0x00);
    low=SPISendRecvByte(0x00);
    //disable TP
    SPI_CS_TP_HIGH;
    high=(high<<5)|(low>>3);
    if(high>MAX_TP_Y){
        high=MAX_TP_Y;
    }
    if(high<MIN_TP_Y){
        high=MIN_TP_Y;
    }
    high=high-MIN_TP_Y;
    high=high*MAX_SCREEN_Y;
    high=high/y_diff;
    pt.y=high;

//    //enable TP
//    SPI_CS_TP_LOW;
//    SPISendRecvByte(DISABLE_INT);
//    high=SPISendRecvByte(0x00);
//    low=SPISendRecvByte(0x00);
//    //disable TP
//    SPI_CS_TP_HIGH;

    //enter reset mode
    UCB3CTLW0 |= UCSWRST;
    UCB3BRW=0;
    //exit software reset
    UCB3CTLW0 &= (~UCSWRST);

    /* Return point  */
    return pt;
}

/************************************  Public Functions  *******************************************/

