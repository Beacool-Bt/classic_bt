#ifndef __MPU6500_H
#define __MPU6500_H
#include "drv_i2c.h"


#define	MPU6500_ADDR	(0x69)
#define MPU6500_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6500_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU6500_DEFAULT_ADDRESS     MPU6500_ADDRESS_AD0_LOW

//Product ID Description for MPU6500:  | High 4 bits  | Low 4 bits        |
//                                     | Product Name | Product Revision  |
#define MPU6500_REV_C4_ES     0x14  //        0001           0100
#define MPU6500_REV_C5_ES     0x15  //        0001           0101
#define MPU6500_REV_D6_ES     0x16  //        0001           0110
#define MPU6500_REV_D7_ES     0x17  //        0001           0111
#define MPU6500_REV_D8_ES     0x18  //        0001           1000
#define MPU6500_REV_C4        0x54  //        0101           0100
#define MPU6500_REV_C5        0x55  //        0101           0101
#define MPU6500_REV_D6        0x56  //        0101           0110
#define MPU6500_REV_D7        0x57  //        0101           0111
#define MPU6500_REV_D8        0x58  //        0101           1000
#define MPU6500_REV_D9        0x59  //        0101           1001

#define MPU6500_RA_ST_X_GYRO        0x00
#define MPU6500_RA_ST_Y_GYRO        0x01
#define MPU6500_RA_ST_Z_GYRO        0x02
#define MPU6500_RA_ST_X_ACCEL       0x0D
#define MPU6500_RA_ST_Y_ACCEL       0x0E
#define MPU6500_RA_ST_Z_ACCEL       0x0F
#define MPU6500_RA_XG_OFFS_USRH     0x13 //[15:0] XG_OFFS_USR
#define MPU6500_RA_XG_OFFS_USRL     0x14
#define MPU6500_RA_YG_OFFS_USRH     0x15 //[15:0] YG_OFFS_USR
#define MPU6500_RA_YG_OFFS_USRL     0x16
#define MPU6500_RA_ZG_OFFS_USRH     0x17 //[15:0] ZG_OFFS_USR
#define MPU6500_RA_ZG_OFFS_USRL     0x18
#define MPU6500_RA_SMPLRT_DIV       0x19
#define MPU6500_RA_CONFIG           0x1A
#define MPU6500_RA_GYRO_CONFIG      0x1B
#define MPU6500_RA_ACCEL_CONFIG     0x1C
#define MPU6500_RA_ACCEL_CONFIG_2   0x1D
#define MPU6500_RA_LP_ACCEL_ODR     0x1E
#define MPU6500_RA_WOM_THR          0x1F

#define MPU6500_RA_FIFO_EN          0x23
#define MPU6500_RA_I2C_MST_CTRL     0x24
#define MPU6500_RA_I2C_SLV0_ADDR    0x25
#define MPU6500_RA_I2C_SLV0_REG     0x26
#define MPU6500_RA_I2C_SLV0_CTRL    0x27
#define MPU6500_RA_I2C_SLV1_ADDR    0x28
#define MPU6500_RA_I2C_SLV1_REG     0x29
#define MPU6500_RA_I2C_SLV1_CTRL    0x2A
#define MPU6500_RA_I2C_SLV2_ADDR    0x2B
#define MPU6500_RA_I2C_SLV2_REG     0x2C
#define MPU6500_RA_I2C_SLV2_CTRL    0x2D
#define MPU6500_RA_I2C_SLV3_ADDR    0x2E
#define MPU6500_RA_I2C_SLV3_REG     0x2F
#define MPU6500_RA_I2C_SLV3_CTRL    0x30
#define MPU6500_RA_I2C_SLV4_ADDR    0x31
#define MPU6500_RA_I2C_SLV4_REG     0x32
#define MPU6500_RA_I2C_SLV4_DO      0x33
#define MPU6500_RA_I2C_SLV4_CTRL    0x34
#define MPU6500_RA_I2C_SLV4_DI      0x35
#define MPU6500_RA_I2C_MST_STATUS   0x36
#define MPU6500_RA_INT_PIN_CFG      0x37
#define MPU6500_RA_INT_ENABLE       0x38
#define MPU6500_RA_DMP_INT_STATUS   0x39
#define MPU6500_RA_INT_STATUS       0x3A
#define MPU6500_RA_ACCEL_XOUT_H     0x3B
#define MPU6500_RA_ACCEL_XOUT_L     0x3C
#define MPU6500_RA_ACCEL_YOUT_H     0x3D
#define MPU6500_RA_ACCEL_YOUT_L     0x3E
#define MPU6500_RA_ACCEL_ZOUT_H     0x3F
#define MPU6500_RA_ACCEL_ZOUT_L     0x40
#define MPU6500_RA_TEMP_OUT_H       0x41
#define MPU6500_RA_TEMP_OUT_L       0x42
#define MPU6500_RA_GYRO_XOUT_H      0x43
#define MPU6500_RA_GYRO_XOUT_L      0x44
#define MPU6500_RA_GYRO_YOUT_H      0x45
#define MPU6500_RA_GYRO_YOUT_L      0x46
#define MPU6500_RA_GYRO_ZOUT_H      0x47
#define MPU6500_RA_GYRO_ZOUT_L      0x48
#define MPU6500_RA_EXT_SENS_DATA_00 0x49
#define MPU6500_RA_EXT_SENS_DATA_01 0x4A
#define MPU6500_RA_EXT_SENS_DATA_02 0x4B
#define MPU6500_RA_EXT_SENS_DATA_03 0x4C
#define MPU6500_RA_EXT_SENS_DATA_04 0x4D
#define MPU6500_RA_EXT_SENS_DATA_05 0x4E
#define MPU6500_RA_EXT_SENS_DATA_06 0x4F
#define MPU6500_RA_EXT_SENS_DATA_07 0x50
#define MPU6500_RA_EXT_SENS_DATA_08 0x51
#define MPU6500_RA_EXT_SENS_DATA_09 0x52
#define MPU6500_RA_EXT_SENS_DATA_10 0x53
#define MPU6500_RA_EXT_SENS_DATA_11 0x54
#define MPU6500_RA_EXT_SENS_DATA_12 0x55
#define MPU6500_RA_EXT_SENS_DATA_13 0x56
#define MPU6500_RA_EXT_SENS_DATA_14 0x57
#define MPU6500_RA_EXT_SENS_DATA_15 0x58
#define MPU6500_RA_EXT_SENS_DATA_16 0x59
#define MPU6500_RA_EXT_SENS_DATA_17 0x5A
#define MPU6500_RA_EXT_SENS_DATA_18 0x5B
#define MPU6500_RA_EXT_SENS_DATA_19 0x5C
#define MPU6500_RA_EXT_SENS_DATA_20 0x5D
#define MPU6500_RA_EXT_SENS_DATA_21 0x5E
#define MPU6500_RA_EXT_SENS_DATA_22 0x5F
#define MPU6500_RA_EXT_SENS_DATA_23 0x60
#define MPU6500_RA_MOT_DETECT_STATUS    0x61
#define MPU6500_RA_I2C_SLV0_DO      0x63
#define MPU6500_RA_I2C_SLV1_DO      0x64
#define MPU6500_RA_I2C_SLV2_DO      0x65
#define MPU6500_RA_I2C_SLV3_DO      0x66
#define MPU6500_RA_I2C_MST_DELAY_CTRL   0x67
#define MPU6500_RA_SIGNAL_PATH_RESET    0x68
#define MPU6500_RA_MOT_DETECT_CTRL      0x69
#define MPU6500_RA_USER_CTRL        0x6A
#define MPU6500_RA_PWR_MGMT_1       0x6B
#define MPU6500_RA_PWR_MGMT_2       0x6C
#define MPU6500_RA_BANK_SEL         0x6D
#define MPU6500_RA_MEM_START_ADDR   0x6E
#define MPU6500_RA_MEM_R_W          0x6F
#define MPU6500_RA_DMP_CFG_1        0x70
#define MPU6500_RA_DMP_CFG_2        0x71
#define MPU6500_RA_FIFO_COUNTH      0x72
#define MPU6500_RA_FIFO_COUNTL      0x73
#define MPU6500_RA_FIFO_R_W         0x74
#define MPU6500_RA_WHO_AM_I         0x75

#define MPU6500_RA_XA_OFFSET_H      0x77
#define MPU6500_RA_XA_OFFSET_L      0x78
#define MPU6500_RA_YA_OFFSET_H      0x7A
#define MPU6500_RA_YA_OFFSET_L      0x7B
#define MPU6500_RA_ZA_OFFSET_H      0x7D
#define MPU6500_RA_ZA_OFFSET_L      0x7E

#define MPU6500_TC_PWR_MODE_BIT     7
#define MPU6500_TC_OFFSET_BIT       6
#define MPU6500_TC_OFFSET_LENGTH    6
#define MPU6500_TC_OTP_BNK_VLD_BIT  0

#define MPU6500_VDDIO_LEVEL_VLOGIC  0
#define MPU6500_VDDIO_LEVEL_VDD     1

#define MPU6500_CFG_EXT_SYNC_SET_BIT    5
#define MPU6500_CFG_EXT_SYNC_SET_LENGTH 3
#define MPU6500_CFG_DLPF_CFG_BIT    2
#define MPU6500_CFG_DLPF_CFG_LENGTH 3

#define MPU6500_DLPF_BW_256         0x00
#define MPU6500_DLPF_BW_188         0x01
#define MPU6500_DLPF_BW_98          0x02
#define MPU6500_DLPF_BW_42          0x03
#define MPU6500_DLPF_BW_20          0x04
#define MPU6500_DLPF_BW_10          0x05
#define MPU6500_DLPF_BW_5           0x06

#define MPU6500_GCONFIG_XG_ST_BIT       7
#define MPU6500_GCONFIG_YG_ST_BIT       6
#define MPU6500_GCONFIG_ZG_ST_BIT       5
#define MPU6500_GCONFIG_FS_SEL_BIT      4
#define MPU6500_GCONFIG_FS_SEL_LENGTH   2


#define MPU6500_GYRO_FS_250         0x00
#define MPU6500_GYRO_FS_500         0x01
#define MPU6500_GYRO_FS_1000        0x02
#define MPU6500_GYRO_FS_2000        0x03

#define MPU6500_ACONFIG_XA_ST_BIT           7
#define MPU6500_ACONFIG_YA_ST_BIT           6
#define MPU6500_ACONFIG_ZA_ST_BIT           5
#define MPU6500_ACONFIG_AFS_SEL_BIT         4
#define MPU6500_ACONFIG_AFS_SEL_LENGTH      2
#define MPU6500_ACONFIG_ACCEL_HPF_BIT       2
#define MPU6500_ACONFIG_ACCEL_HPF_LENGTH    3

#define MPU6500_ACONFIG2_FCHOICE_B_BIT      2
#define MPU6500_ACONFIG2_FCHOICE_B_LENGTH   2
#define MPU6500_ACONFIG2_DLPF_BIT           0
#define MPU6500_ACONFIG2_DLPF_LENGTH        2

#define MPU6500_ACCEL_DLPF_BW_460   0x00
#define MPU6500_ACCEL_DLPF_BW_184   0x01
#define MPU6500_ACCEL_DLPF_BW_92    0x02
#define MPU6500_ACCEL_DLPF_BW_41    0x03
#define MPU6500_ACCEL_DLPF_BW_20    0x04
#define MPU6500_ACCEL_DLPF_BW_10    0x05
#define MPU6500_ACCEL_DLPF_BW_5     0x06

#define MPU6500_ACCEL_FS_2          0x00
#define MPU6500_ACCEL_FS_4          0x01
#define MPU6500_ACCEL_FS_8          0x02
#define MPU6500_ACCEL_FS_16         0x03

#define MPU6500_DHPF_RESET          0x00
#define MPU6500_DHPF_5              0x01
#define MPU6500_DHPF_2P5            0x02
#define MPU6500_DHPF_1P25           0x03
#define MPU6500_DHPF_0P63           0x04
#define MPU6500_DHPF_HOLD           0x07


#define MPU6500_PWR1_DEVICE_RESET_BIT   7
#define MPU6500_PWR1_SLEEP_BIT          6
#define MPU6500_PWR1_CYCLE_BIT          5
#define MPU6500_PWR1_TEMP_DIS_BIT       3
#define MPU6500_PWR1_CLKSEL_BIT         2
#define MPU6500_PWR1_CLKSEL_LENGTH      3

#define MPU6500_CLOCK_INTERNAL          0x00
#define MPU6500_CLOCK_PLL_XGYRO         0x01
#define MPU6500_CLOCK_PLL_YGYRO         0x02
#define MPU6500_CLOCK_PLL_ZGYRO         0x03
#define MPU6500_CLOCK_PLL_EXT32K        0x04
#define MPU6500_CLOCK_PLL_EXT19M        0x05
#define MPU6500_CLOCK_KEEP_RESET        0x07

#define MPU6500_PWR2_LP_WAKE_CTRL_BIT       7
#define MPU6500_PWR2_LP_WAKE_CTRL_LENGTH    2
#define MPU6500_PWR2_STBY_XA_BIT            5
#define MPU6500_PWR2_STBY_YA_BIT            4
#define MPU6500_PWR2_STBY_ZA_BIT            3
#define MPU6500_PWR2_STBY_XG_BIT            2
#define MPU6500_PWR2_STBY_YG_BIT            1
#define MPU6500_PWR2_STBY_ZG_BIT            0




#define MPU6500_DEG_PER_LSB_250  (float)((2 * 250.0) / 65536.0)
#define MPU6500_DEG_PER_LSB_500  (float)((2 * 500.0) / 65536.0)
#define MPU6500_DEG_PER_LSB_1000 (float)((2 * 1000.0) / 65536.0)
#define MPU6500_DEG_PER_LSB_2000 (float)((2 * 2000.0) / 65536.0)

#define MPU6500_G_PER_LSB_2      (float)((2 * 2) / 65536.0)
#define MPU6500_G_PER_LSB_4      (float)((2 * 4) / 65536.0)
#define MPU6500_G_PER_LSB_8      (float)((2 * 8) / 65536.0)
#define MPU6500_G_PER_LSB_16     (float)((2 * 16) / 65536.0)




void mpu6500_init(void);
bool mpu6500_test(void);

bool mpu6500_test_connection(void);


// SMPLRT_DIV register
uint8_t mpu6500_get_rate(void);
void mpu6500_set_rate(uint8_t rate);



// WHO_AM_I register
uint8_t mpu6500_get_id(void);



#endif /* __MPU6500_H */

