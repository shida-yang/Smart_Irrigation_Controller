/*
 * LCDLib.h
 *
 *  Created on: Mar 2, 2017
 *      Author: Danny
 */

#ifndef LCDLIB_H_
#define LCDLIB_H_

#include <stdbool.h>
#include <stdint.h>
/************************************ Defines *******************************************/


/* Custom Defines */
#define SHIFT_AMOUNT 8
#define MAX_ADC_VAL 4095

/* TP EDGE */
#define MAX_TP_X 3844
#define MAX_TP_Y 3822
#define MIN_TP_X 348
#define MIN_TP_Y 264
static uint32_t x_diff=MAX_TP_X-MIN_TP_X;
static uint32_t y_diff=MAX_TP_Y-MIN_TP_Y;

/* Screen size */
#define MAX_SCREEN_X     320
#define MAX_SCREEN_Y     240
#define MIN_SCREEN_X     0
#define MIN_SCREEN_Y     0
#define SCREEN_SIZE      76800

/* Register details */
#define SPI_START   (0x70)     /* Start byte for SPI transfer        */
#define SPI_RD      (0x01)     /* WR bit 1 within start              */
#define SPI_WR      (0x00)     /* WR bit 0 within start              */
#define SPI_DATA    (0x02)     /* RS bit 1 within start byte         */
#define SPI_INDEX   (0x00)     /* RS bit 0 within start byte         */

/* CS LCD*/
#define SPI_CS_LOW P10OUT &= ~BIT4
#define SPI_CS_HIGH P10OUT |= BIT4

/* CS Touchpanel */
#define SPI_CS_TP_LOW P10OUT &= ~BIT5
#define SPI_CS_TP_HIGH P10OUT |= BIT5

/* XPT2046 registers definition for X and Y coordinate retrieval */
#define CHX         0x90
#define CHY         0xD0
#define DISABLE_INT 0x81

/* LCD colors */
#define LCD_WHITE          0xFFFF
#define LCD_BLACK          0x0000
#define LCD_BLUE           0x0197
#define LCD_RED            0xF800
#define LCD_MAGENTA        0xF81F
#define LCD_GREEN          0x07E0
#define LCD_CYAN           0x7FFF
#define LCD_YELLOW         0xFFE0
#define LCD_GRAY           0x2104
#define LCD_LIGHT_GRAY     (LCD_GRAY<<2)
#define LCD_PURPLE         0xF11F
#define LCD_ORANGE         0xFD20
#define LCD_PINK           0xfdba
#define LCD_OLIVE          0xdfe4

/* ILI 9325 registers definition */
#define READ_ID_CODE                        0x00
#define DRIVER_OUTPUT_CONTROL               0x01
#define DRIVING_WAVE_CONTROL                0x02
#define ENTRY_MODE                          0x03
#define RESIZING_CONTROL                    0x04
#define DISPLAY_CONTROL_1                   0x07
#define DISPLAY_CONTROL_2                   0x08
#define DISPLAY_CONTROL_3                   0x09
#define DISPLAY_CONTROL_4                   0x0A
#define RGB_DISPLAY_INTERFACE_CONTROL_1     0x0C
#define FRAME_MARKER_POSITION               0x0D
#define RGB_DISPLAY_INTERFACE_CONTROL_2     0x0F
#define POWER_CONTROL_1                     0x10
#define POWER_CONTROL_2                     0x11
#define POWER_CONTROL_3                     0x12
#define POWER_CONTROL_4                     0x13
#define GRAM_HORIZONTAL_ADDRESS_SET         0x20
#define GRAM_VERTICAL_ADDRESS_SET           0x21
#define DATA_IN_GRAM                        0x22
#define POWER_CONTROL_7                     0x29
#define FRAME_RATE_AND_COLOR_CONTROL        0x2B

#define GAMMA_CONTROL_1                     0x30
#define GAMMA_CONTROL_2                     0x31
#define GAMMA_CONTROL_3                     0x32
#define GAMMA_CONTROL_4                     0x35
#define GAMMA_CONTROL_5                     0x36
#define GAMMA_CONTROL_6                     0x37
#define GAMMA_CONTROL_7                     0x38
#define GAMMA_CONTROL_8                     0x39
#define GAMMA_CONTROL_9                     0x3C
#define GAMMA_CONTROL_10                    0x3D

#define HOR_ADDR_START_POS                  0x50
#define HOR_ADDR_END_POS                    0x51
#define VERT_ADDR_START_POS                 0x52
#define VERT_ADDR_END_POS                   0x53
#define GATE_SCAN_CONTROL_0X60              0x60
#define GATE_SCAN_CONTROL_0X61              0x61
#define GATE_SCAN_CONTROL_0X6A              0x6A
#define PART_IMAGE_1_DISPLAY_POS            0x80
#define PART_IMG_1_START_END_ADDR_0x81      0x81
#define PART_IMG_1_START_END_ADDR_0x82      0x81
#define PART_IMAGE_2_DISPLAY_POS            0x83
#define PART_IMG_2_START_END_ADDR_0x84      0x84
#define PART_IMG_2_START_END_ADDR_0x85      0x85
#define PANEL_ITERFACE_CONTROL_1            0x90
#define PANEL_ITERFACE_CONTROL_2            0x92
#define PANEL_ITERFACE_CONTROL_4            0x95

#define GRAM                                0x22
#define HORIZONTAL_GRAM_SET                 0x20
#define VERTICAL_GRAM_SET                   0x21

#define WRITE_DISPLAY_BRIGHTNESS_VALUE      0xB1
#define WRITE_CTRL_DISPLAY_VALUE            0xB3
/* WRITE_CTRL_DISPLAY_VALUE bits */
#define BCTRL_BIT                           BIT5    // Brightness Control Block On/Off
#define DD_BIT                              BIT3    // Display Dimming Control
#define BL_BIT                              BIT2    // Backlight Control On/Off

/************************************ Defines *******************************************/

/********************************** Structures ******************************************/
typedef struct Point {
    uint16_t x;
    uint16_t y;
}Point;
/********************************** Structures ******************************************/

/************************************ Public Functions  *******************************************/

/*******************************************************************************
 * Function Name  : LCD_Configure_Brightness_CTRL
 * Description    : Configure CTRL Display Value (WRITE_CTRL_DISPLAY_VALUE)
 * Input          : BCTRL, DD, BL
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void LCD_Configure_Brightness_CTRL(bool BCTRL, bool DD, bool BL);

/*******************************************************************************
 * Function Name  : LCD_Change_Brightness
 * Description    : Change the brightness of LCD
 * Input          : brightness (0 to 255)
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void LCD_Change_Brightness(uint8_t brightness);

/*******************************************************************************
 * Function Name  : LCD_DrawRectangle
 * Description    : Draw a rectangle as the specified color
 * Input          : xStart, xEnd, yStart, yEnd, Color
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void LCD_DrawRectangle(int16_t xStart, int16_t xEnd, int16_t yStart, int16_t yEnd, uint16_t Color);
void LCD_DrawRectangle_edge(int16_t xStart, int16_t xEnd, int16_t yStart, int16_t yEnd, uint16_t fillColor, uint16_t edgeColor);

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
inline void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor);
inline void PutChar_size( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint8_t size);

/******************************************************************************
* Function Name  : LCD_Text
* Description    : Displays the string
* Input          : - Xpos: Horizontal coordinate
*                  - Ypos: Vertical coordinate
*                  - str: Displayed string
*                  - charColor: Character color
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color);
void LCD_Text_size(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint8_t size);

/*******************************************************************************
* Function Name  : LCD_Write_Data_Only
* Description    : Data writing to the LCD controller
* Input          : - data: data to be written
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
inline void LCD_Write_Data_Only(uint16_t data);

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : Fill the screen as the specified color
* Input          : - Color: Screen Color
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_Clear(uint16_t Color);

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : Drawn at a specified point coordinates
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate
* Output         : None
* Return         : None
* Attention      : 18N Bytes Written
*******************************************************************************/
void LCD_SetPoint(uint16_t Xpos, uint16_t Ypos, uint16_t color);
uint16_t LCD_GetPoint(uint16_t Xpos, uint16_t Ypos);

/*******************************************************************************
* Function Name  : LCD_WriteData
* Description    : LCD write register data
* Input          : - data: register data
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
inline void LCD_WriteData(uint16_t data);

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention      : None
*******************************************************************************/
inline uint16_t LCD_ReadReg(uint16_t LCD_reg);

/*******************************************************************************
* Function Name  : LCD_WriteIndex
* Description    : LCD write register address
* Input          : - index: register address
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
inline void LCD_WriteIndex(uint16_t index);

/*******************************************************************************
 * Function Name  : SPISendRecvTPByte
 * Description    : Send one byte then receive one byte of response from Touchpanel
 * Input          : uint8_t: byte
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline uint8_t SPISendRecvTPByte (uint8_t byte);

/*******************************************************************************
* Function Name  : SPISendRecvByte
* Description    : Send one byte then recv one byte of response
* Input          : uint8_t: byte
* Output         : None
* Return         : Recieved value 
* Attention      : None
*******************************************************************************/
inline uint8_t SPISendRecvByte(uint8_t byte);

/*******************************************************************************
* Function Name  : LCD_Write_Data_Start
* Description    : Start of data writing to the LCD controller
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
inline void LCD_Write_Data_Start(void);

/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : LCD read data
* Input          : None
* Output         : None
* Return         : return data
* Attention  : None
*******************************************************************************/
inline uint16_t LCD_ReadData(bool readGRAM);

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
inline void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
inline void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos );

/*******************************************************************************
* Function Name  : LCD_Init
* Description    : Configures LCD Control lines, sets whole screen black
* Input          : bool usingTP: determines whether or not to enable TP interrupt
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_Init(bool usingTP);

/*******************************************************************************
 * Function Name  : TP_ReadXY
 * Description    : Obtain X and Y touch coordinates
 * Input          : None
 * Output         : None
 * Return         : Point structure
 * Attention      : None
 *******************************************************************************/
Point TP_ReadXY();

/*******************************************************************************
 * Function Name  : TP_ReadX
 * Description    : Obtain X touch coordinate
 * Input          : None
 * Output         : None
 * Return         : X Coordinate
 * Attention      : None
 *******************************************************************************/
uint16_t TP_ReadX();

/*******************************************************************************
 * Function Name  : TP_ReadY
 * Description    : Obtain Y touch coordinate
 * Input          : None
 * Output         : None
 * Return         : Y Coordinate
 * Attention      : None
 *******************************************************************************/
uint16_t TP_ReadY();

/************************************ Public Functions  *******************************************/




#endif /* LCDLIB_H_ */
