#include <xc.h>
#include "config.h"
#include "LIB_PCF8574.h"



void I2C_Init_Master(unsigned long speed_master){
    ADCON1|=(7<<0);  
    TRISB |= 0x03;                              //Configuring SDA and SCL as input pins
    SSPCON1 = 0x28;                             //Configuring as I2C Master mode with SCL clock frequency= FOSC/(4 * (SSPADD + 1))
    SSPADD = (unsigned char)((_XTAL_FREQ/(4*speed_master))-1);     //Setting SCL frequency with value in clock_freq
    SSPSTAT = 0x80;
            
}

void I2C_Idle(void)
{
  
   while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
   
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

void I2C_Restart(void){
    //habilitar recepcion para comunicacion de esclavo a maestro
    SSPCON2bits.RSEN=1; //generar señal de restart
    while(SSPCON2bits.RSEN); //esperar a que termine de generar la señal de restart
 
}
void I2C_Stop(void){
    /*generar condicion de parada */
    SSPCON2bits.PEN=1;//inicializamos la condicion de  STOP
    while(SSPCON2bits.PEN); //esperamos a que termine de generarse la condicion  de STOP
    while(!SSPSTATbits.P);
    SSPCON2bits.PEN=0;
    
}



/***************************************PCF8574*******************************************/
void WritePCF(uint8_t dato){
    I2C_Start();
    I2C_Write_Data((DIR_PCF<<1)|DIR_PCF_INT);
    I2C_Write_Data(dato|LED_LCD);
    I2C_Stop();
}

/**************************************LCD - PCF8574*********************/
 
void dato_LCD(uint8_t ctrl,uint8_t cmd){
    uint8_t dato;
    if((cmd&1)==1) dato=dato|LCD_D4; else dato=dato&~LCD_D4;
    if((cmd&2)==2) dato=dato|LCD_D5; else dato=dato&~LCD_D5;
    if((cmd&4)==4) dato=dato|LCD_D6; else dato=dato&~LCD_D6;
    if((cmd&8)==8) dato=dato|LCD_D7; else dato=dato&~LCD_D7;
    __delay_us(60);
    
    if(ctrl==1)
        dato = dato|LCD_RS;  //se quieren enviar datos
    else
        dato = dato&~LCD_RS; //se quieren enviar comandos
    
    WritePCF(dato|LCD_E);
    __delay_us(2);
    WritePCF(dato &~LCD_E);
}

void trama_LCD(uint8_t ctrl, uint8_t cmd){
    dato_LCD(ctrl,cmd>>4);   //enviar nibble alto
    dato_LCD(ctrl,cmd&0x0F);    //enviar nibble bajo
}

void I2C_LCD_Init(void){
    I2C_Init_Master(_I2C_STANDART_SPEED);
    __delay_ms(15);
    I2C_Start();
    I2C_Write_Data((DIR_PCF<<1)|DIR_PCF_INT);
    I2C_Write_Data(0x00);
    I2C_Stop();
    __delay_ms(15);
    
    for(uint8_t i=0;i<3;i++){
        dato_LCD(0,0x03);
        __delay_ms(5);
    }
    
    dato_LCD(0,0x02);
    trama_LCD(0,0x28);
    __delay_ms(5);
    trama_LCD(0,0x08);
    __delay_ms(5);
    trama_LCD(0,0x01);
    __delay_ms(5);
    trama_LCD(0,0x0F);
    __delay_ms(5);
   
}

void I2C_LCD_Char(uint8_t letra){
    trama_LCD(1,letra);         //1==envio de datos, 0==envio de comandos
    __delay_us(10);
}

void  I2C_LCD_Cmd(uint8_t cmd){
    trama_LCD(0,cmd);
    __delay_us(10);
}

void I2C_LCD_XY(uint8_t y, uint8_t x){
    
    switch(y){
        case 1: I2C_LCD_Cmd(0x80 + (x-1));break;
        case 2: I2C_LCD_Cmd(0xC0 + (x-1));break;
        case 3: I2C_LCD_Cmd(0x94 + (x-1));break;
        case 4: I2C_LCD_Cmd(0xD4 + (x-1));break;
    }
    __delay_ms(100);
    return;
}

void I2C_LCD_String(char *text){
    while(*text != '\0'){
        I2C_LCD_Char(*text);
        text++;
    }
    return;
}