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
#include "LIB_PCF8574.h"




void main(void) {
    
    
   ADCON1=0x0F;
    
   I2C_LCD_Init();
   __delay_ms(1500);
   I2C_LCD_Cmd(_CURSOR_OFF);
   I2C_LCD_Char('H');
   I2C_LCD_Char('o');
   I2C_LCD_Char('l');
   I2C_LCD_Char('a');
   
   I2C_LCD_XY(3,5);
   I2C_LCD_Char('s');
   I2C_LCD_Char('a');
   I2C_LCD_Char('l');
   I2C_LCD_Char('u');
   I2C_LCD_Char('d');
   I2C_LCD_Char('o');
   I2C_LCD_Char('s');
   
   I2C_LCD_XY(4,1);
   I2C_LCD_String("programando");
    while(true){

    }
    return;
}