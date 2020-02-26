#include "mpu6050.h"
#include "yc11xx_iic.h"


static void mpu6050_read_reg(uint8_t addr, uint8_t *data)
{
  I2C_ReadDatatoBuff(MPU6050_DEFAULT_ADDRESS, I2C_REGADDR8BITS(addr), data, 1);
}

static void mpu6050_write_reg(uint8_t addr, uint8_t data )
{
  I2C_SendData(MPU6050_DEFAULT_ADDRESS, I2C_REGADDR8BITS(addr), data);
}

static void mpu6050_read_regs(uint8_t addr, uint8_t *data, uint8_t len)
{
  I2C_ReadDatatoBuff(MPU6050_DEFAULT_ADDRESS, I2C_REGADDR8BITS(addr), data, len);
}

void mpu6050_init(void)
{


	  // reset
     mpu6050_write_reg(MPU_RA_PWR_MGMT_1, 0x80);
     delay_ms(100);

	
	
	mpu6050_write_reg(MPU_RA_SIGNAL_PATH_RESET, BIT_GYRO | BIT_ACC | BIT_TEMP);
	delay_ms(5);

		
	//�����豸ʱ��Դ��������Z��
	mpu6050_write_reg(MPU_RA_PWR_MGMT_1, 0x03);
	delay_ms(5);

    mpu6050_write_reg(MPU_RA_PWR_MGMT_2, 0x00);
	delay_ms(5);

	
    mpu6050_write_reg(MPU_RA_SMPLRT_DIV, 0x00);
	delay_ms(5);

	
	//i2c��·ģʽ
	mpu6050_write_reg(MPU_RA_INT_PIN_CFG, 0 << 7 | 0 << 6 | 0 << 5 | 0 << 4 | 0 << 3 | 0 << 2 | 1 << 1 | 0 << 0);
	delay_ms(5);

	//��ͨ�˲�Ƶ�ʣ�0x03(42Hz)
	mpu6050_write_reg(MPU_RA_CONFIG, MPU6050_LPF_42HZ);
	delay_ms(5);

	
	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
	mpu6050_write_reg(MPU_RA_GYRO_CONFIG, 18);
	delay_ms(5);

	
	//���ټ��Լ졢������Χ(���Լ죬+-8G)			
	mpu6050_write_reg(MPU_RA_ACCEL_CONFIG, 2 << 3);
	delay_ms(5);

}



