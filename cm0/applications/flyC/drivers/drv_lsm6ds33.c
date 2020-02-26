#include "drv_lsm6ds33.h"
#include "drv_i2c.h"


static ErrorStatus LSM6DS3_ReadReg(uint8_t Reg, uint8_t* Data) 
{
	//if(!I2C_ReadByte(LSM6DS3_I2C_ADDRESS,Reg,Data))
  if(!i2c_read(LSM6DS3_I2C_ADDRESS, Reg, Data,1)) 
	return ERROR;                                                        
  else                                                                       
	return SUCCESS;                                                      
}


static ErrorStatus LSM6DS3_WriteReg(uint8_t Reg, uint8_t Data) 
{

  i2c_write_byte(LSM6DS3_I2C_ADDRESS, Reg, Data);    
  return ERROR;                                                      
}

ErrorStatus LSM6DS3_ReadWHOAMI(uint8_t *value)
{
 
 if( !LSM6DS3_ReadReg(LSM6DS3_WHO_AM_I_REG,value) )
    return ERROR;


  return SUCCESS;

 }
ErrorStatus LSM6DS3_ReadData(uint8_t *buffer,uint8_t len) 
 {  
  if(IIC_read(LSM6DS3_I2C_ADDRESS, LSM6DS3_OUTX_L_G, buffer,len)) 
	return ERROR;                                                        
  else                                                                       
	return SUCCESS;
 
 }
ErrorStatus LSM6DS3_GetAccData(uint8_t *buff) 
{
  uint8_t i, j, k;
  uint8_t numberOfByteForDimension;
  
  numberOfByteForDimension=6/3;

  k=0;
  for (i=0; i<3;i++ ) 
  {
	for (j=0; j<numberOfByteForDimension;j++ )
	{	
		if( !LSM6DS3_ReadReg(LSM6DS3_OUTX_L_XL+k, &buff[k]))
		  return ERROR;
		k++;	
	}
  }

  return SUCCESS; 
}
ErrorStatus LSM6DS3_GetGyroData(uint8_t *buff) 
{
  uint8_t i, j, k;
  uint8_t numberOfByteForDimension;
  
  numberOfByteForDimension=6/3;

  k=0;
  for (i=0; i<3;i++ ) 
  {
	for (j=0; j<numberOfByteForDimension;j++ )
	{	
		if( !LSM6DS3_ReadReg(LSM6DS3_OUTX_L_G+k, &buff[k]))
		  return ERROR;
		k++;	
	}
  }

  return SUCCESS; 
}

void lsm6ds3_hw_init(void)
{
	 uint8_t id =0;

	LSM6DS3_ReadWHOAMI(&id);
	if(id==LSM6DS3_WHO_AM_I)
	{
    LSM6DS3_WriteReg(LSM6DS3_CTRL1_XL,0x5e);//208HZ,2g,bandwidth 100Hz
    LSM6DS3_WriteReg(LSM6DS3_CTRL2_G,0x6c);//2000dps,416Hz
    LSM6DS3_WriteReg(LSM6DS3_CTRL3_C,0x44);
    LSM6DS3_WriteReg(LSM6DS3_CTRL4_C,0x80);
    LSM6DS3_WriteReg(LSM6DS3_CTRL5_C,0x00);
    LSM6DS3_WriteReg(LSM6DS3_CTRL6_G,0x00);//XL_HM_MODE
    LSM6DS3_WriteReg(LSM6DS3_CTRL7_G,0x32);//G_HM_MODE,GYRO HPF
    LSM6DS3_WriteReg(LSM6DS3_CTRL8_XL,0xc0);//ACC HPF
    LSM6DS3_WriteReg(LSM6DS3_CTRL9_XL,0x38);//ACC_EN
    LSM6DS3_WriteReg(LSM6DS3_CTRL10_C,0x3c);//GYRO_EN
    LSM6DS3_WriteReg(LSM6DS3_TAP_CFG1,0x10);//LPF2
	}
}

