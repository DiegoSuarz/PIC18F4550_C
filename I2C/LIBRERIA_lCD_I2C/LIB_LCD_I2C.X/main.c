/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 20 de marzo de 2022, 08:42 PM
 */


#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "config.h"
#include "I2C_LCD_Lib.h"

#define DIR_LCD 0x4E
#define DIR_LCD2 0x40


void main(void) {
    
    
    ADCON1=0x0F;
    
    I2C_Lcd_Init(DIR_LCD);
    I2C_Lcd_XY(DIR_LCD,4,3);
    I2C_Lcd_Char(DIR_LCD,'a');
    __delay_ms(1000);
    I2C_Lcd_Cmd(DIR_LCD,12); //12-> quita el cursor
    I2C_Lcd_XY(DIR_LCD,1,3);
    I2C_Lcd_String(DIR_LCD,"hola mundo");
    
    I2C_Lcd_Init(DIR_LCD2);
    I2C_Lcd_XY(DIR_LCD2,2,1);
    I2C_Lcd_Char(DIR_LCD2,'h');
    __delay_ms(1000);
    I2C_Lcd_Cmd(DIR_LCD2,_CURSOR_OFF); //12-> quita el cursor
    I2C_Lcd_XY(DIR_LCD2,3,5);
    I2C_Lcd_String(DIR_LCD2,"probando");
    
    
    while(true){

    }
    return;
}