
#define STANDART_SPEED 100000
#define HIGH_SPEED 4000000

void I2C_Init_Master(unsigned long clock_freq);
void I2C_Start(unsigned char addres_wr);
void I2C_Write_Data(unsigned char dato);
void I2C_Restart(void);
void I2C_Reception(void);
void I2C_Stop(void);
unsigned short I2C_Read_S_M(void);
void I2C_N_ACK(void);
void I2C_Init_Slave(unsigned char address);
void I2C_Idle(void);
void mssp_Wait(void);
void I2C_ACK(void);
