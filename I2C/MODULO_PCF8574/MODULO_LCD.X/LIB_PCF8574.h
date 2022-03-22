
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> 

//configuarr la velocidad del bus I2C
#define _I2C_STANDART_SPEED 100000
#define _I2C_HIGH_SPEED 4000000

//DEFINIR LOS PINES DEL PCF
#define P0  0b00000001
#define P1  0b00000010
#define P2  0b00000100
#define P3  0b00001000
#define P4  0b00010000
#define P5  0b00100000
#define P6  0b01000000
#define P7  0b10000000
 
//CONEXIONES Y UTILIDADES DEL PCF
#define DIR_PCF     7
#define DIR_PCF_INT 0x40
#define LED_LCD     P3
#define LCD_RS      P0
#define LCD_RW      P1
#define LCD_E       P2
#define LCD_D4      P4
#define LCD_D5      P5
#define LCD_D6      P6
#define LCD_D7      P7


//comandos de la LCD
#define _FIRST_ROW          128
#define _SECOND_ROW         129
#define _THIRD_ROW          148
#define _FORUTH_ROW         212
#define _CLEAR              1
#define _RETURN_HOME        2
#define _CURSOR_OFF         12
#define _UNDERLINE_ON       14
#define _BLINK_CURSOR_ON    15
#define _MOVE_CURSOR_LEFT   16
#define _MOVE_CURSOR_RIGHT  20
#define _TURN_OFF           0
#define _TURN_ON            8
#define _SHIFT_LEFT         24
#define _SHIFT_RIGHT        28

//prototipos:
void I2C_Init_Master(unsigned long speed_master);
void I2C_Start(void);
void I2C_Write_Data(unsigned char dato);
void I2C_Stop(void);
void I2C_Idle(void);  //modo inactivo
void I2C_Restart(void);

//prototipos para controlar el PCF8574
void WritePCF(uint8_t dato);

void dato_LCD(uint8_t ctrl,uint8_t cmd);
void trama_LCD(uint8_t ctrl, uint8_t cmd);
void I2C_LCD_Init(void);
void I2C_LCD_Cmd(uint8_t cmd);
void I2C_LCD_Char(uint8_t letra);
void I2C_LCD_XY(uint8_t y, uint8_t x);
void I2C_LCD_String(char *text);

#endif	/* XC_HEADER_TEMPLATE_H */

