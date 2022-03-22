/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 8 de junio de 2020, 07:45 PM
 */


#include <xc.h>
#include "config.h"
#include "I2C_Lib.h"


#define SLAVE_ADDRESS_LCD 0x4E

void lcd_send_data (unsigned char data)
{
	unsigned char data_l, data_u;
	data_l = (data<<4)&0xf0;  //select lower nibble by moving it to the upper nibble position
	data_u = data&0xf0;  //select upper nibble

	I2C_Start(SLAVE_ADDRESS_LCD);
	
	I2C_Write_Data(data_u|0x0D);  //enable=1 and rs =1
	I2C_Write_Data(data_u|0x09);  //enable=0 and rs =1

	I2C_Write_Data (data_l|0x0D);  //enable =1 and rs =1
	I2C_Write_Data (data_l|0x09);  //enable=0 and rs =1

	I2C_Stop();
}

// send command to LCD

void lcd_send_cmd (unsigned char data)
{
	unsigned char data_l, data_u;
	data_l = (data<<4)&0xf0;  //select lower nibble by moving it to the upper nibble position
	data_u = data&0xf0;  //select upper nibble

	I2C_Start(SLAVE_ADDRESS_LCD);
	
	I2C_Write_Data(data_u|0x0C);  //enable=1 and rs =0
	I2C_Write_Data(data_u|0x08);  //enable=0 and rs =0

	I2C_Write_Data(data_l|0x0C);  //enable =1 and rs =0
	I2C_Write_Data(data_l|0x08);  //enable=0 and rs =0

	I2C_Stop();
}

lcd_clear (void)
{
    lcd_send_cmd (0x80);
    for (int i=0;i<60;i++)
    {
        lcd_send_data (' ');
    }
    lcd_send_cmd (0xc0);
    for (int i=0;i<60;i++)
    {
        lcd_send_data (' ');
    }
    
}

// initialise LCD

void lcd_init (void)
{
	lcd_send_cmd (0x02);
	lcd_send_cmd (0x28);
	lcd_send_cmd (0x0c);
	lcd_send_cmd (0x80);
}

// send string to LCD

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}


void main(void) {
    
    // initialise I2C at 100 KHz
    
    I2C_Init_Master(100000);  
    
    // initialise LCD
    
    lcd_init ();
    
    // send string to LCD
    
    lcd_send_string ("HOLA");
    
    lcd_send_cmd (0xc0);  // second row
    
    lcd_send_string ("PIC18F4550");
    
    while (1)
    {
        
    }
    
    
    return;
}
