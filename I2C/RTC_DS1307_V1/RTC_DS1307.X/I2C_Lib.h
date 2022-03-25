/*
 * File:   I2C_Lib.c
 * Author: Diego
 *
 * Created on 5 de junio de 2020, 05:36 PM
 */


#include "config.h"
#include <stdint.h>

/*velocidad de transmision I2c*/
#define _I2C_STANDART_SPEED 100000
#define _I2C_HIGH_SPEED 4000000

/*interrupcion*/
#define _I2C_INT_ON 1
#define _I2C_INT_OFF 0

void I2C_Init_Master(unsigned long speed_master,uint8_t interrupt_);
void I2C_Start(void);
void I2C_Write_Data(unsigned char dato);
void I2C_Restart(void);
void I2C_Reception(void);
void I2C_Stop(void);
unsigned char I2C_Read_Slave(void);
void I2C_N_ACK(void);
void I2C_Init_Slave(unsigned char address,uint8_t interrupt_);
void I2C_Idle(void);  //modo inactivo
void I2C_ACK(void);
void I2C_Read_Data(uint8_t dato);
/**********************************************************/

void I2C_Init_Master(unsigned long speed_master,uint8_t interrupt_){
    ADCON1|=(7<<0);  
    TRISB |= 0x03;                              //Configuring SDA and SCL as input pins
    SSPCON1 = 0x28;                             //Configuring as I2C Master mode with SCL clock frequency= FOSC/(4 * (SSPADD + 1))
    SSPADD = (unsigned char)((_XTAL_FREQ/(4*speed_master))-1);     //Setting SCL frequency with value in clock_freq
    SSPSTAT = 0x80;
    
    /*MSSP Interrupcion*/
    if(interrupt_){
        GIE =1;
        PEIE=1;
        SSPIE=1;
        SSPIF = 0;  
    }else{}
}
void I2C_Start(void){

    I2C_Idle();                                
    SEN=1; 
    while(SSPCON2bits.SEN);//Iniciar la condicion de star
    if(SSPSTATbits.S){};
          
}

void I2C_Write_Data(unsigned char dato){
    SSPBUF = dato; //byte de dirreccion + 1bit de escritura(0) 
    while(SSPSTATbits.BF); //esperar a que el registro buffer quede vacio
    while(SSPSTATbits.R_nW); //esperar a que todos los datos se transmitan (acabe el 9 ciclo de SLC)
    I2C_Idle();
    if(ACKSTAT)                              //check if acknowledgement signal received 
    {
        I2C_Restart();                            //If no acknowledgement received, give restart signal and transmit data again
        SSPBUF=dato;
    }
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
/*I2c maestro generando condicion de recepcion de datos*/
void I2C_Reception(void){ 
    SSPCON2bits.RCEN=1; //habilitando modo de recepcion de datos, del MODO MAESTRO I2C
    while(SSPSTATbits.BF==0);////esperar a que el registro buffer se llene, cuando cambia a 1 significa que el buffe ya recibio todos los datos

}

unsigned char I2C_Read_Slave(void){
    
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

void I2C_Init_Slave(unsigned char address,uint8_t interrupt_){
    ADCON1 = 0x0F;
    TRISB0 = 1;                     /* Set up I2C lines by setting as input */
    TRISB1 = 1;
    SSPSTAT = 80;                   /* Slew rate disabled, other bits are cleared */
    SSPCON1 = 0x36;	
    SSPADD = address; 					
    SSPCON2 = 0x01;
    
    if(interrupt_){
        GIE=1;
        PEIE=1;
        SSPIE = 1;                      
        SSPIF = 0;
    }else{}
}

void I2C_Idle(void)
{
  
   while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
   
/* Función de espera: mientras se esté iniciada una comunicación,
esté habilitado una recepción, esté habilitado una parada
esté habilitado un reinicio de la comunicación, esté iniciada
una comunicación o se este transmitiendo, el IC2 PIC se esperará    
antes de realizar algún trabajo
     */ 
}



void I2C_ACK(void){
    ACKDT=0;                                    
    ACKEN=1; 
    while(SSPCON2bits.ACKEN);
    
}

void I2C_Read_Data(uint8_t dato){
    SSPBUF = dato; //byte de dirreccion + 1bit de escritura(0) 
    dato=(uint8_t)((dato&0xFE)|0x01);
    while(SSPSTATbits.BF); //esperar a que el registro buffer quede vacio
    while(SSPSTATbits.R_nW); //esperar a que todos los datos se transmitan (acabe el 9 ciclo de SLC)
    I2C_Idle();
    if(ACKSTAT)                              //check if acknowledgement signal received 
    {
        I2C_Restart();                            //If no acknowledgement received, give restart signal and transmit data again
        SSPBUF=dato;
    }
}