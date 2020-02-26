GCC_ARM_PATH :=  E:\gcc-arm-none-eabi-5_4-2016q3-20160926-win32
THUMB_PATH := $(GCC_ARM_PATH)\arm-none-eabi\lib\thumb
CROSS_COMPILE := $(GCC_ARM_PATH)\bin\arm-none-eabi-

BT_PATH = .\respin
PATCH_PATH = .\patch
UTIL_PATH = $(BT_PATH)\util
INC = .\$(GCC_ARM_PATH)\arm-none-eabi\include
SDK_ROOT = .
INCLUDES = -I. -I$(SDK_ROOT)/ -I$(SDK_ROOT)/cmsis/ \
	  -I$(SDK_ROOT)/device/ -I$(SDK_ROOT)/drivers/hal/ \
	  -I$(SDK_ROOT)/drivers/uart/ -I$(SDK_ROOT)/libcore/uart/ \
	  -I$(SDK_ROOT)/drivers/timer/ -I$(SDK_ROOT)/drivers/cvsd/ \
	  -I$(SDK_ROOT)/drivers/keypad/ -I$(SDK_ROOT)/drivers/adc/ \
	  -I$(SDK_ROOT)/patch/  -I$(SDK_ROOT)/libcore/sys/ \
	  -I$(SDK_ROOT)/drivers/adc/ -I$(SDK_ROOT)/drivers/mic/ \
	  -I$(SDK_ROOT)/drivers/dac/  -I$(SDK_ROOT)/drivers/bt/ \
	  -I$(SDK_ROOT)/libcore/util/ -I$(SDK_ROOT)/drivers/log/ \
	  -I$(SDK_ROOT)/drivers/gpio/ -I$(SDK_ROOT)/drivers/keyscan/\
	  -I$(SDK_ROOT)/drivers/adpcm/ \
      -I$(GCC_ARM_PATH)/arm-none-eabi/include

SRC_FILES = $(SDK_ROOT)/drivers/timer/yc_timer.c \
			$(SDK_ROOT)/drivers/keypad/yc_keypad.c \
			$(SDK_ROOT)/drivers/adpcm/ima_enc.c \
			$(SDK_ROOT)/drivers/keypad/yc_queue.c \
			$(SDK_ROOT)/drivers/adc/drv_adc.c \
			$(SDK_ROOT)/drivers/mic/drv_mic.c \
            $(SDK_ROOT)/drivers/uart/yc11xx_uart.c \
            $(SDK_ROOT)/drivers/hal/yc_drv_common.c \
            $(SDK_ROOT)/drivers/iic/yc11xx_iic.c \
            $(SDK_ROOT)/drivers/spi/yc11xx_spi.c \
            $(SDK_ROOT)/drivers/gpio/yc11xx_gpio.c \
            $(SDK_ROOT)/drivers/bt/yc11xx_bt.c \
            $(SDK_ROOT)/drivers/pwm/yc11xx_pwm.c


OBJ = ${patsubst %.c, %.o, ${SRC_FILES}} 

LDSCRIPTS := -T  gcc.ld -lgcc

LIBFILES := -lm -I${INC} -L.\$(THUMB_PATH) -lc
LIBDIR   := -L.
LIBDIR   += -L "E:\gcc-arm-none-eabi-5_4-2016q3-20160926-win32\lib\gcc\arm-none-eabi\5.4.1\armv6-m"
LIBDIR   += -L "E:\gcc-arm-none-eabi-5_4-2016q3-20160926-win32\arm-none-eabi\lib\armv6-m"

#$(warning   OBJIEAT is $(OBJ)) 

genlib:$(OBJ)
	$(CROSS_COMPILE)ar rcs -o .\libcore\libyc11xx.a $(OBJ)

$(OBJS) :  startup_m0.s
	$(CROSS_COMPILE)as $^ -o $@ -mthumb -mcpu=cortex-m0 

$(OBJ) : %.o : %.c
	$(CROSS_COMPILE)gcc $^ -c $(INCLUDES) -g -O0 -o $@ -mthumb -mcpu=cortex-m0 -msoft-float -lyc11xx
    
clean :
	del /Q output\*.*
	del /Q /S .\*.o