/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 24 de marzo de 2022, 12:28 PM
 */


#include <xc.h>
#include "config.h"
#include "I2C_Lib.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "lib_lcd_20x4.h"

#define DIR_RTC 0xD0

//definir formato de salida:
//#define FORMATO_24h 
//#define FORMATO_12h_PM
#define FORMATO_12h_AM

enum{  //DIRECCION DE REGISTROS DEL RTC DS1307
    reg_Segundos =0x00,
    reg_min,
    reg_horas,
    reg_dates,
    reg_dias,
    reg_meses,
    reg_years
}registrosRTC;

typedef struct { //DATOS DEL RTC
    uint8_t segundos;
    uint8_t minutos;
    uint8_t horas;
    uint8_t dias;
    uint8_t dates;
    uint8_t meses;
    uint8_t years;
    
}datoRtc;

datoRtc data={  56, //segundos
                59, //minutos
                7,  //horas
                15, //dias de la semana
                3,  //dias
                5,  //mes
                30  //año xxnn
             };


uint8_t bin_bcd_convert(uint8_t dato){
    uint8_t data;
    data=(uint8_t)((dato/10)<<4)|(dato%10);
    return data;
}

char aux[20]=" ";


void ConfiRegRTC(uint8_t dir,uint8_t dato, uint8_t reg){
    uint8_t data;
    
    data=bin_bcd_convert(dato);
    if(reg==reg_horas){
        Lcd_XY(1,17);
        Lcd_Out("  ");
        
#ifdef  FORMATO_12h_AM
        data|=0b01000000;
        Lcd_XY(1,17);
        Lcd_Out("AM");
#endif
        
#ifdef  FORMATO_12h_PM     
        data|=0b01100000;
        Lcd_XY(1,17);
        Lcd_Out("PM");
#endif
    }
    I2C_Start();
    I2C_Write_Data(dir); //enviando la direccion del rtc
    I2C_Write_Data(reg);
    I2C_Write_Data(data);
    I2C_Stop();
}

uint8_t bcd_bin_convert(uint8_t dato){
    dato = ((dato & 0xF0)>>4)*10 + (dato&0x0F);
    return dato;
}

uint8_t getDataDs1307(uint8_t dato){
    uint8_t data;
    I2C_Start();
    I2C_Write_Data(0xD0);
    I2C_Write_Data(dato);
    I2C_Restart();
    I2C_Write_Data(0xD1);
    data = I2C_Read_Slave();
    I2C_N_ACK();
    I2C_Stop();
    
    data = bcd_bin_convert(data);
    return data;
}

void readDs1307(void){
    Lcd_XY(1,1);
    Lcd_Out("TIME: ");
    
    Lcd_Char((data.horas/10)+48);
    Lcd_Char((data.horas%10)+48);
    Lcd_Char(':');
    Lcd_Char((data.minutos/10)+48);
    Lcd_Char((data.minutos%10)+48);
    Lcd_Char(':');
    Lcd_Char((data.segundos/10)+48);
    Lcd_Char((data.segundos%10)+48);
    
    Lcd_XY(2,1);
    Lcd_Out("FECHA: ");
    Lcd_Char((data.dias/10)+48);
    Lcd_Char((data.dias%10)+48);
    Lcd_Char('/');
    Lcd_Char((data.meses/10)+48);
    Lcd_Char((data.meses%10)+48);
    Lcd_Char('/');
    Lcd_Char((data.years/10)+48);
    Lcd_Char((data.years%10)+48);
    
}

void readDs1307_sprintf(void){
    Lcd_XY(1,1);
    sprintf(aux,"TIEMPO: %02d:%02d:%02d",data.horas,data.minutos,data.segundos);
    Lcd_Out(aux);
    Lcd_XY(2,1);
    sprintf(aux,"FECHA: %02d/%02d/%02d",data.dias,data.meses,data.years);
    Lcd_Out(aux);
    
}

int main(void) {
    
    
    ADCON1=0x0F;
    
    Lcd_Init();
    Lcd_Cmd(_LCD_CURSOR_OFF);
    I2C_Init_Master(_I2C_STANDART_SPEED,_I2C_INT_OFF);
        
    ConfiRegRTC(DIR_RTC,data.segundos,reg_Segundos);
    ConfiRegRTC(DIR_RTC,data.minutos,reg_min);
    ConfiRegRTC(DIR_RTC,data.horas,reg_horas);
    ConfiRegRTC(DIR_RTC,data.dates,reg_dates);
    ConfiRegRTC(DIR_RTC,data.dias,reg_dias);
    ConfiRegRTC(DIR_RTC,data.meses,reg_meses);
    ConfiRegRTC(DIR_RTC,data.years,reg_years);
    
    while(1){
        data.segundos=getDataDs1307(reg_Segundos);
        data.minutos=getDataDs1307(reg_min);
        data.horas=getDataDs1307(reg_horas);
        data.dates=getDataDs1307(reg_dates);
        data.dias=getDataDs1307(reg_dias);
        data.meses=getDataDs1307(reg_meses);
        data.years=getDataDs1307(reg_years);
         
        readDs1307_sprintf();  //metodo sprintf
       
        //readDs1307();  //metodo convetir a asii (+48)
        __delay_ms(100);
    }
    return 0;
}
