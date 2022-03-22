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
#include "I2C_Lib.h"

#define DIR_LCD 0x4E
#define DIR_SLAVE2 0x40

void WritePCF(uint8_t Dir, uint8_t dato){
    I2C_Start();
    I2C_Write_Data(Dir);
    I2C_Write_Data(dato);
    I2C_Stop();
}

uint8_t ReadPCF(uint8_t Dir){

    uint8_t Direcc_R, dato;
    
    Direcc_R = (uint8_t)((Dir&0xFE)|0x01);
    I2C_Start();
    I2C_Write_Data(Direcc_R);
    dato = I2C_Read_Slave();
    I2C_Stop();
    return dato;
}

void cmd_LCD(char dir, char cmd);
void dato_LCD(uint8_t dir, uint8_t cmd);
void write_data(uint8_t dir, uint8_t dato);

void I2C_Lcd_Init(uint8_t dir);
void I2C_Lcd_Char(uint8_t dir, uint8_t letra);
void I2C_Lcd_Cmd(uint8_t dir, uint8_t cmd);
void I2C_Lcd_XY(uint8_t dir,uint8_t y, uint8_t x);
void I2C_Lcd_String(uint8_t dir, char *text);

void main(void) {
    
    
    ADCON1=0x0F;
    
    I2C_Init_Master(_I2C_STANDART_SPEED,_I2C_INT_OFF);
    
    I2C_Lcd_Init(DIR_LCD);
    I2C_Lcd_XY(DIR_LCD,1,3);
    I2C_Lcd_Char(DIR_LCD,'a');
    __delay_ms(1000);
    I2C_Lcd_Cmd(DIR_LCD,12); //12-> quita el cursor
    I2C_Lcd_XY(DIR_LCD,2,3);
    I2C_Lcd_String(DIR_LCD,"hola mundo");
    while(true){

    }
    return;
}

void cmd_LCD(char dir, char cmd){  //funcion para enviar comandos
    uint8_t dato;
    WritePCF(dir,0);    //limpiar el puerto del PCF
    __delay_us(100);
    dato = cmd & 0xF0;
    write_data(dir,dato);
    dato=(uint8_t)((cmd&0x0F)<<4);
    write_data(dir,dato);
}

void dato_LCD(uint8_t dir, uint8_t cmd){ //funcion para enviar datos
    uint8_t dato;
    WritePCF(dir,0);    //limpiar el puerto del PCF
    __delay_us(100);
    dato = (cmd & 0xF0)|0b00000001;
    write_data(dir,dato);
    dato=(uint8_t)(((cmd&0x0F)<<4)|0b00000001);
    write_data(dir,dato);
}

void write_data(uint8_t dir, uint8_t dato){
    WritePCF(dir,dato|0b00000010);
    __delay_ms(5);
    WritePCF(dir,dato|0b00000000);
    __delay_ms(5);
}

void I2C_Lcd_Init(uint8_t dir){
    __delay_ms(15);
    cmd_LCD(dir,0x39);
    __delay_ms(5);
    cmd_LCD(dir,0x30);
    __delay_us(100);
    cmd_LCD(dir,0x32);
    __delay_us(100);
    
    //configuramos la LCD
    __delay_us(100);
    cmd_LCD(dir,0x2B);
    __delay_us(100);
    cmd_LCD(dir,0x08);
    __delay_us(100);
    cmd_LCD(dir,0x0F);
    __delay_us(100);
    cmd_LCD(dir,0x01);
    __delay_us(100);
    cmd_LCD(dir,0x06);
    __delay_us(100);
    cmd_LCD(dir,0x0F);
    __delay_us(100);
    
    
}

void I2C_Lcd_Char(uint8_t dir, uint8_t letra){
    dato_LCD(dir,letra);
    WritePCF(dir,0);
}

void I2C_Lcd_Cmd(uint8_t dir, uint8_t cmd){
    cmd_LCD(dir,cmd);
    WritePCF(dir,0);
}


void I2C_Lcd_XY(uint8_t dir,uint8_t y, uint8_t x){
    
    switch(y){
        case 1: I2C_Lcd_Cmd(dir, 0x80+(x-1)); break;
        case 2: I2C_Lcd_Cmd(dir, 0xC0+(x-1));break;
        case 3: I2C_Lcd_Cmd(dir, 0x94+(x-1));break;
        case 4: I2C_Lcd_Cmd(dir, 0xD4+(x-1));break;
    }
    __delay_ms(100);
    WritePCF(dir,0);
}

void I2C_Lcd_String(uint8_t dir, char *text){
    while(*text != '\0'){
        I2C_Lcd_Char(dir,*text);
        text++;
    }
    
}