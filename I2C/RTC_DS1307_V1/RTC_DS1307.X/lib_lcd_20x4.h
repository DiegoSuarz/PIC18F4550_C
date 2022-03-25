// LIBRERIA DE 4 BITS

// Establecer las conexiones de la LCD
#define TRIS_RS TRISDbits.RD0
#define PIN_RS  LATDbits.LATD0

#define TRIS_E TRISDbits.RD1
#define PIN_E  LATDbits.LATD1

#define TRIS_D4 TRISDbits.RD4
#define PIN_D4  LATDbits.LATD4

#define TRIS_D5 TRISDbits.RD5
#define PIN_D5  LATDbits.LATD5

#define TRIS_D6 TRISDbits.RD6
#define PIN_D6  LATDbits.LATD6

#define TRIS_D7 TRISDbits.RD7
#define PIN_D7  LATDbits.LATD7

/********************************************************/
// DEFINICION DE  COMANDOS PROPUESTOS POR EL FABRICANTE //
//           CON MODELO MIKROC                          //
/********************************************************/
#define      _LCD_FIRST_ROW           128
#define      _LCD_SECOND_ROW          192
#define      _LCD_THIRD_ROW           148
#define      _LCD_FOURTH_ROW          212
#define      _LCD_CLEAR               1
#define      _LCD_RETURN_HOME         2
#define      _LCD_CURSOR_OFF          12
#define      _LCD_UNDERLINE_ON        14
#define      _LCD_BLINK_CURSOR_ON     15
#define      _LCD_MOVE_CURSOR_LEFT    16
#define      _LCD_MOVE_CURSOR_RIGHT   20
#define      _LCD_TURN_OFF            0
#define      _LCD_TURN_ON             8
#define      _LCD_SHIFT_LEFT          24
#define      _LCD_SHIFT_RIGHT         28

//definir los metodos a utilizar
void Lcd_Init(void);
void enviar_dato(unsigned char cmd);
void Lcd_Char(char letra);
void Lcd_Cmd(unsigned char cmd);
void Lcd_XY(unsigned char y, unsigned char x);
void Lcd_Char_XY(unsigned char y, unsigned char x, char letra);
void Lcd_Out(char *text);
void Lcd_Out_XY(unsigned char y, unsigned char x, char *text);



/******************************************************/
void enviar_dato(unsigned char cmd){
    TRIS_D4=0;
    TRIS_D5=0;
    TRIS_D6=0;
    TRIS_D7=0;
    PIN_D4=0;
    PIN_D5=0;
    PIN_D6=0;
    PIN_D7=0;
    
    PIN_E=0;
    __delay_us(100);
    
    PIN_D4=(cmd>>4)&0x01;
    PIN_D5=(cmd>>5)&0x01;
    PIN_D6=(cmd>>6)&0x01;
    PIN_D7=(cmd>>7)&0x01;
    PIN_E=1;
    __delay_us(100);
    PIN_E=0;
    
    PIN_D4=(cmd&0x0F);
    PIN_D5=((cmd&0x0F)>>1);
    PIN_D6=((cmd&0x0F)>>2);
    PIN_D7=((cmd&0x0F)>>3);
    
    PIN_E=1;
    __delay_us(100);
    PIN_E=0;
    
    TRIS_D4=1;
    TRIS_D5=1;
    TRIS_D6=1;
    TRIS_D7=1;
       
    return;
}

void Lcd_Init(void){
    TRIS_E=0;
    TRIS_RS=0;
    TRIS_D4=0;
    TRIS_D5=0;
    TRIS_D6=0;
    TRIS_D7=0;
    
    PIN_RS=0;
    __delay_ms(15);
    // secuencia de sincro
    enviar_dato(0x30);
    __delay_ms(5);
    enviar_dato(0x30);
    __delay_us(100);
    enviar_dato(0x32);
    __delay_us(100);
    
    //configurar la lcd
    __delay_us(100);
    enviar_dato(0x2B);
    __delay_us(100);
    enviar_dato(0x08);
    __delay_us(100);
    enviar_dato(0x0F);
    __delay_us(100);
    enviar_dato(0x01);
    __delay_us(100);
    enviar_dato(0x06);
    __delay_us(100);
    
    enviar_dato(0x0F);
    __delay_us(100);  
    
    Lcd_Cmd(12);
    __delay_us(100);
    return;
}

void Lcd_Cmd(unsigned char cmd){
    PIN_RS=0;
    enviar_dato(cmd);
    PIN_RS=0;
    return;
}
void Lcd_Char(char letra){
    PIN_RS=1;
    enviar_dato(letra);
    PIN_RS=0;
    return;
}

void Lcd_XY(unsigned char y, unsigned char x){
    switch(y){
        case 1: Lcd_Cmd(0x80+(x-1)); break;
        case 2: Lcd_Cmd(0xC0+(x-1)); break;
        case 3: Lcd_Cmd(0x94+(x-1)); break;
        case 4: Lcd_Cmd(0xD4+(x-1)); break;
    }
    __delay_us(100);
    PIN_RS=0;
    return;
}
void Lcd_Char_XY(unsigned char y, unsigned char x, char letra){
    Lcd_XY(y,x);
    PIN_RS=1;
    enviar_dato(letra);
    PIN_RS=0;
    return;
}
void Lcd_Out(char *text){
    PIN_RS=1;
    while(*text){
        Lcd_Char(*text);
        text++;
    }
    return;
}

void Lcd_Out_XY(unsigned char y, unsigned char x, char *text){
    Lcd_XY(y,x);
    PIN_RS=1;
    while(*text){
        Lcd_Char(*text);
        text++;
    }
    return;
}

