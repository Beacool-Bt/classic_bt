#ifndef __DRV_LSM6DS33_H
#define __DRV_LSM6DS33_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
//#include "board.h"
#include "yc11xx.h"

/************** I2C Address *****************/
#define LSM6DS3_I2C_ADDRESS      0x6B
/************** Who am I  *******************/
#define LSM6DS3_WHO_AM_I         0x69

/************** Device Register  *******************/
#define LSM6DS3_RAM_ACCESS  	0X01
#define LSM6DS3_SENSOR_SYNC_TIME  	0X04
#define LSM6DS3_SENSOR_SYNC_EN  	0X05
#define LSM6DS3_FIFO_CTRL1  	0X06
#define LSM6DS3_FIFO_CTRL2  	0X07
#define LSM6DS3_FIFO_CTRL3  	0X08
#define LSM6DS3_FIFO_CTRL4  	0X09
#define LSM6DS3_FIFO_CTRL5  	0X0A
#define LSM6DS3_ORIENT_CFG_G  	0X0B
#define LSM6DS3_REFERENCE_G  	0X0C
#define LSM6DS3_INT1_CTRL  	0X0D
#define LSM6DS3_INT2_CTRL  	0X0E
#define LSM6DS3_WHO_AM_I_REG  	0X0F
#define LSM6DS3_CTRL1_XL  	0X10
#define LSM6DS3_CTRL2_G  	0X11
#define LSM6DS3_CTRL3_C  	0X12
#define LSM6DS3_CTRL4_C  	0X13
#define LSM6DS3_CTRL5_C  	0X14
#define LSM6DS3_CTRL6_G  	0X15
#define LSM6DS3_CTRL7_G  	0X16
#define LSM6DS3_CTRL8_XL  	0X17
#define LSM6DS3_CTRL9_XL  	0X18
#define LSM6DS3_CTRL10_C  	0X19
#define LSM6DS3_MASTER_CONFIG  	0X1A
#define LSM6DS3_WAKE_UP_SRC  	0X1B
#define LSM6DS3_TAP_SRC  	0X1C
#define LSM6DS3_D6D_SRC  	0X1D
#define LSM6DS3_STATUS_REG  	0X1E
#define LSM6DS3_OUTX_L_G  	0X22
#define LSM6DS3_OUTX_H_G  	0X23
#define LSM6DS3_OUTY_L_G  	0X24
#define LSM6DS3_OUTY_H_G  	0X25
#define LSM6DS3_OUTZ_L_G  	0X26
#define LSM6DS3_OUTZ_H_G  	0X27
#define LSM6DS3_OUTX_L_XL  	0X28
#define LSM6DS3_OUTX_H_XL  	0X29
#define LSM6DS3_OUTY_L_XL  	0X2A
#define LSM6DS3_OUTY_H_XL  	0X2B
#define LSM6DS3_OUTZ_L_XL  	0X2C
#define LSM6DS3_OUTZ_H_XL  	0X2D
#define LSM6DS3_SENSORHUB1_REG  	0X2E
#define LSM6DS3_SENSORHUB2_REG  	0X2F
#define LSM6DS3_SENSORHUB3_REG  	0X30
#define LSM6DS3_SENSORHUB4_REG  	0X31
#define LSM6DS3_SENSORHUB5_REG  	0X32
#define LSM6DS3_SENSORHUB6_REG  	0X33
#define LSM6DS3_SENSORHUB7_REG  	0X34
#define LSM6DS3_SENSORHUB8_REG  	0X35
#define LSM6DS3_SENSORHUB9_REG  	0X36
#define LSM6DS3_SENSORHUB10_REG  	0X37
#define LSM6DS3_SENSORHUB11_REG  	0X38
#define LSM6DS3_SENSORHUB12_REG  	0X39
#define LSM6DS3_FIFO_STATUS1  	0X3A
#define LSM6DS3_FIFO_STATUS2  	0X3B
#define LSM6DS3_FIFO_STATUS3  	0X3C
#define LSM6DS3_FIFO_STATUS4  	0X3D
#define LSM6DS3_FIFO_DATA_OUT_L  	0X3E
#define LSM6DS3_FIFO_DATA_OUT_H  	0X3F
#define LSM6DS3_TIMESTAMP0_REG  	0X40
#define LSM6DS3_TIMESTAMP1_REG  	0X41
#define LSM6DS3_TIMESTAMP2_REG  	0X42
#define LSM6DS3_STEP_COUNTER_L  	0X4B
#define LSM6DS3_STEP_COUNTER_H  	0X4C
#define LSM6DS3_FUNC_SRC  	0X53
#define LSM6DS3_TAP_CFG1  	0X58
#define LSM6DS3_TAP_THS_6D  	0X59
#define LSM6DS3_INT_DUR2  	0X5A
#define LSM6DS3_WAKE_UP_THS  	0X5B
#define LSM6DS3_WAKE_UP_DUR  	0X5C
#define LSM6DS3_FREE_FALL  	0X5D
#define LSM6DS3_MD1_CFG  	0X5E
#define LSM6DS3_MD2_CFG  	0X5

#ifndef ERROR
#define ERROR -1
#endif

#ifndef SUCC
#define SUCCESS 0
#endif

typedef unsigned char ErrorStatus;


ErrorStatus LSM6DS3_ReadData(uint8_t *buffer,uint8_t len);
ErrorStatus LSM6DS3_GetAccData(uint8_t *buff);
ErrorStatus LSM6DS3_GetGyroData(uint8_t *buff);
void lsm6ds3_hw_init(void);

#ifdef __cplusplus
}
#endif

#endif
