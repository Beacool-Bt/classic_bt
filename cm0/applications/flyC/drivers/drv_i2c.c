
#include "drv_i2c.h"
#include "yc11xx_iic.h"
#include "yc11xx_gpio.h"

#define IIC_SCL_GPIO_NUM					22	//gpio 22
#define IIC_SDA_GPIO_NUM					21	//gpio 21

void i2c_init(void)
{
	I2C_InitTypeDef mem_test;	//定义I2C初始化结构体
	mem_test.I2C_RXLen = 20;	//依次对初始化信息赋值
	mem_test.I2C_TXLen = 20;
	mem_test.I2C_ClockSpeed = IICD_CLOCKSPEED400KHZ;
	I2C_Init(&mem_test);		//调用初始化函数
	
	GPIO_SetGpioMultFunction(IIC_SCL_GPIO_NUM,  GPCFG_IIC_SCL);	//配置GPIO  22为SCL引脚
	GPIO_SetGpioMultFunction(IIC_SDA_GPIO_NUM, GPCFG_IIC_SDA );	//配置GPIO 21为SDA引脚
}






