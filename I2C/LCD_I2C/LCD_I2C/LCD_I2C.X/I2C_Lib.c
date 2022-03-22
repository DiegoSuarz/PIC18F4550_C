/*
 * File:   I2C_Lib.c
 * Author: Diego
 *
 * Created on 5 de junio de 2020, 05:36 PM
 */
#include <pic18f4550.h>

#include "config.h"
#include "I2C_Lib.h"

void I2C_Init_Master(unsigned long clock_freq){
    ADCON1 = 0x0F;   
    TRISB |= 0x03;                              //Configuring SDA and SCL as input pins
    SSPCON1 = 0x28;                             //Configuring as I2C Master mode with SCL clock frequency= FOSC/(4 * (SSPADD + 1))
    SSPADD = (_XTAL_FREQ/(4*clock_freq))-1;     //Setting SCL frequency with value in clock_freq
    SSPSTAT = 0x80;
    
    /*MSSP INTERRUPT*/
//    GIE =1;
//    PEIE=1;
//    SSPIE=1;
    SSPIF = 0;  
}

void I2C_Start(unsigned char addres_wr){

    I2C_Idle();                                
    SEN=1; 
    while(SSPCON2bits.SEN);//Initiate start condition
    if(SSPSTATbits.S);
    SSPBUF = addres_wr; //byte de dirreccion + 1bit de escritura(0) 
    while(SSPSTATbits.BF); //esperar a que el registro buffer quede vacio
    while(SSPSTATbits.R_nW); //esperar a que todos los datos se transmitan (acabe el 9 ciclo de SLC)

    
}

void I2C_Write_Data(unsigned char dato){
    SSPBUF = dato; //byte de dirreccion + 1bit de escritura(0) 
    while(SSPSTATbits.BF); //esperar a que el registro buffer quede vacio
    while(SSPSTATbits.R_nW); //esperar a que todos los datos se transmitan (acabe el 9 ciclo de SLC)
//    while(!ACKSTAT)                              //check if acknowledgement signal received 
//    {
//        I2C_Restart();                            //If no acknowledgement received, give restart signal and transmit data again
//        SSPBUF=dato;
//    }
  }


void I2C_Stop(void){
    /*generar condicion de parada */
    SSPCON2bits.PEN=1;//inicializamos la condicion de  STOP
    while(SSPCON2bits.PEN); //esperamos a que termine de generarse la condicion  de STOP
    while(!SSPSTATbits.P);
    SSPCON2bits.PEN=0;
    
}

void I2C_Restart(void){
    //habilitar recepcion para comunicacion de esclavo a maestro
    SSPCON2bits.RSEN=1; //generar señal de restart
    while(SSPCON2bits.RSEN); //esperar a que termine de generar la señal de restart
 
}
void I2C_Reception(void){
    SSPCON2bits.RCEN=1; //habilitando modo de recepcion de datos, del MODO MAESTRO I2C
    while(SSPSTATbits.BF==0);////esperar a que el registro buffer se llene, cuando cambia a 1 significa que el buffe ya recibio todos los datos

}

unsigned short I2C_Read_S_M(void){
    
    unsigned char dato;
    
    I2C_Reception();
    I2C_Idle();
    dato = SSPBUF;
    RCEN=0;  
    return dato;
}

void I2C_N_ACK(void){
    /*Enviando señal de NO ACK*/
    SSPCON2bits.ACKDT=1; //preparando para enviar bit de NO ACK
    SSPCON2bits.ACKEN=1; //genera la señal de NO ACK
    while(SSPCON2bits.ACKEN);
  
}

void I2C_Init_Slave(unsigned char address){
//    ADCON1 = 0x0F;
//    TRISB0 = 1;                     /* Set up I2C lines by setting as input */
//    TRISB1 = 1;
//    SSPSTAT = 80;                   /* Slew rate disabled, other bits are cleared */
//    SSPCON1 = 0x36;	
//    SSPADD = address; 					
//    SSPCON2 = 0x01;
//    
//   /* MSSP INTERRUPT */  
//    GIE=1;
//    PEIE=1;
//    SSPIE = 1;                      
//    SSPIF = 0;
}

void I2C_Idle(void)
{
  // Wait for Status Idle (i.e. ACKEN, RCEN, PEN, RSEN, SEN) and I2C Bus stop detection
   while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void mssp_Wait(void)
{
    while(!SSPIF);                             
    SSPIF=0;
}

void I2C_ACK(void){
    ACKDT=0;                                    
    ACKEN=1; 
    while(SSPCON2bits.ACKEN);
    
}