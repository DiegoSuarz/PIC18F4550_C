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

#define DIR_SLAVE1 0x4E
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

void main(void) {
    
    uint8_t data_rec;
    ADCON1=0x0F;
    TRISD=0x00;
    LATD=0x00;
    I2C_Init_Master(_I2C_STANDART_SPEED,_I2C_INT_OFF);
    WritePCF(DIR_SLAVE2,0x05);
    
    while(true){
//        for(uint8_t i=0;i<=255;i++){
//            WritePCF(DIR_SLAVE2,i);
//            __delay_ms(300);
//        }
    data_rec = ReadPCF(DIR_SLAVE1);
    LATD = data_rec;
    WritePCF(DIR_SLAVE2,data_rec);
    __delay_ms(100);
    }
    return;
}
