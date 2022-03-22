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
/**********************************************************/
