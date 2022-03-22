
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef I2C_LCD_Lib_H
#define	I2C_LCD_Lib_H


#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "config.h"

#define DIR_LCD 0x4E   
#define DIR_SLAVE2 0x40

#define _I2C_STANDART_SPEED 100000
#define _I2C_HIGH_SPEED 4000000

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





/**********************************************************************************/
void I2C_Init_Master(unsigned long speed_master);
void I2C_Start(void);
void I2C_Write_Data(unsigned char dato);
void I2C_Stop(void);
void I2C_Idle(void);  //modo inactivo
void I2C_Restart(void);

/**********************************************************/

/**********************************************************************************/
//prototipos para controlar el PCF8574
void WritePCF(uint8_t Dir, uint8_t dato);


void cmd_LCD(char dir, char cmd);
void dato_LCD(uint8_t dir, uint8_t cmd);
void write_data(uint8_t dir, uint8_t dato);

//prototipos para controlar el LCD
void I2C_Lcd_Init(uint8_t dir);
void I2C_Lcd_Char(uint8_t dir, uint8_t letra);
void I2C_Lcd_Cmd(uint8_t dir, uint8_t cmd);
void I2C_Lcd_XY(uint8_t dir,uint8_t y, uint8_t x);
void I2C_Lcd_String(uint8_t dir, char *text);


/**********************************************************************************/

#endif	/* XC_HEADER_TEMPLATE_H */

