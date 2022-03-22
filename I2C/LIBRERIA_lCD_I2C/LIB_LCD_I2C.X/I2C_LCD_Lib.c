
#include "I2C_LCD_Lib.h"

/***************************************I2C protocol*******************************************/

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
void WritePCF(uint8_t Dir, uint8_t dato){
    I2C_Start();
    I2C_Write_Data(Dir);
    I2C_Write_Data(dato);
    I2C_Stop();
}

/**************************************LCD - PCF8574*********************************************/
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
    
    I2C_Init_Master(_I2C_STANDART_SPEED);
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
    __delay_us(100);
    WritePCF(dir,0);
}

void I2C_Lcd_String(uint8_t dir, char *text){
    while(*text != '\0'){
        I2C_Lcd_Char(dir,*text);
        text++;
    }
    
}
