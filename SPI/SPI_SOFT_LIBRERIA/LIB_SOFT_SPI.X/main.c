/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 26 de marzo de 2022, 05:19 PM
 */


#include <xc.h>
#include "CONFIG_Lib.h"
#include <stdint.h>
#include <stdbool.h>


#define SDI_TRIS     TRISDbits.RD0
#define SD0_TRIS     TRISDbits.RD1
#define CLK_TRIS     TRISDbits.RD2
#define SS_TRIS      TRISDbits.RD3

#define SDI         PORTDbits.RD0
#define SDO         PORTDbits.RD1
#define CLK         PORTDbits.RD2
#define SS          PORTDbits.RD3

#define IN  1
#define OUT 0


void SPI_SOFT_Init(void);
void SPI_SOFT_Tx(uint8_t dato);
uint8_t SPI_SOFT_Rx(void);


int main(void) {
    
    TRISB = 0x00;
    SPI_SOFT_Init();
 
// CARGA CON 0X00 AL REGISTRO DE STATUS DE LA MEMORIA (PARA QUE NO EXISTA MEMORIA BLOQUEADA A LA ESCRITURA)    
    SS = 1;       //sclavo inactivo
    SS = 0;          //esclavo activo
    SPI_SOFT_Tx(0b00000001);  //REGISTRO PARA LA ESCRITURA DEL RESGISTRO STATUS DE LA MEMORIA
    SPI_SOFT_Tx(0b00000000);
    SS = 1;       //sclavo inactivo      
    __delay_ms(10);

//PERMISO DE ESCRITURA      
    SS = 0;          //esclavo activo
    SPI_SOFT_Tx(0b00000110);  //REGISTRO PARA LA ESCRITURA DEL RESGISTRO STATUS DE LA MEMORIA
    SS = 1;       //sclavo inactivo      
    __delay_ms(10);
    
    SS = 0;          //esclavo activo
    SPI_SOFT_Tx(0b00000010);  //ORDEN PARA LA ESCRITURA DE DATOS DE LA MEMORIA
    //SS = 1;       //sclavo inactivo      
    __delay_ms(10);
    
    //enviar direccion de memoria a la cual se quiere escribir (0x0000);
    //SS = 0;          //esclavo activo
    SPI_SOFT_Tx(0b00000000);
    SPI_SOFT_Tx(0b00000000);
    //SS = 1;       //sclavo inactivo      
    __delay_ms(10);
    
    //enviar dato a escribir en la memoria en al direccion 0x0000;
    //SS = 0;          //esclavo activo
    SPI_SOFT_Tx(0b00000011);
    SPI_SOFT_Tx(0b00001111);
    SS = 1;       //sclavo inactivo      
    __delay_ms(10);
    
    
    
    
    uint8_t dato,dato1;
    SS = 0;          //esclavo activo
    SPI_SOFT_Tx(0b00000011); //ORDEN PARA LA LECTURA DE DATOS DE LA MEMORIA
    //ENVIAMOS LA DIRECCION DESDE LA QUE SE VA A LEER (0x0000)
    SPI_SOFT_Tx(0b00000000);
    SPI_SOFT_Tx(0b00000000);
    __delay_ms(1000);
    SS = 1;
     __delay_ms(1000);
      SS = 0;
    SPI_SOFT_Tx(0b00000011);
    SPI_SOFT_Tx(0b00000000);
    SPI_SOFT_Tx(0b00000000);
    
    dato=SPI_SOFT_Rx();
    LATB=dato;
    __delay_ms(600);
    dato1=SPI_SOFT_Rx();
    LATB=dato1;
    SS = 1;
  
   
        
    
    
    
    while(1){
    //LECTURA DE LOS 32 BYTES DE LA MEMORIA
        
    }
    
    return 0;
}

void SPI_SOFT_Init(void){
    
    SDI_TRIS = IN;
    SD0_TRIS = OUT;    
    CLK_TRIS = OUT;    
    SS_TRIS = OUT;
    
    
    
}
void SPI_SOFT_Tx(uint8_t dato){
    
    for(uint8_t i=0;i<8;i++){
        CLK = 0;
        //SDO=((dato<<i)&0x80)?1:0;
        if((dato<<i)&0x80)SDO=1; else SDO=0;
        CLK =1;
    }
    
}
uint8_t SPI_SOFT_Rx(void){
    uint8_t buffer=0;
       
    int8_t i=7;
    do{
        --i;
        CLK=0;
        buffer |= (SDI<<(i+1));
        CLK=1;
        __delay_ms(10);
        
    }while(i>=0);
    
    return buffer;
}