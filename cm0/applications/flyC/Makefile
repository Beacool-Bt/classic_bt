GCC_ARM_PATH :=  E:\gcc-arm-none-eabi-5_4-2016q3-20160926-win32
THUMB_PATH := $(GCC_ARM_PATH)\arm-none-eabi\lib\thumb
CROSS_COMPILE := $(GCC_ARM_PATH)\bin\arm-none-eabi-

BT_PATH = ..\..\..\rom
PATCH_PATH = ..\..\patch
UTIL_PATH = $(BT_PATH)\util
INC = .\$(GCC_ARM_PATH)\arm-none-eabi\include
SDK_ROOT = ..\..
INCLUDES = -I$(SDK_ROOT)/ -I$(SDK_ROOT)/cmsis/ \
           -I$(SDK_ROOT)/device/ -I$(SDK_ROOT)/drivers/hal/ \
           -I$(SDK_ROOT)/drivers/uart/ -I$(SDK_ROOT)/libcore/uart/ \
           -I$(SDK_ROOT)/libcore/util/ -I$(SDK_ROOT)/drivers/log/ \
           -I$(SDK_ROOT)/libcore/bt/  \
           -I$(SDK_ROOT)/drivers/pwm/ -I$(SDK_ROOT)/libcore/pwm/ \
           -I$(SDK_ROOT)/drivers/gpio/ -I$(SDK_ROOT)/drivers/iic/ \
           -I$(SDK_ROOT)/patch/ -I$(SDK_ROOT)/drivers/bt/ \
           -I$(SDK_ROOT)/applications/flyC/lib/ -I$(SDK_ROOT)/applications/flyC/drivers/ \
           -I$(GCC_ARM_PATH)/arm-none-eabi/include

SRC_FILES = $(SDK_ROOT)/Drv_debug.c \
	    $(SDK_ROOT)/applications/flyC/lib/copter.c \
	    $(SDK_ROOT)/applications/flyC/lib/board.c \
            $(SDK_ROOT)/applications/flyC/lib/imu.c \
            $(SDK_ROOT)/applications/flyC/lib/scheduler.c \
	    $(SDK_ROOT)/applications/flyC/lib/config.c \
            $(SDK_ROOT)/applications/flyC/lib/sensor.c \
            $(SDK_ROOT)/applications/flyC/lib/pid.c \
            $(SDK_ROOT)/applications/flyC/lib/MyMath.c \
            $(SDK_ROOT)/applications/flyC/lib/motor.c \
            $(SDK_ROOT)/applications/flyC/lib/filter.c \
            $(SDK_ROOT)/applications/flyC/lib/param.c \
	    $(SDK_ROOT)/applications/flyC/lib/rc.c \
            $(SDK_ROOT)/applications/flyC/lib/anolink.c \
            $(SDK_ROOT)/applications/flyC/lib/control.c \
	    $(SDK_ROOT)/applications/flyC/drivers/yc11xx_iic.c \
            $(SDK_ROOT)/applications/flyC/drivers/systick.c \
	    $(SDK_ROOT)/applications/flyC/drivers/drv_i2c.c \
	    $(SDK_ROOT)/applications/flyC/drivers/drv_motors.c \
	    $(SDK_ROOT)/applications/flyC/drivers/drv_led.c \
	    $(SDK_ROOT)/applications/flyC/drivers/mpu6050.c \
            $(SDK_ROOT)/libcore/uart/app_uart.c \
            $(SDK_ROOT)/drivers/uart/yc11xx_uart.c \
            $(SDK_ROOT)/drivers/hal/yc_drv_common.c \
            $(SDK_ROOT)/libcore/sys/system.c \
            $(SDK_ROOT)/libcore/sys/kdevice.c \
            $(SDK_ROOT)/libcore/sys/kservice.c \
            $(SDK_ROOT)/drivers/pwm/yc11xx_pwm.c \
            $(SDK_ROOT)/drivers/iic/yc11xx_iic.c \
            $(SDK_ROOT)/drivers/spi/yc11xx_spi.c \
            $(SDK_ROOT)/drivers/gpio/yc11xx_gpio.c \
            $(SDK_ROOT)/drivers/bt/yc11xx_bt.c \
            $(SDK_ROOT)/libcore/pwm/app_pwm.c	\
           # $(SDK_ROOT)/libcore/iic/app_iic.c
LIB_FILES = $(SDK_ROOT)/drivers/uart/yc11xx_uart.c \
            $(SDK_ROOT)/drivers/hal/yc_drv_common.c \
            $(SDK_ROOT)/libcore/sys/system.c \
            $(SDK_ROOT)/libcore/sys/kdevice.c \
            $(SDK_ROOT)/libcore/sys/kservice.c \
            $(SDK_ROOT)/libcore/sys/mempool.c \
            $(SDK_ROOT)/drivers/pwm/yc11xx_pwm.c \
            $(SDK_ROOT)/drivers/iic/yc11xx_iic.c \
            $(SDK_ROOT)/drivers/spi/yc11xx_spi.c \
            $(SDK_ROOT)/drivers/gpio/yc11xx_gpio.c \
            $(SDK_ROOT)/drivers/bt/yc11xx_bt.c \
            $(SDK_ROOT)/drivers/bt/mesh.c
           
OBJ = ${patsubst %.c, %.o, ${SRC_FILES}} 
OBJ_LIB = ${patsubst %.c, %.o, ${LIB_FILES}} 
TARGET_LIB := yc11xx.a
LDSCRIPTS := -T $(SDK_ROOT)/gcc.ld -lgcc

LIBFILES := -lm -I${INC} -L.\$(THUMB_PATH) -lc
LIBDIR   := -L.
LIBDIR   += -L "E:\gcc-arm-none-eabi-5_4-2016q3-20160926-win32\lib\gcc\arm-none-eabi\5.4.1\armv6-m"
LIBDIR   += -L "E:\gcc-arm-none-eabi-5_4-2016q3-20160926-win32\arm-none-eabi\lib\armv6-m"

#$(warning   OBJIEAT is $(OBJ)) 

all : output\a.out
	$(CROSS_COMPILE)objdump -S $^ > output\disasm
	$(CROSS_COMPILE)objcopy $^ -O ihex output\out.hex
	perl $(UTIL_PATH)\hex2rom.pl 0 32 big < output\out.hex > output\out.rom
#   copy output\out.rom ..\rom\output\out.rom
	copy $(PATCH_PATH)\ramcode.rom .\output\ramcode.rom
	copy $(PATCH_PATH)\patch.dat + .\patch\user.dat + .\patch\rf.dat .\output\sched.rom
#   copy .\patch\sched.rom .\output\sched.rom
	copy $(PATCH_PATH)\memmap.format .\output\memmap.format
OBJS = startup_m0.o

output\a.out : $(OBJ) $(OBJS)
#	$(CROSS_COMPILE)ld $^ -o $@ -Ttext 0 -Tdata 0x10011000 -Map=output\memmap -lm -I${INC} -L.\$(THUMB_PATH)
	$(CROSS_COMPILE)ld $^ -o $@ $(LIBDIR) $(LIBFILES) $(LDSCRIPTS) -Map=output\memmap

$(OBJS) : $(SDK_ROOT)/startup_m0.s
	$(CROSS_COMPILE)as $^ -o $@ -mthumb -mcpu=cortex-m0 

$(OBJ) : %.o : %.c
	$(CROSS_COMPILE)gcc $^ -c $(INCLUDES) -g -O0 -o $@ -mthumb -mcpu=cortex-m0 -msoft-float

lib : $(OBJ_LIB)
	ar -r $(TARGET_LIB) $(OBJ_LIB)


clean :
	del /Q output\*.*
	del /Q /S .\*.o
	del /Q /S ..\..\*.o