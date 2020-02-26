#ifndef _YC11XX_H_
#define _YC11XX_H_
#include <stdio.h>
#include "ycdef.h"
#include "btreg.h" 

#ifdef __cplusplus
extern "C" {
#endif

#define OS_ENTER_CRITICAL __disable_irq
#define OS_EXIT_CRITICAL __enable_irq
#define OS_SLEEP __WFI

/* -------------------------  Interrupt Number Definition  ------------------------ */
typedef enum 
{
/* -------------------  Cortex-M0 Processor Exceptions Numbers  -------------------                                                                 */
	Reset_IRQn                    = -15,              /*!<   1  Reset Vector, invoked on Power up and warm reset                  */
	NonMaskableInt_IRQn           = -14,              /*!<   2  Non maskable Interrupt, cannot be stopped or preempted         */
    	HardFault_IRQn                = -13,              /*!<   3  Hard Fault, all classes of Fault                                                 */
    	SVCall_IRQn                   =  -5,              /*!<  11  System Service Call via SVC instruction                                  */
    	DebugMonitor_IRQn             =  -4,              /*!<  12  Debug Monitor                                                                      */
    	PendSV_IRQn                   =  -2,              /*!<  14  Pendable request for system service                                       */
    	SysTick_IRQn                  =  -1,              /*!<  15  System Tick Timer                                                                */
/* ---------------------  C32F0xx Specific Interrupt Numbers  --------------------                                                                     */
	usb_handler_IRQn              =   0,
  	iicd_handler_IRQn             =   1,
  	qspi_handler_IRQn             =   2,
  	spid_handler_IRQn             =   3,
  	uart_handler_IRQn             =   4,
  	uartb_handler_IRQn            =   5,
  	adcd_handler_IRQn             =   6,
  	i2s_handler_IRQn              =   7,
  	bt_handler_IRQn               =   8,
  	GPIO0_IRQ            =   9,
  	GPIO1_IRQ            =   10,
  	GPIO2_IRQ            =   11,
  	GPIO3_IRQ            =   12,
  	GPIO4_IRQ            =   13,
  	GPIO5_IRQ            =   14,
  	GPIO6_IRQ            =   15,
  	GPIO7_IRQ            =   16,
  	GPIO8_IRQ            =   17,
  	GPIO9_IRQ            =   18,
  	GPIO10_IRQ            =   19,
  	GPIO11_IRQ            =   20,
  	GPIO12_IRQ            =   21,
  	GPIO13_IRQ            =   22,
  	GPIO14_IRQ            =   23,
  	GPIO15_IRQ            =   24,
  	GPIO16_IRQ            =   25,
  	GPIO17_IRQ            =   26,
  	GPIO18_IRQ            =   27,
  	GPIO19_IRQ            =   28,
  	GPIO20_IRQ            =   29,
  	GPIO21_IRQ            =   30,
  	GPIO22_IRQ            =   31,
} IRQn_Type;


/** @addtogroup Configuration_of_CMSIS
  * @{
  */


/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* ----------------Configuration of the Cortex-M0 Processor and Core Peripherals---------------- */
#define __CM0_REV                 0x0000            /*!< Cortex-M0 Core Revision                                               */
#define __MPU_PRESENT                  0            /*!< MPU present or not                                                    */
#define __NVIC_PRIO_BITS               2            /*!< Number of Bits used for Priority Levels                               */
#define __Vendor_SysTickConfig         0            /*!< Set to 1 if different SysTick Config is used                          */
/** @} */ /* End of group Configuration_of_CMSIS */

#include <core_cm0.h>                               /*!< Cortex-M0 processor and core peripherals                              */



//******************************************************************************************
//start define reg address
//******************************************************************************************
/**
#define CORE_CHIPID							  0x8000
#define CORE_KICK                  0x8003
#define CORE_LPM_WR                0x8005
#define CORE_MISC_CTRL             0x8006
#define CORE_OTP_RDATA             0x8007
#define CORE_DMA_START             0x800a
#define CORE_RESET                 0x8010
#define CORE_ENCRYPT               0x8011
#define CORE_UART_TXD              0x8015
#define CORE_UCODE_HI              0x8022
#define CORE_UCODE_CTRL            0x8023
#define CORE_UCODE_LOW             0x8024
#define CORE_UCODE_DATA            0x8025
#define	CORE_SCS_WDATA						0x8026
#define	CORE_SCS_ADDR							0x802a
#define	CORE_FLASH_OFFSET					0x802c
#define	CORE_FLASH_READ_CMD				0x802f
#define	CORE_I2S_CTRL							0x8030
#define  CORE_I2S_CLKDIV        	0x8031
#define	CORE_I2S_RX_SADDR					0x8032
#define	CORE_I2S_TX_SADDR					0x8034
#define	CORE_I2S_LEN							0x8036
#define	CORE_ADCD_MAP						0x8038
#define CORE_ADCD_DELAY						0x8039
#define CORE_ADCD_CTRL                 	    0x803B
#define	CORE_ADCD_SADDR						0x803c
#define	CORE_ADCD_EADDR						0x803e
#define	CORE_RTHALFSLOT						0x8040
#define	CORE_CLKSEL								0x8042
#define	CORE_CONFIG								0x8043
#define CORE_UART_CLKSEL               		0X8043
#define	CORE_OTP_ADDR_SET					0x8044
#define	CORE_OTPD_CTRL						0x8046
#define	CORE_OTP_CTRL							0x8046
#define	CORE_OTPD_RXADDR					0x8048
#define	CORE_OTPD_RXLEN						0x804a
#define	CORE_LPM_REG							0x804c
#define	CORE_CLKOFF								0x8050
#define	CORE_UART_BAUD						0x8052
#define	CORE_UART_RX_SADDR				0x8054
#define	CORE_UART_RX_EADDR				0x8056
#define	CORE_UART_RX_RPTR					0x8058
#define	CORE_UART_TX_SADDR				0x805a
#define	CORE_UART_TX_EADDR				0x805c
#define	CORE_UART_TX_WPTR					0x805e
#define	CORE_UART_CTRL						0x8060
#define	CORE_GPIO_KEY							0x8061
#define	CORE_CORDIC_MODE					0x8063
#define	CORE_SUM_EN								0x8063
#define	CORE_ROWCOLUMN_NUM				0x8064
#define	CORE_SCANCYCLE_TIMER			0x8065
#define	CORE_U_DEBOUNCE						0x8066
#define	CORE_M_DEBOUNCE						0x8067
#define	CORE_MODKEY								0x8068
#define	CORE_KSCTRL								0x8070
#define	CORE_UARTB_CTRL						0x8071
#define	CORE_UARTB_BAUD						0x8072
#define	CORE_UARTB_RX_SADDR				0x8074
#define	CORE_UARTB_RX_EADDR				0x8076
#define	CORE_UARTB_RX_RPTR				0x8078
#define	CORE_UARTB_TX_SADDR				0x807a
#define	CORE_UARTB_TX_EADDR				0x807c
#define	CORE_UARTB_TX_WPTR				0x807e
#define	CORE_GPIO_CONF						0x8080
#define CORE_GPIO0_CONF                 				0X8080
#define CORE_GPIO1_CONF                 				0X8081
#define CORE_GPIO2_CONF                 				0X8082
#define CORE_GPIO3_CONF                 				0X8083
#define CORE_GPIO4_CONF                 				0X8084
#define CORE_GPIO5_CONF                 				0X8085
#define CORE_GPIO6_CONF                 				0X8086
#define CORE_GPIO7_CONF                 				0X8087
#define CORE_GPIO8_CONF                 				0X8088
#define	CORE_QSPI_CTRL						0x80a0
#define	CORE_QSPI_DELAY						0x80a1
#define	CORE_QSPI_TXLEN						0x80a2
#define	CORE_QSPI_TXADDR					0x80a4
#define	CORE_QSPI_RXADDR					0x80a6
#define	CORE_QSPI_RXLEN						0x80a8
#define	CORE_IICD_CTRL						0x80aa
#define  CORE_IICD_SCL_LOW              0x80ab
#define  CORE_IICD_SCL_HIGH             0x80ac
#define  CORE_IICD_START_SETUP          0x80ad
#define  CORE_IICD_START_HOLD           0x80ae
#define  CORE_IICD_STOP_SETUP           0x80af
#define  CORE_IICD_DATA_SETUP           0x80b0
#define  CORE_IICD_DATA_HOLD            0x80b1
#define	CORE_IICD_TXLEN								0x80b2
#define	CORE_IICD_TXADDR							0x80b4
#define	CORE_IICD_RXADDR							0x80b6
#define	CORE_IICD_RXLEN							0x80b8
#define	CORE_SPID_CTRL							0x80ba
#define	CORE_SPID_DELAY							0x80bb
#define	CORE_SPID_TXLEN						0x80bc
#define	CORE_SPID_TXADDR					0x80be
#define	CORE_SPID_RXADDR					0x80c0
#define	CORE_SPID_RXLEN						0x80c2
#define	CORE_CORDIC_A							0x80c4
#define	CORE_CORDIC_B							0x80c8
#define	CORE_BIST_CTRL_REG				0x80cc
#define	CORE_CCNT_BIT							0x80cc
#define  CORE_PWM0_PCNT                 0x80cd
#define  CORE_PWM0_NCNT                 0x80cf
#define	CORE_PWM0_CTRL						0x80d1
#define  CORE_PWM1_PCNT                 0x80d2
#define  CORE_PWM1_NCNT                 0x80d4
#define	CORE_PWM1_CTRL						0x80d6
#define  CORE_PWM2_PCNT                 0x80d7
#define  CORE_PWM2_NCNT                 0x80d9
#define	CORE_PWM2_CTRL						0x80db
#define  CORE_PWM3_PCNT                 0x80dc
#define  CORE_PWM3_NCNT                 0x80de
#define	CORE_PWM3_CTRL						0x80e0
#define  CORE_PWM4_PCNT                 0x80e1
#define  CORE_PWM4_NCNT                 0x80e3
#define	CORE_PWM4_CTRL						0x80e5
#define  CORE_PWM5_PCNT                 0x80e6
#define  CORE_PWM5_NCNT                 0x80e8
#define	CORE_PWM5_CTRL						0x80ea
#define  CORE_PWM6_PCNT                 0x80eb
#define  CORE_PWM6_NCNT                 0x80ed
#define	CORE_PWM6_CTRL						0x80ef
#define  CORE_PWM7_PCNT                 0x80f0
#define  CORE_PWM7_NCNT                 0x80f2
#define	CORE_PWM7_CTRL						0x80f4
#define	CORE_CVSD_CTRL						0x80f5
#define	CORE_CVSDIN_SADDR					0x80f6
#define	CORE_CVSDOUT_SADDR				0x80f8
#define	CORE_CVSD_LEN							0x80fa
#define	CORE_PCMIN_SADDR					0x80fc
#define	CORE_PCMOUT_SADDR					0x80fe
#define	CORE_DAC_SADDR						0x8100
#define	CORE_DAC_LEN							0x8102
#define	CORE_DAC_CTRL							0x8104
#define	CORE_VOL_TEST							0x8105
#define	CORE_THRESHOLD_GAIN				0x8106
#define	CORE_THRESHOLD_MAX				0x8108
#define	CORE_VOX_TIMER_WINDOW			0x810a
#define	CORE_VOX_CTRL							0x810b
#define	CORE_DS_COEF_A						0x810c
#define	CORE_CALIBRATION_WINDOW		0x810f
#define	CORE_CLKN									0x8300
#define	CORE_EXTMCLK							0x8304
#define	CORE_MISC_STATUS					0x8308
#define	CORE_REG_ZW_COUNTER				0x8309
#define	CORE_UART_STATUS					0x830c
#define	CORE_UART_RBAUD						0x830c
#define	CORE_UART_TX_ITEMS				0x830e
#define	CORE_UART_TX_RPTR					0x8310
#define	CORE_UART_RX_ITEMS				0x8312
#define	CORE_UART_RX_WPTR					0x8314
#define	CORE_ADCD_ADDR						0x8316
#define	CORE_KEY_STAT							0x8318
#define	CORE_KEY_EVENT						0x8319
#define	CORE_KEY_EVENT_NUM				0x831a
#define	CORE_DEBUG_BAUD						0x831b
#define	CORE_GPIO_IN							0x831c
#define	CORE_I2S_WRPTR						0x8320
#define	CORE_I2S_RDPTR						0x8322
#define	CORE_SPID_REMAIN					0x8324
#define	CORE_OTPD_REMAIN					0x8326
#define	CORE_QSPI_REMAIN					0x8328
#define	CORE_IICD_REMAIN					0x832a
#define	CORE_SARDATA							0x832c
#define	CORE_OTP_ADDR							0x832e
#define	CORE_DMA_STATUS						0x8330
#define	CORE_PERF_STATUS					0x8331
#define	CORE_CORDIC_OUT						0x8332
#define	CORE_LPM_CTRL							0x8336
#define	CORE_LPM_STATUS						0x8342
#define	CORE_CCNT_COUNTER					0x834b
#define	CORE_BIST_DONE						0x834e
#define	CORE_BIST_FAIL						0x8350
#define	CORE_SUMDATA							0x8352
#define	CORE_UARTB_STATUS					0x8354
#define	CORE_UARTB_RBAUD					0x8354
#define	CORE_UARTB_TX_ITEMS				0x8356
#define	CORE_UARTB_TX_RPTR				0x8358
#define	CORE_UARTB_RX_ITEMS				0x835a
#define	CORE_UARTB_RX_WPTR				0x835c
#define	CORE_SCS_RDATA						0x835e
#define	CORE_PCMIN_PTR						0x8362
#define	CORE_PCMOUT_PTR						0x8364
#define	CORE_CVSDIN_PTR						0x8366
#define	CORE_CVSDOUT_PTR					0x8368
#define CORE_DAC_OUTPUT_PTR					0x836a

#define CORE_SARADC_CONFIG1				0x8971
#define CORE_SARADC_CONFIG2				0x8972
#define CORE_SARADC_CONFIG3				0x8973
#define CORE_SARADC_CONFIG4				0x8974
#define CORE_RF_ADC_CONFIG				0x8906
//high pass filter
#define CORE_MIC_HPF   						0x8114
#define CORE_MIC_HPF_CTR					0x8115


#define CORE_SBC_SADDR 0x810a 
#define CORE_SBC_DADDR 0x810c 
#define CORE_SBC_SLEN  0x810e 
#define CORE_SBC_DLEN  0x8110 
#define CORE_SBC_SWP   0x8112 
#define CORE_SBC_SRP   0x836c 
#define CORE_SBC_DWP   0x836e 
#define CORE_EXE_ADDR  0x8370 
#define CORE_SBC_CTRL    0x8064
#define CORE_SBC_CRL    0x800b


#define CORE_USB_CONFIG 		 0x8c00
#define CORE_USB_INT_MASK  		 0x8c01
#define CORE_USB_ADDR  			 0x8c04
#define CORE_USB_TRIG  			 0x8c10
#define CORE_USB_STALL			 0x8c11
#define CORE_USB_CLEAR  		 0x8c12
#define CORE_USB_EP				 0X8C18
#define CORE_USB_DFIFO(x)   			 (0x8c18+x)
#define CORE_USB_EP_LEN  		 0x8c20

#define CORE_USB_STATUS 		 0x8c26
#define CORE_USB_FIFO_EMPTY 	 0x8c27
**/

#define CORE_CHIPID                         	0x8000
#define CORE_REVID                           0x8001
#define CORE_KICK                            0x8003
#define CORE_LPM_WR                          0x8005
#define CORE_MISC_CTRL                       0x8006
#define CORE_OTP_RDATA                       0x8007
#define CORE_DMA_START                       0x800A
#define CORE_SBC_CLR                         0x800B
#define CORE_RESET                           0x8010
#define CORE_ENCRYPT                         0x8011
#define CORE_UART_TXD                        0x8015
#define CORE_UCODE_HI                        0x8022
#define CORE_UCODE_CTRL                      0x8023
#define CORE_UCODE_LOW                       0x8024
#define CORE_UCODE_DATA                      0x8025
                                        
#define CORE_SCS_WDATA                       0x8026
#define CORE_SCS_ADDR                        0x802A
#define CORE_FLASH_OFFSET                    0x802C
#define CORE_FLASH_READ_CMD                  0x802F
#define CORE_I2S_CTRL                        0x8030
#define CORE_I2S_CLKDIV                      0x8031
#define CORE_I2S_RX_SADDR                    0x8032
#define CORE_I2S_TX_SADDR                    0x8034
#define CORE_I2S_LEN                         0x8036
#define CORE_ADCD_DCOFF                      0x8038
#define CORE_ADCD_DELAY                      0x803A
#define CORE_ADCD_CTRL                       0x803B
#define CORE_ADCD_SADDR                      0x803C
#define CORE_ADCD_EADDR                      0x803E
#define CORE_RTHALFSLOT                      0x8040
#define CORE_CLKSEL                          0x8042
#define CORE_CONFIG                          0x8043
#define CORE_UART_CLKSEL                     0x8043
#define CORE_OTP_ADDR                        0x8044
#define CORE_OTP_DIN                         0x8046
#define CORE_OTP_CTRL                        0x8047
#define CORE_OTPD_RXADDR                     0x8048
#define CORE_OTPD_RXLEN                      0x804A
#define CORE_LPM_REG                         0x804C
#define CORE_LPM_ISOGATE                     0x804F
#define CORE_CLKOFF                          0x8050
#define CORE_UART_BAUD                       0x8052
#define CORE_UART_RX_SADDR                   0x8054
#define CORE_UART_RX_EADDR                   0x8056
#define CORE_UART_RX_RPTR                    0x8058
#define CORE_UART_TX_SADDR                   0x805A
#define CORE_UART_TX_EADDR                   0x805C
#define CORE_UART_TX_WPTR                    0x805E
#define CORE_UART_CTRL                       0x8060
#define CORE_GPIO_KEY0                       0x8061
#define CORE_GPIO_KEY1                       0x8062
#define CORE_GPIO_KEY2                       0x8063
#define CORE_SUM_EN                          0x8063
                                         
#define CORE_SBC_CTRL                        0x8064
#define CORE_TONE_ENABLE                     0x8065
#define CORE_TONE1                           0x8066
#define CORE_TONE2                           0x8067
#define CORE_TONE3                           0x8068
#define CORE_TONE4                           0x8069
#define CORE_TOFF1                           0x806A
#define CORE_TOFF2                           0x806B
#define CORE_TOFF3                           0x806C
#define CORE_TOFF4                           0x806D
#define CORE_TONE_REPEAT                     0x806E
#define CORE_TONE_SELECT                     0x806F
#define CORE_TONE_SELECT2                    0x8070
                                          
#define CORE_KSCN_RCTC                       0x8064
#define CORE_KSCN_SCANCYCLE_TIMER_L          0x8065
#define CORE_KSCN_SCANCYCLE_TIMER_H          0x8066
#define CORE_KSCN_M_DEBOUNCE                 0x8067
#define CORE_KSCN_MODKEY                     0x8068
#define CORE_KSCN_ROWMASK                    0x806F
#define CORE_KSCN_KSCTRL                     0x8070
                                         
#define CORE_UARTB_CTRL                      0x8071
#define CORE_UARTB_BAUD                      0x8072
#define CORE_UARTB_RX_SADDR                  0x8074
#define CORE_UARTB_RX_EADDR                  0x8076
#define CORE_UARTB_RX_RPTR                   0x8078
#define CORE_UARTB_TX_SADDR                  0x807A
#define CORE_UARTB_TX_EADDR                  0x807C
#define CORE_UARTB_TX_WPTR                   0x807E
#define CORE_GPIO_CONF                       0x8080
#define CORE_QSPI_CTRL				                0x80A0
#define CORE_QSPI_DELAY                      0x80A1
#define CORE_QSPI_TXLEN			                0x80A2
#define CORE_QSPI_TXADDR			                0x80A4
#define CORE_QSPI_RXADDR			                0x80A6
#define CORE_QSPI_RXLEN			                0x80A8
#define CORE_IICD_CTRL				                0x80AA
#define CORE_IICD_SCL_LOW                    0x80AB
#define CORE_IICD_SCL_HIGH                   0x80AC
#define CORE_IICD_START_SETUP                0x80AD
#define CORE_IICD_START_HOLD                 0x80AE
#define CORE_IICD_STOP_SETUP                 0x80AF
#define CORE_IICD_DATA_SETUP                 0x80B0
#define CORE_IICD_DATA_HOLD                  0x80B1
#define CORE_IICD_TXLEN			                0x80B2
#define CORE_IICD_TXADDR			                0x80B4
#define CORE_IICD_RXADDR			                0x80B6
#define CORE_IICD_RXLEN			                0x80B8
#define CORE_SPID_CTRL				                0x80BA
#define CORE_SPID_DELAY                      0x80BB
#define CORE_SPID_TXLEN			                0x80BC
#define CORE_SPID_TXADDR			                0x80BE
#define CORE_SPID_RXADDR			                0x80C0
#define CORE_SPID_RXLEN			                0x80C2
#define CORE_CORDIC_A				                0x80C4
#define CORE_CORDIC_B				                0x80C8
#define CORE_BIST_CTRL                       0x80CC
#define CORE_CCNT_BIT				                0x80CC
                                           
#define CORE_PWM0_PCNT                       0x80CD
#define CORE_PWM0_NCNT                       0x80CF
#define CORE_PWM0_CTRL				                0x80D1
#define CORE_PWM1_PCNT                       0x80D2
#define CORE_PWM1_NCNT                       0x80D4
#define CORE_PWM1_CTRL				                0x80D6

#define CORE_PWM2_PCNT                       0x80D7
#define CORE_PWM2_NCNT                       0x80D9                                       
#define CORE_PWM2_CTRL				                0x80DB

#define CORE_PWM3_PCNT                       0x80DC
#define CORE_PWM3_NCNT                       0x80DE                                     
#define CORE_PWM3_CTRL				                0x80E0

#define CORE_PWM4_PCNT                       0x80E1
#define CORE_PWM4_NCNT                       0x80E3                                       
#define CORE_PWM4_CTRL				                0x80E5

#define CORE_PWM5_PCNT                       0x80E6
#define CORE_PWM5_NCNT                       0x80E8                                       
#define CORE_PWM5_CTRL				                0x80EA

#define CORE_PWM6_PCNT                       0x80EB
#define CORE_PWM6_NCNT                       0x80ED                                       
#define CORE_PWM6_CTRL				                0x80EF

#define CORE_PWM7_PCNT                       0x80F0
#define CORE_PWM7_NCNT                       0x80F2                                       
#define CORE_PWM7_CTRL				                0x80F4
                                        
#define CORE_CVSD_CTRL				                0x80F5
#define CORE_CVSDIN_SADDR		                0x80F6
#define CORE_CVSDOUT_SADDR		                0x80F8
#define CORE_CVSD_LEN				                0x80FA
#define CORE_PCMIN_SADDR			                0x80FC
#define CORE_PCMOUT_SADDR		                0x80FE
#define CORE_DAC_SADDR                       0x8100
#define CORE_DAC_LEN                         0x8102
#define CORE_DAC_WPTR                        0x8104
#define CORE_DAC_CTRL                        0x8106
#define CORE_DAC_SEL                         0x8107
#define CORE_DAC_LVOL                        0x8108
#define CORE_DAC_RVOL                        0x8109
#define CORE_SBC_SADDR                       0x810A
#define CORE_SBC_DADDR                       0x810C
#define CORE_SBC_SLEN                        0x810E
#define CORE_SBC_DLEN                        0x8110
#define CORE_SBC_SWP                         0x8112
#define CORE_MIC_HPF                         0x8114
#define CORE_MIC_HPF_CTRL                    0x8115
                                        
#define CORE_CLKN                            0x8300
#define CORE_EXTMCLK                         0x8304
#define CORE_MISC_STATUS                     0x8308
#define CORE_EXE_RD                          0x8309
#define CORE_AVG_PWR                         0x830A
#define CORE_UART_STATUS                     0x830C
#define CORE_UART_RBAUD                      0x830C
#define CORE_UART_TX_ITEMS                   0x830E
#define CORE_UART_TX_RPTR                    0x8310
#define CORE_UART_RX_ITEMS                   0x8312
#define CORE_UART_RX_WPTR                    0x8314
#define CORE_ADCD_ADDR                       0x8316
#define CORE_XTAL_CNT                        0x8318
#define CORE_SBC_STATUS                      0x8319
#define CORE_DEBUG_BAUD                      0x831B
#define CORE_GPIO_IN                         0x831C
#define CORE_GPIO_IN1                        0x831D
#define CORE_I2S_WRPTR                       0x8320
#define CORE_I2S_RDPTR                       0x8322
#define CORE_SPID_REMAIN                     0x8324
#define CORE_OTPD_REMAIN                     0x8326
#define CORE_QSPI_REMAIN                     0x8328
#define CORE_IICD_REMAIN                     0x832A
#define CORE_ADC_IN                          0x832C
#define CORE_CURRENT_OTP_ADDR                0x832E
#define CORE_DMA_STATUS                      0x8330
#define CORE_PERF_STATUS                     0x8331
#define CORE_CORDIC_OUT                      0x8332
#define CORE_LPM_CTRL                        0x8336
#define CORE_LPM_XTALCNT                     0x833A
#define CORE_LPM_BUCKCNT                     0x833B
#define CORE_LPM_LDOCNT                      0x833C
#define CORE_LPM_LATCH                       0x833D
#define CORE_GPIO_WAKEUP_LOW                 0x8342
#define CORE_GPIO_WAKEUP_HIGH                0x8346
                                          
#define CORE_CCNT_COUNTER                    0x834B
#define CORE_BIST_DONE                       0x834A
#define CORE_BIST_FAIL                       0x834E
#define CORE_SUMDATA                         0x8352
#define CORE_UARTB_STATUS                    0x8354
#define CORE_UARTB_RBAUD                     0x8354
#define CORE_UARTB_TX_ITEMS                  0x8356
#define CORE_UARTB_TX_RPTR                   0x8358
#define CORE_UARTB_RX_ITEMS                  0x835A
#define CORE_UARTB_RX_WPTR                   0x835C
#define CORE_SCS_RDATA                       0x835E
#define CORE_PCMIN_PTR                      0x8362
#define CORE_PCMOUT_PTR                     0x8364
#define CORE_CVSDIN_PTR                     0x8366
#define CORE_CVSDOUT_PTR                    0x8368
#define CORE_DAC_RPTR                       0x836A
                                         
#define CORE_SBC_SRP                         0x836C
#define CORE_SBC_DWP                         0x836E
#define CORE_EXE_ADDR                        0x8370
                                           
#define CORE_ICE_CTRL                        0x8203
#define CORE_ICE_STATUS                      0x8204
#define CORE_ICE_BREAK0                      0x8205
#define CORE_ICE_BREAK1                      0x8207
                                         
#define   MEM_B_BOX          								 0x8400
#define   MEM_E_BOX          								 0x8500
#define   MEM_L_BOX          								 0x8600
                                       
#define MEMK                                 0x8700
                                          
#define RFEN_RX                              0x8900
#define RFEN_TX                              0x8901
#define RFEN_MDM                             0x8902
#define RFEN_SN                              0x8903
#define RFEN_MSC                             0x8904
#define RFEN_CK                              0x8905
#define RFEN_ADC                             0x8906
#define RF_AGC_CTRL                          0x8912
#define RFEN_ULP                             0x8914
#define RF_ADC_MODE                          0x8971
#define RF_ADC_GC                            0x8972
#define RF_ADC_CH                            0x8973
                                       
#define RF_RX_VCTRL_REG1                     0x894B
#define RF_RX_VCTRL_REG2                     0x894C
#define RF_RX_IB_LNA                         0x894D
#define RF_RX_LNA_CTRIM                      0x894E
                                         
#define RF_BPF_CTRIM                         0x8950
#define RF_BPF_IB                            0x8951
#define RF_ADC_RCCAL                         0x8952
#define RF_RCCAL_CTRL                        0x8952
#define RF_RCCAL_SEL                         0x8953
#define RF_TX_PA                             0x8955
#define RF_TX_MIX                            0x8956
#define RF_PLL_RSTN                          0x895F
#define RF_SDM_EN                            0x8964
#define RF_PLL_INTG                          0x8960
#define RF_PLL_FRAC                          0x8961
#define RF_PLL_FRACHI                        0x8963
#define RF_AFC_CAP                           0x896C
#define RF_CLKPLL_BIAS                       0x8967
#define RF_CLKPLL_EN                         0x896F
#define RF_CLKPLL_INT                        0x8968
#define RF_CLKPLL_FRAC                       0x8969
#define RF_BALUN_CTRIM                       0x896D
#define RF_SARADC_CTRL                       0x8972
#define RFEN_CHGPUMP                         0x8973
#define RF_ADC_BIAS_GAIN                     0x8978
#define RF_ADC_REFBUF                        0x8979
#define RF_AUDIO_VOLL                        0x897A
#define RF_AUDIO_VOLR                        0x897B
#define RF_AUDIO_LDO                         0x897C
#define RF_ADAC_REF_CTL                      0x897D
#define RFEN_AADC                            0x897E
#define RF_AUDIO_PDDAC0                      0x897F
#define RF_AUDIO_PDDAC1                      0x8980
#define RF_AUDIO_DAC_LDO                     0x8981
                                         
#define RF_RCCAL_RESULT                      0x8990
#define RF_AFC_D2A                           0x8991
#define RF_RSSI                              0x8992
                                        
#define CORE_USB_CONFIG		                  0x8C00
#define CORE_USB_INT_MASK	                  0x8C01
#define CORE_USB_ADDR                        0x8C04
#define CORE_USB_TRIG	                      0x8C10
#define CORE_USB_STALL		                    0x8C11
#define CORE_USB_CLEAR		                    0x8C12
#define CORE_USB_EP                          0x8C18
#define CORE_USB_DFIFO0                      0x8C18
#define CORE_USB_DFIFO1                      0x8C19
#define CORE_USB_DFIFO2                      0x8C1A
#define CORE_USB_EP_LEN                      0x8C20
#define CORE_USB_STATUS		                  0x8C26
#define CORE_USB_FIFO_EMPTY                  0x8C27
#define CORE_USB_FIFO_FULL                   0x8C28


//************************************************************************************************
//end define reg address
//************************************************************************************************

//*****************************************************************************
//config gpio selected function
//*****************************************************************************
#define  GPCFG_INPUT			0  
#define  GPCFG_QSPI_NCS        	2  
#define  GPCFG_QSPI_SCK        	3  
#define  GPCFG_QSPI_IO0        		4  
#define  GPCFG_QSPI_IO1        		5  
#define  GPCFG_QSPI_IO2        		6  
#define  GPCFG_QSPI_IO3        		7  
#define  GPCFG_UART_TXD        	8  
#define  GPCFG_UART_RXD        	9  
#define  GPCFG_UART_RTS        	10 
#define  GPCFG_UART_CTS        	11 
#define  GPCFG_UARTB_TXD       	12 
#define  GPCFG_UARTB_RXD       	13 
#define  GPCFG_UARTB_RTS       	14 
#define  GPCFG_UARTB_CTS       	15 
#define  GPCFG_PWM_OUT0        	16 
#define  GPCFG_PWM_OUT1        	17 
#define  GPCFG_PWM_OUT2        	18 
#define  GPCFG_PWM_OUT3        	19 
#define  GPCFG_PWM_OUT4        	20 
#define  GPCFG_PWM_OUT5        	21 
#define  GPCFG_PWM_OUT6        	22 
#define  GPCFG_PWM_OUT7        	23 
#define  GPCFG_I2S_DOUT        	24 
#define  GPCFG_I2S_LRCKOUT     	25 
#define  GPCFG_I2S_CLKOUT      	26 
#define  GPCFG_I2S_DIN         		28 
#define  GPCFG_I2S_LRCKIN      	29 
#define  GPCFG_I2S_CLKIN       		30 
#define  GPCFG_SPID_MISO       	31 
#define  GPCFG_SPID_NCS        	32 
#define  GPCFG_SPID_SCK        	33 
#define  GPCFG_SPID_MOSI       	34 
#define  GPCFG_SPID_SDIO       	35 
#define  GPCFG_JTAG_SWCLK      	36 
#define  GPCFG_JTAG_SWDAT      	37 
#define  GPCFG_QDEC_X0         	38 
#define  GPCFG_QDEC_X1         	39 
#define  GPCFG_QDEC_Y0         	40 
#define  GPCFG_QDEC_Y1         	41 
#define  GPCFG_QDEC_Z0         		42 
#define  GPCFG_QDEC_Z1         		43 
#define  GPCFG_IIC_SCL         		44 
#define  GPCFG_IIC_SDA        		45 
#define  GPCFG_OUTPUT_LOW      	62 
#define  GPCFG_OUTPUT_HIGH     	63
#define  GPCFG_PULLUP			0x40 
#define  GPCFG_PULLDOWN        	0x80 
#define  GPCFG_NO_IE           		0xc0
//*****************************************************************************
//end config gpio selected function
//*****************************************************************************

/**
  *@brief GPIO number.
  */
typedef enum
{
	GPIO_0 = 0,
	GPIO_1,
	GPIO_2,
	GPIO_3,
	GPIO_4,
	GPIO_5,
	GPIO_6,
	GPIO_7,
	GPIO_8,
	GPIO_9,
	GPIO_10,
	GPIO_11,
	GPIO_12,
	GPIO_13,
	GPIO_14,
	GPIO_15,
	GPIO_16,
	GPIO_17,
	GPIO_18,
	GPIO_19,
	GPIO_20,
	GPIO_21,
	GPIO_22,
	GPIO_23,
	GPIO_24,
	GPIO_25,
	GPIO_26,
	GPIO_27,
	GPIO_28,
	GPIO_29,
	GPIO_30,
	GPIO_31,
	GPIO_MAX_NUM,
	GPIO_ACTIVE_BIT=0x80,
}GPIO_NUM;

//*************************************************************************************
//interrupt id
//*************************************************************************************
#define	USB_INTID		0
#define	IICD_INTID		1
#define	QSPI_INTID		2
#define	SPID_INTID		3
#define	KEY_INTID		4
#define	UART_INTID		5
#define	UARTB_INTID	6
#define	ADCD_INTID		7
#define	I2S_INTID		8
#define	BT_INTID		9
#define	RSA_INTID		10
//*************************************************************************************
//end interrupt id
//*************************************************************************************


#define reg_map(reg)		((int)(reg) | 0x10000000)
#define reg_map_m0(reg)	((int)(reg) | 0x10010000)
#define PREFETCH_LINE(addr)	*(volatile int*)0x20000000 = addr
#define des_ctrl			*(volatile uint8_t*)0x30000002
#define des_key(x)		*(volatile uint8_t*)(0x30000003 + x)
#define des_in(x)			*(volatile uint8_t*)(0x30000018 + x)
#define crypt_status		*(volatile uint8_t*)0x30010000
#define des_out(x)			*(volatile uint8_t*)(0x30010004 + x)
#define des_start			*(volatile uint8_t*)0x30008000
#define rsa_exp(x)			*(volatile int32_t*)(0x30020000 + x*4)
#define rsa_out(x)			*(volatile int32_t*)(0x30020000 + x*4)
#define rsa_in(x)			*(volatile int32_t*)(0x30020080 + x*4)
#define rsa_mod(x)		*(volatile int32_t*)(0x30020100 + x*4)
#define rsa_ctrl			*(volatile int32_t*)0x30020180

#define NVIC_SETENA		*(volatile int32_t*)0xe000e100
#define NVIC_CLRENA		*(volatile int32_t*)0xe000e180
#define NVIC_ISPR		*(volatile int32_t*)0xe000e200
#define NVIC_ICPR		*(volatile int32_t*)0xe000e280
#define NVIC_SETPEND      *(volatile int32_t*)0xe000e200
#define NVIC_CLRPEND      *(volatile int32_t*)0xe000e280
#define NVIC_00_03_IPR		*(volatile int32_t*)0xe000e400
#define NVIC_04_07_IPR		*(volatile int32_t*)0xe000e404
#define NVIC_08_11_IPR		*(volatile int32_t*)0xe000e408
#define NVIC_12_15_IPR		*(volatile int32_t*)0xe000e40C
#define NVIC_16_19_IPR		*(volatile int32_t*)0xe000e410
#define NVIC_20_23_IPR		*(volatile int32_t*)0xe000e414
#define NVIC_24_27_IPR		*(volatile int32_t*)0xe000e418
#define NVIC_28_31_IPR		*(volatile int32_t*)0xe000e40C


#define CPU_MHZ        (48*1000000)
/* SysTick registers */  
/* SysTick control & status */ 
#define INITCPU_SYST_CSR     ((volatile unsigned int *)0xE000E010)
/* SysTick Reload value */ 
#define INITCPU_SYST_RVR     ((volatile unsigned int *)0xE000E014)
/* SysTick Current value */ 
#define INITCPU_SYST_CVR     ((volatile unsigned int *)0xE000E018)
/* SysTick CSR register bits */ 
#define INITCPU_SYST_CSR_COUNTFLAG (1 << 16)
#define INITCPU_SYST_CSR_CLKSOURCE (1 << 2)
#define INITCPU_SYST_CSR_TICKINT   (1 << 1)
#define INITCPU_SYST_CSR_ENABLE    (1 << 0)



typedef unsigned char byte;
typedef unsigned short word;

void _nop() __attribute__((optimize("O0")));

#define TO_16BIT_ADDR(A) (((int)A)&0xFFFF)

#define HREAD(reg)		*(volatile byte*)(reg_map(reg))
#define HREADW(reg)		(int)HREAD(reg) | (HREAD(reg + 1) << 8)
#define HREADL(reg)		(int)(HREAD(reg)) | (HREAD(reg + 1) << 8)| (HREAD(reg + 2) << 16)| (HREAD(reg + 3) << 24)
#define HWRITE(reg, val)	HREAD(reg) = (byte)(val)
#define HWRITEW(reg, val)	do { HWRITE(reg, (int)(val));HWRITE(reg + 1, (int)(val) >> 8); }while(0)
#define HWRITEL(reg, val)	do { HWRITE(reg, (int)(val));HWRITE(reg + 1, (int)(val) >> 8);HWRITE(reg + 2, (int)(val) >> 16);HWRITE(reg + 3, (int)(val) >> 24); }while(0)

#define HWRITE24BIT(reg,val) do { HWRITE(reg, (int)(val));HWRITE(reg + 1, (int)(val) >> 8); HWRITE(reg + 2, (int)(val) >> 16); }while(0)
//#define HWOR(reg, val)	HWRITE(reg, HREAD(reg) | (val))

#define HWOR(reg, val)	HWRITE(reg, ((HREAD(reg) )| (val)))
#define HWCOR(reg, val)	HWRITE(reg, HREAD(reg) & ~(val))

//#define SETBIT(reg, val)	HWRITE(reg, HREAD(reg) | (val))
//#define CLRBIT(reg, val)	HWRITE(reg, HREAD(reg) & ~(val))

#define BW(addr)			(int)*(addr) << 24 | (int)*(addr + 1) << 16 | (int)*(addr + 2) << 8 | *(addr + 3)

static inline void hw_delay() 
{
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
}


/***********************************CONFIG**********************************************/
#define BASE_ADDR                                   0x10000000
#define CONFIG_UART_TXD                                    0x08
#define CONFIG_UART_RXD                                    0x09
#define CONFIG_UART_RTS                                    0x0a
#define CONFIG_UART_CTS                                    0x0b
#define CONFIG_UARTB_TXD                                   0x0C
#define CONFIG_UARTB_RXD                                   0x0D
#define CONFIG_UARTB_RTS                                   0x0E
#define CONFIG_UARTB_CTS                                   0x0F

/***********************************MEM CONFIG*******************************************/
/***************************UART A CONFIG BUFF***************************/
#define UART_A_RX_BUFF_START   0x1800
#define UART_A_TX_BUFF_START   0x1900
#define UART_A_RX_BUFF_END     0x18ff
#define UART_A_TX_BUFF_END     0x19ff

/***************************UART B CONFIG BUFF***************************/
#define UART_B_RX_BUFF_START   0x1a00
#define UART_B_TX_BUFF_START   0x1b00
#define UART_B_RX_BUFF_END     0x1aff
#define UART_B_TX_BUFF_END     0x1bff

/***************************RING CONFIG BUFF***************************/
#define RING_RX_BUFF_START   0x4B00
#define RING_TX_BUFF_START   0x4D00
#define RING_RX_BUFF_END     0x4Cff
#define RING_TX_BUFF_END     0x4Eff

/***********************************REG*************************************************/
#define REG_CLOCK_SELECT_RW_11BIT                  0x10008042
#define REG_CONFIG_RW_H5BIT                        0x10008043
#define REG_CLOCK_OFF_1_RW_8BIT                    0x10008050
#define REG_CLOCK_OFF_2_RW_8BIT                    0x10008051
#define REG_GPIO_CONFIG(X)                   (0x10008080+(X))
/* ----------------UART---------------- */
#define REG_UART_A_BAUD_RATE_RW_15BIT              0x10008052
#define REG_UART_A_RX_START_ADDR_RW_16BIT          0x10008054
#define REG_UART_A_RX_END_ADDR_RW_16BIT            0x10008056
#define REG_UART_A_RX_RPTR_RW_16BIT                0x10008058
#define REG_UART_A_TX_START_ADDR_RW_16BIT          0x1000805A
#define REG_UART_A_TX_END_ADDR_RW_16BIT            0x1000805C
#define REG_UART_A_TX_WPTR_RW_16BIT                0x1000805E
#define REG_UART_A_CONTROL_RW_8BIT                 0x10008060

#define REG_UART_B_BAUD_RATE_RW_15BIT              0x10008072
#define REG_UART_B_RX_START_ADDR_RW_16BIT          0x10008074
#define REG_UART_B_RX_END_ADDR_RW_16BIT            0x10008076
#define REG_UART_B_RX_RPTR_RW_16BIT                0x10008078
#define REG_UART_B_TX_START_ADDR_RW_16BIT          0x1000807A
#define REG_UART_B_TX_END_ADDR_RW_16BIT            0x1000807C
#define REG_UART_B_TX_WPTR_RW_16BIT                0x1000807E
#define REG_UART_B_CONTROL_RW_8BIT                 0x10008071

#define REG_UART_A_STATE_R_4BIT                    0x1000810C
#define REG_UART_A_DET_BAUD_R_8BIT                 0x1000810D
#define REG_UART_A_TX_ITEMS_R_16BIT                0x1000810E
#define REG_UART_A_TX_RPTR_R_16BIT                 0x10008110
#define REG_UART_A_RX_ITEMS_R_16BIT                0x10008112
#define REG_UART_A_RX_RPTR_R_16BIT                 0x10008114

#define REG_UART_B_STATE_R_4BIT                    0x10008152
#define REG_UART_B_DET_BAUD_R_8BIT                 0x10008153
#define REG_UART_B_TX_ITEMS_R_16BIT                0x10008154
#define REG_UART_B_TX_RPTR_R_16BIT                 0x10008156
#define REG_UART_B_RX_ITEMS_R_16BIT                0x10008158
#define REG_UART_B_RX_RPTR_R_16BIT                 0x1000815A


#define REG_M0_LOCK_FLAG_RW_16BIT                  0x10004A00
#define REG_BT_LOCK_FLAG_R_16BIT                   0x10004A02
#define REG_LOCK_VICTIM(X)                         (0x10004A04+(X))
#define REG_RING_RX_START_ADDR_RW_16BIT            0x10004A14
#define REG_RING_RX_END_ADDR_RW_16BIT              0x10004A16
#define REG_RING_RX_RPTR_RW_16BIT                  0x10004A18
#define REG_RING_TX_START_ADDR_RW_16BIT            0x10004A1a
#define REG_RING_TX_END_ADDR_RW_16BIT              0x10004A1c
#define REG_RING_TX_WPTR_RW_16BIT                  0x10004A1e

//#define REG_RING_TX_S_ITEMS_R_16BIT               0x10004A20
#define REG_RING_TX_S_RPTR_R_16BIT                0x10004A20
//#define REG_RING_RX_S_ITEMS_R_16BIT               0x10004A24
#define REG_RING_RX_S_RPTR_R_16BIT                0x10004A22

#define SystemCoreClock 48000000

#define M0_MEMORY_BASE  0x10010000

#define _ALIGN_SIZE 4

#ifdef __cplusplus
}
#endif


#endif //_YC11XX_H

